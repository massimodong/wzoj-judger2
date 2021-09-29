/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sand-box.h
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

#ifndef _SAND_BOX_H_
#define _SAND_BOX_H_

#include "solution.h"
#include "cpu-set-manager.h"

class Testcase;

class SandBox
{
public:
	void start(const char *path);
	void end();
	bool compile_solution(Solution &solution);
	void run_testcase(Testcase &);

protected:

private:
	pid_t fork_safe();
	void setlimits(uint64_t, uint64_t, uint64_t);
	void apply_seccomp();
	void prepare_compile_files(const Solution &solution);
	void clear_compile_files();
	bool compile(int language);
	[[ noreturn ]] void executeCompile(int language);
	[[ noreturn ]] void executeRunTestcase(uint64_t, double);

};

#endif // _SAND_BOX_H_

