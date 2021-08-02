/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * remote-object.cc
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

#include "remote-object.h"
#include "http.h"
#include "common.h"

RemoteObject::RemoteObject():http(Http::getInstance()), remoteObjectLoaded(false){
}

RemoteObject::~RemoteObject(){
	if(remoteObjectLoaded) unload();
}

void RemoteObject::load(int id){
	load_real(id);
	remoteObjectLoaded = true;
}

void RemoteObject::load_real(int){
	LOG(FATAL)<<"Calling a virtual function RemoteObject::load";
}

void RemoteObject::unload(){
}