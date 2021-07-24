/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * http.h
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

#ifndef _HTTP_H_
#define _HTTP_H_

#include "json.h"
#include "json-forwards.h"

class Http
{
public:
	static Http& getInstance();
	Json::Value get(std::string url, std::map<std::string,std::string> par);
	Json::Value get(std::string url);
	Json::Value post(std::string url, std::map<std::string,std::string> par);
	

protected:

private:
	Http();
	Json::Value jsonDecode(const std::string *);
	Json::Value raw_post(std::string url, std::string data, bool isPost);
};

#endif // _HTTP_H_

