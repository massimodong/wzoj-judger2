/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * post-rq.cc
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

#include "post-rq.h"

PostRq::PostRq(std::string url):Request(url){
}

Json::Value PostRq::post(){
	CURL *curl = curl_easy_init();
	std::string path = OJ_URL + url + "?judger_token=" + OJ_TOKEN;
	
	std::string data;
	bool first_param = true;
	for(auto const &p: params){
		char *value = curl_easy_escape(curl, p.second.c_str(), 0);
		if(first_param){
			data += p.first + "=" + value;
			first_param = false;
		}else{
			data += "&" + p.first + "=" + value;
		}
		curl_free(value);
	}
	DLOG(INFO)<<"performin post request at\n"<<path<<"\nand data\n"<<data;

	curl_easy_cleanup(curl);
	return http.raw_post(path, data, true);
}