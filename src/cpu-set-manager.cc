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

CpuSetManager::CpuSetManager(){
	/*
	std::lock_guard<std::mutex> lk(mutex);
	ncpu = get_nprocs();
	
	LOG(INFO)<<"There are total "<<ncpu<<" cpus";
	nworkcpu = ncpu - 1;
	LOG(INFO)<<"Using "<<nworkcpu<<" cpus as working cpu";

	if(mkdir("/dev/cpuset", S_IRWXU))
		LOG(FATAL)<<"Failed creating directory /dev/cpuset: "
				  <<strerror(errno)<<"\n"
				  <<"Probably another process is using cpuset?";

	safecall(mount, "cputset", "/dev/cpuset", "cpuset", 0, NULL);

	safecall(mkdir, "/dev/cpuset/idleset", S_IRWXU);
	//safecall(mkdir, "/dev/cpuset/")

	for(int i=0;i<nworkcpu;++i) idle_cpus.emplace_back(i);
	nwaiting_tasks = 0;

	updateIdleCpus();
	*/
};

CpuSetManager::~CpuSetManager(){
	/*
	std::lock_guard<std::mutex> lk(mutex);
	safecall(umount, "/dev/cpuset");
	safecall(rmdir, "/dev/cpuset");
	*/
}

void CpuSetManager::use(){
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
		ready2idle(cpuid);
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

void CpuSetManager::ready2idle(int cpu_id){
	idle_cpus.emplace_back(cpu_id);
	updateIdleCpus();
}

void CpuSetManager::updateIdleCpus(){
}