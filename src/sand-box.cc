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
#include <sys/resource.h>

const static int COMPILE_TIME = 60;
const static int COMPILE_FILE_SIZE = 10 * STD_MB;
const static int COMPILE_MEMORY = 256 * STD_MB;

void SandBox::start(const char *path){
	safecall(chdir, path);
}

bool SandBox::compile_solution(Solution &solution){
	bool pass_compile = false;
	if(solution.problem.type == OJ_PROBLEM_TYPE_SUBMIT_SOLUTION)
		pass_compile = true;
	
	if(pass_compile) return true;

	prepare_compile_files(solution);

	int ret = compile(solution.language);

	/*
	pid_t pid = fork_safe();
	if(pid == 0){
		safecall(chdir, "./compile");
		prepare_compile_files(solution);
		compile(solution.language);
	}else{
	}
	*/

	if(!ret) solution.ce = readFile("./compile/ce.txt");
	safecall(rename, "./compile/Main", "Main");

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

	while(setgid(JUDGER_UID)!=0) sleep(1);
	while(setuid(JUDGER_UID)!=0) sleep(1);
	while(setresuid(JUDGER_UID, JUDGER_UID, JUDGER_UID)!=0) sleep(1);
}

void SandBox::prepare_compile_files(const Solution &solution){
	safecall(chdir, "./compile");
	if(solution.problem.type == OJ_PROBLEM_TYPE_INTERACT){
		LOG(FATAL)<<"TODO";
	}
	FILE *fsrc = fopen(solution.srcFileName, "w");
	fputs(solution.code.c_str(), fsrc);
	fclose(fsrc);
	safecall(chdir, "..");
}

void SandBox::clear_compile_files(){
	LOG(WARNING)<<"clear compile files";
}

bool SandBox::compile(int language){
	pid_t pid = fork_safe();
	if(pid == 0){
		executeCompile(language);
	}else{
		int status = 0;
		safecall(waitpid, pid, &status, 0);
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