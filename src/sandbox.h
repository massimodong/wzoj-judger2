/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * sandbox.h
 * Copyright (C) 2022 Massimo Dong <ms@maxmute.com>
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

#ifndef _SANDBOX_H_
#define _SANDBOX_H_

#include "common.h"

class Sandbox
{
public:
	Sandbox(int);
	~Sandbox();

	void ready();
	void clean();
	int compile(int language, std::string code);
	int open_file(const char *filename = NULL);
protected:

private:
	std::string name;

	std::vector<std::string> executable_files;
	std::vector<std::pair<int, std::string>> normal_files;
	int raw_compile(int language);
};

#endif // _SANDBOX_H_

