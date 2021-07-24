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
	cd(path);
}

bool SandBox::compile(const Solution &solution){
	bool pass_compile = false;
	if(solution.problem.type == OJ_PROBLEM_TYPE_SUBMIT_SOLUTION)
		pass_compile = true;
	
	if(pass_compile) return true;

	pid_t pid = fork_safe();
	if(pid == 0){
		cd("./compile");
		prepare_compile_files(solution);
		setlimits(COMPILE_TIME, COMPILE_MEMORY, COMPILE_FILE_SIZE);
	}else{
	}
}

void SandBox::cd(const char *path){
	if(chdir(path)){
		LOG(FATAL)<<"chdir to "<<path<<" failed";
	}
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
}

void SandBox::prepare_compile_files(const Solution &solution){
	
}