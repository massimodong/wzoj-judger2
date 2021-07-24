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

class SandBox
{
public:
	void start(const char *path);
	bool compile(const Solution &solution);

protected:

private:
	void cd(const char *path);
	pid_t fork_safe();
	void setlimits(uint64_t, uint64_t, uint64_t);
	void prepare_compile_files(const Solution &solution);

};

#endif // _SAND_BOX_H_

