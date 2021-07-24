/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * solutions-listener.h
 * Copyright (C) 2021  <massimo@DESKTOP-ONMM65U>
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

#ifndef _SOLUTIONS_LISTENER_H_
#define _SOLUTIONS_LISTENER_H_

#include "common.h"
#include "http.h"

class Judger;

class SolutionsListener
{
public:
	 void startListening(Judger &judger, int sleep_time);
	 void join();

protected:

private:
	std::thread workerThread;
	 
	static void worker(Judger &judger, int sleep_time);
	static void worker_udp(Judger &judger);
	static void probe(Judger &judger);

};

#endif // _SOLUTIONS_LISTENER_H_

