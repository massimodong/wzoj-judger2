/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * solutions-listener.cc
 * Copyright (C) 2021  <massimo@DESKTOP-ONMM65U>
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

#include "solutions-listener.h"
#include "judger.h"

void SolutionsListener::startListening(Judger &judger, int sleep_time){
	if(sleep_time){
		workerThread = std::thread(worker, std::ref(judger), sleep_time);
	}else{
		workerThread = std::thread(worker_udp, std::ref(judger));
	}
}

void SolutionsListener::join(){
	workerThread.join();
}

void SolutionsListener::worker(Judger &judger, int sleep_time){
	do{
		probe(judger);
		sleep(sleep_time);
	}while(OJ_RUNNING);
}

void SolutionsListener::worker_udp(Judger &judger){
	LOG(FATAL)<<"TODO";
}

void SolutionsListener::probe(Judger &judger){
	Json::Value sols = GetRq("/judger/pending-solutions").get();
	DLOG(INFO)<<sols;
	for(int i=0;i<sols.size();++i){
		judger.pushPendingSolution(sols[i]["id"].asInt());
	}
}