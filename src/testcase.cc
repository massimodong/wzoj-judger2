/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * testcase.cc
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

#include "testcase.h"
class Solution;

Testcase::Testcase(const char *name_, const char *fin_, const char *fout_, const Solution &solution):solution(solution){
	name = strdup(name_);
	fin = strdup(fin_);
	fout = strdup(fout_);
	LOG(INFO)<<"name: "<<name<<"\nfin: "<<fin<<"\nfout: "<<fout;
}

Testcase::Testcase(Testcase&& o):solution(o.solution){
	DLOG(INFO)<<"move "<<o.name;
	name = o.name;
	fin = o.fin;
	fout = o.fout;
	o.name = o.fin = o.fout = NULL;
	thread = std::move(o.thread);
}

Testcase::~Testcase(){
	if(name){
		assert(fin);
		assert(fout);
		DLOG(INFO)<<"Destroying testcase "<<name;
		free(name);
		free(fin);
		free(fout);
	}
}

void Testcase::run(SandBox &sandbox){
	thread = std::thread(&Testcase::run_testcase, this, std::ref(sandbox));
}

void Testcase::run_testcase(SandBox &sandbox){
	sandbox.run_testcase(std::ref(*this));
}

void Testcase::wait(){
	thread.join();
}

void Testcase::rate(int fdout){
	//TODO
	verdict = "AC";
	score = 100;
}