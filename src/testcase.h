/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * testcase.h
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

#ifndef _TESTCASE_H_
#define _TESTCASE_H_

#include "common.h"
#include "sand-box.h"

class Testcase
{
public:
	Testcase(const char *name, const char *fin, const char *fout, const Solution &solution);
	Testcase(Testcase&&);
	~Testcase();
	void run(SandBox &);
	void wait();
	void rate(int);

	char *name, *fin, *fout;
	const Solution &solution;

	uint64_t time_used, memory_used;
	int score;
	std::string verdict;
protected:

private:
	std::thread thread;

	void run_testcase(SandBox &sandbox);

};

#endif // _TESTCASE_H_

