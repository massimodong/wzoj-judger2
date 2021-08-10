/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * comparer.cc
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

#include "comparer.h"

static inline bool isnewline(int c){
	return (c == '\n') || (c == '\r');
}

Comparer::Comparer(int fd):fd(fd), pos(0), buffer_size(0){
	read_bom();
}

bool Comparer::operator ==(Comparer &o){
	read_char();
	o.read_char();
	//DLOG(INFO)<<"start comparing";
	while(true){
		//DLOG(INFO)<<"main loop";
		if(c != o.c){
			if(c == EOF){
				return o.reachEOF();
			}else if(o.c == EOF){
				return reachEOF();
			}else if(isnewline(c) || isnewline(o.c)){
				bool ret = reachNewLine() && o.reachNewLine();
				if(!ret) return false;
			}else if(!(isspace(c) && isspace(o.c))){
				return false;
			}
		}else if(c == EOF){
			return true;
		}else{
			read_char();
			o.read_char();
		}
	}
	LOG(FATAL)<<"should not reach here";
}

void Comparer::read_bom(){
	fill_buffer();
	if(pos) LOG(FATAL)<<"???";
	if(buffer_size >= 3){
		if(buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF'){
			pos += 3;
		}
	}
}

void Comparer::fill_buffer(){
	assert(pos <= buffer_size);
	if(pos < buffer_size) return;

	if(buffer_size > 0 && buffer[0] == EOF) LOG(FATAL)<<"reaing already EOF file.";

	buffer_size = read(fd, buffer, COMP_BUFF_SIZE);
	if(buffer_size == -1) LOG(FATAL)<<"failed reading files";
	else if(buffer_size == 0) buffer[buffer_size++] = EOF;

	pos = 0;
}

void Comparer::read_char(){
	fill_buffer();
	if(pos >= buffer_size) LOG(FATAL)<<"???";
	c = buffer[pos++];
	
	//DLOG(INFO)<<fd<<" read char "<<c;
}
bool Comparer::reachNewLine(){
	while(!isnewline(c)){
		if(c == EOF) return true;
		if(!isspace(c)) return false;
		read_char();
	}
	while(isnewline(c)) read_char();
	return true;
}

bool Comparer::reachEOF(){
	while(c != EOF){
		if(!reachNewLine()) return false;
	}
	return true;
}

void Comparer::reset(){
	lseek(fd, 0, SEEK_SET);
	pos = buffer_size = 0;

	read_bom();
}