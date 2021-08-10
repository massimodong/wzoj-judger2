/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * comparer.h
 * Copyright (C) 2021 Unknown <dongmassimo@gmail.com>
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

#ifndef _COMPARER_H_
#define _COMPARER_H_

#include "common.h"

const int COMP_BUFF_SIZE = 1024;
static_assert(COMP_BUFF_SIZE > 3);

class Comparer
{
public:
	Comparer(int);
	bool operator ==(Comparer &o);
	void reset();

protected:

private:
	int fd, c;
	char buffer[COMP_BUFF_SIZE];
	int pos, buffer_size;
	void fill_buffer();
	void read_bom();
	void read_char();
	bool reachNewLine();
	bool reachEOF();
};

#endif // _COMPARER_H_

