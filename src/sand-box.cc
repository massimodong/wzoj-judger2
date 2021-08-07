/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sand-box.cc
 * Copyright (C) 2021 Massimo Dong <ms@maxmute.com>
 *
 * wzoj-judger2 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * wzoj-judger2 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "common.h"
#include "sand-box.h"
#include "testcase.h"
#include <sys/resource.h>
#include <filesystem>
#include <seccomp.h>

const static int COMPILE_TIME = 60;
const static int COMPILE_FILE_SIZE = 10 * STD_MB;
const static int COMPILE_MEMORY = 256 * STD_MB;

#define BINDDIRS(ACTION)\
	ACTION("bin");\
	ACTION("usr");\
	ACTION("lib");\
	ACTION("lib64");

#define BIND(s)\
	safecall(mkdir, "compile/" s, S_IRWXU);\
	safecall(mount, "/" s, "compile/" s, "auto", MS_BIND, NULL);

#define UBIND(s)\
	safecall(umount, "compile/" s);\
	safecall(rmdir, "compile/" s);

void SandBox::start(const char *path){
	safecall(chdir, path);
	safecall(mkdir, "compile", S_IRWXU);
	safecall(chown, "compile", JUDGER_UID, JUDGER_UID);
	safecall(mkdir, "run", S_IRWXU);

	BINDDIRS(BIND);
}

void SandBox::end(){
	BINDDIRS(UBIND);
	safecall(rmdir, "compile");
	safecall(rmdir, "run");

	safecall(chdir, "..");
}

bool SandBox::compile_solution(Solution &solution){
	bool pass_compile = false;
	if(solution.problem.type == OJ_PROBLEM_TYPE_SUBMIT_SOLUTION)
		pass_compile = true;
	
	if(pass_compile) return true;

	prepare_compile_files(solution);

	bool ret = compile(solution.language);

	dpause();

	/*
	pid_t pid = fork_safe();
	if(pid == 0){
		safecall(chdir, "./compile");
		prepare_compile_files(solution);
		compile(solution.language);
	}else{
	}
	*/

	if(!ret){
		solution.ce = readFile("./compile/ce.txt");
	}else{
		safecall(rename, "./compile/Main", "Main");
	}

	clear_compile_files();

	return ret;
}

pid_t SandBox::fork_safe(){
	pid_t ret = fork();
	if(ret == -1){
		LOG(FATAL)<<"fork failed";
	}
	return ret;
}

void SandBox::setlimits(uint64_t time, uint64_t memory, uint64_t file_size){
	struct rlimit LIM;
	int ret = 0;
	LIM.rlim_max = LIM.rlim_cur = time;
	ret |= setrlimit(RLIMIT_CPU, &LIM);
	
	LIM.rlim_max = LIM.rlim_cur = memory;
	ret |= setrlimit(RLIMIT_AS, &LIM);
	
	LIM.rlim_max = LIM.rlim_cur = file_size;
	ret |= setrlimit(RLIMIT_FSIZE, &LIM);

	if(ret){
		LOG(FATAL)<<"Failed setting limits";
	}

	safecall(chroot, "./");

	safecall(setgid, JUDGER_UID);
	safecall(setuid, JUDGER_UID);
	safecall(setresuid, JUDGER_UID, JUDGER_UID, JUDGER_UID);
}

void SandBox::apply_seccomp(){
	scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
	if(ctx == NULL) LOG(FATAL)<<"ceccomp_init failed";

	const int whitelist[] = {SCMP_SYS(brk), SCMP_SYS(readlink), SCMP_SYS(read),
		SCMP_SYS(newfstatat), SCMP_SYS(write), SCMP_SYS(mprotect),
		SCMP_SYS(arch_prctl), SCMP_SYS(lseek), SCMP_SYS(uname),
		SCMP_SYS(execve), SCMP_SYS(exit_group)};

	for(int syscall: whitelist){
		seccomp_rule_add(ctx, SCMP_ACT_ALLOW, syscall, 0);
	}

	int load_res = seccomp_load(ctx);
	if(load_res) LOG(FATAL)<<"load seccomp contex failed";
	seccomp_release(ctx);
}

void SandBox::prepare_compile_files(const Solution &solution){
	safecall(chdir, "./compile");
	if(solution.problem.type == OJ_PROBLEM_TYPE_INTERACT){
		LOG(FATAL)<<"TODO";
	}
	FILE *fsrc = fopen(solution.srcFileName, "w");
	fputs(solution.code.c_str(), fsrc);
	fclose(fsrc);
	compile_files.push_back(solution.srcFileName);
	compile_files.push_back("ce.txt");
	safecall(chdir, "..");
}

void SandBox::clear_compile_files(){
	safecall(chdir, "./compile");
	for(auto &f: compile_files){
		safecall(unlink, f.c_str());
	}
	safecall(chdir, "..");
	LOG(WARNING)<<"clear compile files";
}

bool SandBox::compile(int language){
	pid_t pid = fork_safe();
	if(pid == 0){
		executeCompile(language);
	}else{
		int status = 0;
		safecall(waitpid, pid, &status, 0);
		DLOG(INFO)<<"status: "<<status;
		if(status){
			return false;
		}else{
			return true;
		}
	}
}

[[ noreturn ]] void SandBox::executeCompile(int language){
	const char * CP_C[] = { "gcc", "Main.c", "-o", "Main", "-O2", "-fno-asm", "-Wall",
			"-lm", "--static", "-std=c99", "-DONLINE_JUDGE", NULL };
	const char * CP_X[] = { "g++", "Main.cc", "-o", "Main", "-O2", "-fno-asm", "-Wall",
			"-lm", "--static", "-std=c++0x", "-DONLINE_JUDGE", NULL };
	const char * CP_P[] =
			{ "fpc", "Main.pas","-Cs32000000","-Sh", "-O2", "-Co",
				"-Ct", "-Ci", NULL };
	const char * CP_PY[] = {"/bin/cp", "Main.py", "Main", NULL};
	char * const * CP[] = {(char * const *)CP_C,
		                   (char * const *)CP_X,
		                   (char * const *)CP_P,
	                       NULL,
	                       (char * const *)CP_PY};
	
	safecall(chdir, "./compile");
	if(language == OJ_LANGUAGE_PASCAL){
		freopen("ce.txt", "w", stdout);
	}else{
		freopen("ce.txt", "w", stderr);
	}
	setlimits(COMPILE_TIME, COMPILE_MEMORY, COMPILE_FILE_SIZE);
	safecall(execvp, CP[language][0], CP[language]);
	LOG(FATAL)<<"should not reach here";
}

void SandBox::run_testcase(Testcase &testcase){
	safecall(chdir, "run");
	safecall(mkdir, testcase.name, S_IRWXU);
	safecall(chown, testcase.name, JUDGER_UID, JUDGER_UID);
	safecall(chdir, testcase.name);
	std::filesystem::copy_file(testcase.fin, "data.in");
	std::filesystem::copy_file("../../Main", "Main");

	dpause();

	int status = 0;
	struct rusage usage;
	
	pid_t pid = fork_safe();
	if(pid == 0){
		executeRunTestcase();
	}else{
		safecall(wait4, pid, &status, 0, &usage);
		dpause();
	}

	testcase.time_used = usage.ru_utime.tv_sec * (1000000ll) + usage.ru_utime.tv_usec;
	testcase.memory_used = usage.ru_maxrss;

	if(WIFEXITED(status) && (WEXITSTATUS(status) == 0)){
		int fdout = open("data.out", O_RDONLY);
		if(fdout == -1) LOG(FATAL)<<"Failed to open data.out";
		testcase.rate(fdout);
		safecall(close, fdout);
	}else{ // RE
		testcase.verdict = "RE";
		testcase.score = 100;
	}

	safecall(unlink, "Main");
	safecall(unlink, "data.in");
	safecall(unlink, "data.out");
	safecall(unlink, "re.txt");
	safecall(chdir, "..");
	safecall(rmdir, testcase.name);
	safecall(chdir, "..");
}

[[ noreturn ]] void SandBox::executeRunTestcase(){
	const char * Main[] = { "./Main", NULL };
	safecall_err(NULL, freopen, "data.in", "r", stdin);
	safecall_err(NULL, freopen, "data.out", "w", stdout);
	safecall_err(NULL, freopen, "re.txt", "w", stderr);
	setlimits(COMPILE_TIME, COMPILE_MEMORY, COMPILE_FILE_SIZE);
	apply_seccomp();
	safecall(execvp, Main[0], (char * const *)Main);
	LOG(FATAL)<<"should not reach here";
}