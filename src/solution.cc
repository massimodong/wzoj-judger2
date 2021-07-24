/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * solution.cc
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

#include "common.h"
#include "solution.h"

void Solution::load(int sid){
	std::map<std::string, std::string> par;
	par["solution_id"] = std::to_string(sid);
	Json::Value v = http.get("/judger/solution", par);
	DLOG(INFO)<<v;

	id = v["id"].asInt();
	user_id = v["user_id"].asInt();
	problem_id = v["problem_id"].asInt();
	language = v["language"].asInt();
	code = v["code"].asString();
	time_used = v["time_used"].asUInt64();
	memory_used = v["memory_used"].asDouble();
	status = v["status"].asInt();
	score = v["score"].asInt();
	ce = v["ce"].asString();
	cnt_testcases = v["cnt_testcases"].asInt();

	problem.load(problem_id);
}

bool Solution::checkout(int sid, bool force){
	std::map<std::string, std::string> par;
	par["solution_id"] = std::to_string(sid);
	if(force){
		par["force"] = "true";
	}
	return http.post("/judger/checkout", par)["ok"].asBool();
}