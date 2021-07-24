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
	ncpu = get_nprocs();

	if(nworkcpu <= 0){
		LOG(FATAL)<<"Invalid working cpu number: "<<nworkcpu;
	}else if(nworkcpu >= ncpu){
		LOG(FATAL)<<"You have only "<<ncpu<<" cpus.";
	}
	
	LOG(INFO)<<"There are total "<<ncpu<<" cpus";
	LOG(INFO)<<"Using "<<nworkcpu<<" cpus as working cpu";
};

int CpuSetManager::isolate()
{
	return 0;
}

void CpuSetManager::release(int cpuid)
{
	// TODO: Add implementation here
}



