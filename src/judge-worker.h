/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * judge-worker.h
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

#ifndef _JUDGE_WORKER_H_
#define _JUDGE_WORKER_H_

#include "common.h"
#include "sand-box.h"

class Judger;

class JudgeWorker
{
public:
	JudgeWorker(int worker_id);
	JudgeWorker(JudgeWorker&&);
	~JudgeWorker();
	 
	void startJudging(Judger &);
	void judge(int);
	void join();
	 
protected:

private:
	int worker_id;
	std::string workdir;
	std::thread workerThread;

	SandBox sandbox;
	 
	void worker(Judger &judger);
};

#endif // _JUDGE_WORKER_H_

