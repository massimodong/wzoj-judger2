/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * judger.h
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

#ifndef _JUDGER_H_
#define _JUDGER_H_

#include "common.h"
#include "cpu-set-manager.h"
#include "solutions-listener.h"
#include "judge-worker.h"

class Judger
{
public:
	Judger();
	void work();
	void run_once(int);

	void pushPendingSolution(int sid);
	int popPendingSolution();

protected:

private:
	 SolutionsListener solutionsListener;
	 std::vector<JudgeWorker> judgeWorkers;
	 
	 std::queue<int> pendingSolutionsQueue;
	 std::set<int> pendingSolutionsSet;
	 std::mutex pendingSolutionsMutex;
	 std::condition_variable pendingSolutionsCV;

};

#endif // _JUDGER_H_

