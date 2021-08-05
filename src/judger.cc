/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * judger.cc
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
#include "judger.h"

Judger::Judger(){
}

void Judger::work()
{
	solutionsListener.startListening(*this, OJ_SLEEP_TIME);
	for(int i=0;i<OJ_CNT_WORKERS;++i){
		judgeWorkers.emplace_back(i);
		judgeWorkers[i].startJudging(*this);
	}

	solutionsListener.join();

	pendingSolutionsCV.notify_all(); // tell all workers stop waiting

	for(int i=0;i<OJ_CNT_WORKERS;++i) judgeWorkers[i].join();
}

void Judger::run_once(int solution_id){
	judgeWorkers.emplace_back(0);
	dpause();
	judgeWorkers[0].judge(solution_id);
}

void Judger::pushPendingSolution(int sid){
	{
		std::lock_guard<std::mutex> lk(pendingSolutionsMutex);
		if(pendingSolutionsSet.count(sid) == 0){
			pendingSolutionsSet.insert(sid);
			pendingSolutionsQueue.push(sid);
		}
	}
	pendingSolutionsCV.notify_one();
}

int Judger::popPendingSolution(){
	int ret;
	std::unique_lock<std::mutex> lk(pendingSolutionsMutex);
	pendingSolutionsCV.wait(lk, [this]{
		return !OJ_RUNNING || !pendingSolutionsQueue.empty();
	});
	if(!OJ_RUNNING) return -1;
	ret = pendingSolutionsQueue.front();
	pendingSolutionsQueue.pop();
	pendingSolutionsSet.erase(ret);
	return ret;
}