/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * judger.cc
 * Copyright (C) 2022 Massimo Dong <ms@maxmute.com>
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

#include "judger.h"

Judger::Judger(std::string name, libconfig::Setting &setting):name(name), mutex(std::make_unique<std::mutex>()){
	LOG(INFO)<<"creating "<<name.c_str();

	token = std::string(setting["token"]);
	sandbox_size = int(setting["sandbox_size"]);

	safecall(mkdir, name.c_str(), S_IRWXU);
	safecall(chdir, name.c_str());
	for(int i=0;i<sandbox_size;++i){
		sandboxes.push(std::make_unique<Sandbox>(i));
	}
	safecall(chdir, "..");
}

Judger::~Judger(){
	LOG(INFO)<<"destroying "<<name.c_str();
	safecall(chdir, name.c_str());
	while(!sandboxes.empty()) sandboxes.pop();
	safecall(chdir, "..");
	safecall(rmdir, name.c_str());
}

bool Judger::token_match(std::string key){
	return token == key;
}

void Judger::judge(const JudgeTask &task){
	safecall(chdir, name.c_str());

	auto sandbox = fetch_sandbox();

	if(sandbox){
		sandbox->ready();

		int fd_ce = sandbox->open_ram_file();
		int id = sandbox->compile(task.language(), task.code(), fd_ce);
		if(id == -1){
			LOG(INFO)<<"ce";
			//TODO: report CE
		}else{
			//TODO: read testcases
			//TODO: report compile success
			//TODO: read from config file the judging procedure

			int fd_out = sandbox->open_file("user.out");
			//TODO: something like
			// usage = sandbox->execute_program(id, mappings)
			// where `id` is returned from compile
			// and `mappings` is std::vector<std::pair<int, int>> where we redirect `first` to `second` using dup2(2) (here, 1 -> fd_out)
			// and `usage` contains time and memory usage of this program
		}

		dpause();

		sandbox->clean();
		return_sandbox(std::move(sandbox));
	}else{
		//TODO: reply busy
	}

	safecall(chdir, "..");
}

std::unique_ptr<Sandbox> Judger::fetch_sandbox(){
	std::lock_guard<std::mutex> lk(*mutex);
	if(sandboxes.empty()){
		return nullptr;
	}else{
		auto sandbox = std::move(sandboxes.front());
		sandboxes.pop();
		return sandbox;
	}
}

void Judger::return_sandbox(std::unique_ptr<Sandbox> sandbox){
	std::lock_guard<std::mutex> lk(*mutex);
	sandboxes.push(std::move(sandbox));
}

static std::string getFdContent(int fd, size_t length = 4096){
	std::string ret;
	char buffer[OJ_SMALL_BUFFER_SIZE + 1];
	lseek(fd, 0, SEEK_SET);
	while(ret.length() < length){
		ssize_t count = read(fd, buffer, std::min(length, sizeof(buffer) - 1));
		if(count == -1){
			LOG(FATAL)<<"Error reading from file";
		}else if(count == 0){
			break;
		}else{
			buffer[count] = '\0';
			ret += buffer;
		}
	}
	return ret;
}

void Judger::simple(const SimpleTask &task){
	safecall(chdir, name.c_str());
	auto sandbox = fetch_sandbox();
	if(sandbox){
		sandbox->ready();
		dpause();
		int fd_ce = sandbox->open_ram_file();
		int exe_id = sandbox->compile(task.language(), task.code(), fd_ce);
		if(exe_id == -1){
			LOG(INFO)<<"ce";
			task.set_compileerror(getFdContent(fd_ce));
		}else{
			writeFile("user.in", task.input()); //TODO: this can be a ramfile too
			int fd_in = sandbox->open_file("user.in");
			int fd_out = sandbox->open_ram_file();
			int fd_err = sandbox->open_ram_file();
			dpause();
			std::vector<std::pair<int, int>> mappings;
			mappings.push_back(std::make_pair(0, fd_in));
			mappings.push_back(std::make_pair(1, fd_out));
			mappings.push_back(std::make_pair(2, fd_err));
			auto data = sandbox->execute_program(exe_id, mappings);
			LOG(INFO)<<"RE: "<<data.re();
			LOG(INFO)<<"time: "<<data.time_used;
			LOG(INFO)<<"memory: "<<data.memory_used;
			dpause();

			if(data.re()) task.set_runtimeerror(getFdContent(fd_err));
			task.set_timeused(data.time_used);
			task.set_memoryused(data.memory_used);
			task.set_output(getFdContent(fd_out));

		}

		sandbox->clean();
		return_sandbox(std::move(sandbox));
	}else{
		//TODO
	}
	safecall(chdir, "..");
}