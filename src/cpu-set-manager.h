/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * cpu-set-manager.h
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

#ifndef _CPU_SET_MANAGER_H_
#define _CPU_SET_MANAGER_H_

#include "common.h"

class CpuSetManager
{
public:
	CpuSetManager(int nworkcpu);
	~CpuSetManager();
	void isolate();
	int grab();
	void release(int cpuid);

protected:

private:
	int ncpu, nworkcpu;
	std::vector<int> idle_cpus;
	std::queue<int> ready_cpus;
	int nwaiting_tasks;
	std::mutex mutex;
	std::condition_variable cv;

	void idle2ready(int);
	void ready2idle(int);
	void updateIdleCpus();
};

#endif // _CPU_SET_MANAGER_H_