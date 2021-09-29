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

#include <dirent.h>

JudgeWorker::JudgeWorker(int worker_id):worker_id(worker_id){
	workdir = "run" + std::to_string(worker_id);
	safecall(mkdir, workdir.c_str(), S_IRWXU);
	sandbox.start(workdir.c_str());
}
JudgeWorker::JudgeWorker(JudgeWorker&& o):workerThread(std::move(o.workerThread)){
}
JudgeWorker::~JudgeWorker(){
	sandbox.end();
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
		int sid = judger.popPendingSolution();
		if(sid == -1) break;
		//if(Solution::checkout(sid)){
		//	judge(sid);
		//}
		judge(sid);
	}
}

static bool isInFile(const char *name){
	int l = strlen(name);
	if(l <= 3) return false;
	if(strcmp(name + l - 3, ".in") != 0) return false;
	return true;
}

static void readTestcases(const Solution &solution, std::vector<Testcase> &tcs){
	const char *path = solution.datadir;
	DIR *dp = opendir(path);
	dirent *dirp;
	if(dp != NULL){
		while((dirp = readdir(dp)) != NULL){
			if(dirp->d_type != DT_REG) continue; // skip if not a regular file
			if(!isInFile(dirp->d_name)) continue; // skip if not .in file
			std::string tnameIn(dirp->d_name);
			std::string tname = tnameIn.substr(0, tnameIn.length() - 3);

			tnameIn = path + ("/" + tnameIn);
			std::string tnameOut = path + ("/" + tname + ".out");
			std::string tnameAns = path + ("/" + tname + ".ans");

			if(access(tnameOut.c_str(), F_OK) == 0){
				tcs.emplace_back(tname.c_str(), tnameIn.c_str(), tnameOut.c_str(), std::ref(solution));
				DLOG(INFO)<<"emememe";
			}else if(access(tnameAns.c_str(), F_OK) == 0){
				tcs.emplace_back(tname.c_str(), tnameIn.c_str(), tnameAns.c_str(), std::ref(solution));
				DLOG(INFO)<<"mememem";
			}else{
				//TODO: notify not .out or .ans file exists!
			}
		}
	}else{
		//TODO: notify error!
		LOG(WARNING)<<"Failed opening directory "<<path;
		return;
	}
}

void JudgeWorker::judge(int sid){
	Solution solution;
	solution.load(sid);
	dpause();
	if(!sandbox.compile_solution(solution)){
		solution.reportCE();
		return;
	}

	std::vector<Testcase> testcases;
	readTestcases(solution, testcases);

	solution.reportTestcases(testcases);

	for(auto &t: testcases){
		t.run(sandbox);
	}

	for(auto &t: testcases){
		t.wait();
	}

	safecall(unlink, "Main");
}