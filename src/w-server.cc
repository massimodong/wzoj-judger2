/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * w-server.cc
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

#include "w-server.h"

using namespace grpc;

class WJudgerImpl final : public WJudger::Service {
	Status Judge(
		ServerContext *context,
		const JudgeArgs *args,
		ServerWriter<JudgeReply> *writer) override {
		return Status::OK;
	}
};

void WServer::Run(){
	std::string address("0.0.0.0:9717");
	WJudgerImpl impl;

	ServerBuilder builder;

	builder.AddListeningPort(address, InsecureServerCredentials());
	builder.RegisterService(&impl);

	std::unique_ptr<Server> server(builder.BuildAndStart());

	server->Wait();
}
