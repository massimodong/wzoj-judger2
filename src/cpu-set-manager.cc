/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cpu-set-manager.cc
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

#include "cpu-set-manager.h"
#include "common.h"

#include <sys/sysinfo.h>

CpuSetManager& CpuSetManager::getInstance(){
	static CpuSetManager theInstance;
	return theInstance;
}

static void moveTasks(const char *from, const char *to){
	std::ifstream iftasks(from, std::ios::in);
	while(!iftasks.eof()){
		std::string task;
		iftasks>>task;
		writeFile(to, task);
	}
}

CpuSetManager::CpuSetManager(){
	std::lock_guard<std::mutex> lk(mutex);
	ncpu = get_nprocs();
	
	LOG(INFO)<<"There are total "<<ncpu<<" cpus";

	if(mkdir("/dev/cpuset", S_IRWXU))
		LOG(FATAL)<<"Failed creating directory /dev/cpuset: "
				  <<strerror(errno)<<"\n"
				  <<"Probably another process is using cpuset?";

	safecall(mount, "cputset", "/dev/cpuset", "cpuset", 0, NULL);

	safecall(mkdir, "/dev/cpuset/idleset", S_IRWXU);

	for(int i=1;i<ncpu;++i) idle_cpus.emplace_back(i);
	nwaiting_tasks = 0;

	setpath = (char **)malloc(sizeof(char *) * ncpu);
	for(int i=0;i<ncpu;++i){
		asprintf(setpath + i, "/dev/cpuset/set%d", i);
		safecall(mkdir, setpath[i], S_IRWXU);
		writeFile((setpath[i] + std::string("/cpus")).c_str(), std::to_string(i));
	}
	updateIdleCpus();

	std::string mems;
	std::ifstream ifs("/dev/cpuset/mems", std::ios::in);
	ifs>>mems;
	DLOG(INFO)<<"mems: "<<mems;
	writeFile("/dev/cpuset/idleset/mems", mems);
	for(int i=0;i<ncpu;++i){
		writeFile((setpath[i] + std::string("/mems")).c_str(), mems);
	}

	moveTasks("/dev/cpuset/tasks", "/dev/cpuset/idleset/tasks");
	dpause();
};

CpuSetManager::~CpuSetManager(){
	std::lock_guard<std::mutex> lk(mutex);

	moveTasks("/dev/cpuset/idleset/tasks", "/dev/cpuset/tasks");

	safecall(rmdir, "/dev/cpuset/idleset");
	for(int i=0;i<ncpu;++i) safecall(rmdir, setpath[i]);

	safecall(umount, "/dev/cpuset");
	safecall(rmdir, "/dev/cpuset");

	for(int i=0;i<ncpu;++i) free(setpath[i]);
	free(setpath);
}

int CpuSetManager::grab(){
	std::unique_lock<std::mutex> lk(mutex);
	++nwaiting_tasks;
	cv.wait(lk, [this]{return !idle_cpus.empty() || !ready_cpus.empty();});

	if(ready_cpus.empty()){
		int n = std::min((int)idle_cpus.size(), nwaiting_tasks);
		idle2ready(n);
	}

	assert(!ready_cpus.empty());
	int ret = ready_cpus.front();
	ready_cpus.pop();
	--nwaiting_tasks;
	return ret;
}

void CpuSetManager::release(int cpuid){
	std::unique_lock<std::mutex> lk(mutex);
	if(nwaiting_tasks > ready_cpus.size()){
		ready_cpus.push(cpuid);
	}else{
		makeidle(cpuid);
	}
}

void CpuSetManager::idle2ready(int n){
	for(int i=0;i<n;++i){
		assert(!idle_cpus.empty());
		ready_cpus.push(*idle_cpus.rbegin());
		idle_cpus.pop_back();
	}
	updateIdleCpus();
}

void CpuSetManager::makeidle(int cpu_id){
	idle_cpus.emplace_back(cpu_id);
	updateIdleCpus();
}

void CpuSetManager::updateIdleCpus(){
	std::string s("0");
	for(auto n: idle_cpus){
		s += "," + std::to_string(n);
	}
	writeFile("/dev/cpuset/idleset/cpus", s);
}