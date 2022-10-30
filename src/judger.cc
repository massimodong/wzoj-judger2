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

		int id = sandbox->compile(task.language(), task.code());
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