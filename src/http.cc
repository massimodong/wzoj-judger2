/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * http.cc
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

#include "http.h"
#include "common.h"
#include "json.h"
#include "json-forwards.h"
#include <curl/curl.h>

Http::Http(){
	LOG(INFO)<<"Singleton class Http initialized";
	curl_global_init(CURL_GLOBAL_ALL);
}

Http& Http::getInstance(){
	static Http theInstance;
	return theInstance;
}

Json::Value Http::jsonDecode(const std::string *str){
	Json::CharReaderBuilder builder;
	Json::CharReader *reader = builder.newCharReader();
	Json::Value json;
	std::string errors;
	reader->parse(str->c_str(), str->c_str() + str->size(), &json, &errors);
	delete reader;
	return json;
}

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp){
	size_t realsize = size * nmemb;
	(*((std::string *)userp)) += (char *)buffer;
	return realsize;
}

Json::Value Http::raw_post(std::string url, std::string data, bool isPost){
	long code;
	std::string ret_str;
	while(true){
		CURL *curl = curl_easy_init();
		if(!curl){
			LOG(WARNING)<<"curl_easy_init failed, retrying.";
			for(int i=0;i<5;++i){
				sleep(3);
				curl = curl_easy_init();
				if(!curl){
					LOG(WARNING)<<"curl retry failed.";
				}else{
					LOG(INFO)<<"curl retry success.";
					break;
				}
			}
		}
		if(!curl){
			LOG(FATAL)<<"curl_easy_init failed for too many times.";
		}

		/**
		 * Perform as an ajax request
		 **/
		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, "X-Requested-With: XMLHttpRequest");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		/**
		 * url and data
		 **/
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		if(isPost){
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		}
		
		/**
		 * prepare to receive data
		 **/
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&ret_str);

		curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

		curl_slist_free_all(chunk);
		curl_easy_cleanup(curl);

		switch(code){
			case 500:
			case 501:
			case 502:
			case 503:
				LOG(WARNING)<<"server unavailable, retrying";
				sleep(10);
				break;
			case 200:
				/*succeed*/
				return jsonDecode(&ret_str);
			default:
				LOG(WARNING)<<"unrecognized status code: "<<code;
				sleep(10);
				break;
		}
	}
}

Json::Value Http::get(std::string url, std::map<std::string,std::string> par){
	CURL *curl = curl_easy_init();
	url = OJ_URL + url + "?judger_token=" + OJ_TOKEN;
	//std::cerr<<url.c_str();
	
	for(auto const &p: par){
		char *value = curl_easy_escape(curl, p.second.c_str(), 0);
		url += "&" + p.first + "=" + value;
		curl_free(value);
	}

	DLOG(INFO)<<"performing get request with url\n"<<url;

	curl_easy_cleanup(curl);
	return raw_post(url, std::string(""), false);
}

Json::Value Http::get(std::string url){
	std::map<std::string,std::string> par;
	return get(url, par);
}

Json::Value Http::post(std::string url, std::map<std::string,std::string> par){
	CURL *curl = curl_easy_init();
	url = OJ_URL + url + "?judger_token=" + OJ_TOKEN;
	//std::cerr<<url.c_str();
	
	std::string data;
	bool first_param = true;
	for(auto const &p: par){
		char *value = curl_easy_escape(curl, p.second.c_str(), 0);
		if(first_param){
			data += p.first + "=" + value;
			first_param = false;
		}else{
			data += "&" + p.first + "=" + value;
		}
		curl_free(value);
	}
	DLOG(INFO)<<"performin post request with url\n"<<url<<"\nand data"<<data;

	curl_easy_cleanup(curl);
	return raw_post(url,data,true);
}