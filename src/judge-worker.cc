/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * judge-worker.cc
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

#include "judge-worker.h"
#include "judger.h"
#include "solution.h"

JudgeWorker::JudgeWorker(int worker_id):worker_id(worker_id){
	workdir = "run" + std::to_string(worker_id);
	safecall(mkdir, workdir.c_str(), S_IRWXU);
	sandbox.start(workdir.c_str());
}
JudgeWorker::JudgeWorker(JudgeWorker&& o):workerThread(std::move(o.workerThread)){
}
JudgeWorker::~JudgeWorker(){
	safecall(rmdir, workdir.c_str());
}

void JudgeWorker::startJudging(Judger &judger){
	workerThread = std::thread(&JudgeWorker::worker, this, std::ref(judger));
}

void JudgeWorker::join(){
	workerThread.join();
}

void JudgeWorker::worker(Judger &judger){
	while(OJ_RUNNING){
		Solution solution;
		int sid = judger.popPendingSolution();
		if(sid == -1) break;
		if(solution.checkout(sid)){
			solution.load(sid);
			if(!sandbox.compile(solution)){
			}
		}
	}
}