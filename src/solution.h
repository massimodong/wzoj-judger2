/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * solution.h
 * Copyright (C) 2021 Massimo <ms@maxmute.com>
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

#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include "remote-object.h"
#include "problem.h"

class Solution: public RemoteObject 
{
public:
	int id;
	int user_id;
	int problem_id;
	int language;
	std::string code;
	uint64_t time_used;
	double memory_used;
	int status;
	int score;
	std::string ce;
	int cnt_testcases;

	Problem problem;

	char *datadir;
	const char *srcFileName;

	void load_real(int);
	void unload();
	bool checkout(int, bool = false);

protected:

private:
	void loadC();
	void loadCPP();
	void loadPascal();

};

#endif // _SOLUTION_H_

