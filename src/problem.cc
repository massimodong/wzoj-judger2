/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * problem.cc
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

#include "problem.h"
#include "common.h"

void Problem::load_real(int pid){
	GetRq rq("/judger/problem");
	rq.addParam("problem_id", std::to_string(pid));
	Json::Value v = rq.get();

	DLOG(INFO)<<v;

	id = v["id"].asInt();
	name = v["name"].asString();
	memorylimit = v["memorylimit"].asDouble();
	timelimit = v["timelimit"].asUInt64();
	spj = v["spj"].asBool();
	type = v["type"].asInt();
}