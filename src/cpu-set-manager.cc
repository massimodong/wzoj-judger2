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

CpuSetManager::CpuSetManager(int nworkcpu):nworkcpu(nworkcpu){
	std::lock_guard<std::mutex> lk(mutex);
	ncpu = get_nprocs();

	if(nworkcpu <= 0){
		LOG(FATAL)<<"Invalid working cpu number: "<<nworkcpu;
	}else if(nworkcpu >= ncpu){
		LOG(FATAL)<<"You have only "<<ncpu<<" cpus.";
	}
	
	LOG(INFO)<<"There are total "<<ncpu<<" cpus";
	LOG(INFO)<<"Using "<<nworkcpu<<" cpus as working cpu";

	if(mkdir("/dev/cpuset", S_IRWXU))
		LOG(FATAL)<<"Failed creating directory /dev/cpuset: "
				  <<strerror(errno)<<"\n"
				  <<"Probably another process is using cpuset?";

	safecall(mount, "cputset", "/dev/cpuset", "cpuset", 0, NULL);

	for(int i=0;i<nworkcpu;++i) idle_cpus.emplace_back(i);
	nwaiting_tasks = 0;
};

CpuSetManager::~CpuSetManager(){
	std::lock_guard<std::mutex> lk(mutex);
	safecall(umount, "/dev/cpuset");
	safecall(rmdir, "/dev/cpuset");
}

void CpuSetManager::isolate(){
}

int CpuSetManager::grab(){
	std::unique_lock<std::mutex> lk(mutex);
	++nwaiting_tasks;
	cv.wait(lk, [this]{return !idle_cpus.empty() || !ready_cpus.empty();});

	if(ready_cpus.empty()){
		int n = std::min((int)idle_cpus.size(), nwaiting_tasks);
		makeReady(n);
	}

	assert(!ready_cpus.empty());
	int ret = ready_cpus.front();
	ready_cpus.pop();
	--nwaiting_tasks;
	return ret;
}

void CpuSetManager::release(int cpuid){
	std::unique_lock<std::mutex> lk(mutex);
}

void CpuSetManager::makeReady(int n){
	for(int i=0;i<n;++i){
		ready_cpus.push(idle_cpus.)
	}
}

