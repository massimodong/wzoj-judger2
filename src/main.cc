/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
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

#include "judger.h"
#include "common.h"
#include <getopt.h>
#include <signal.h>

#include "hiredis.h"

volatile bool OJ_RUNNING = true;

const char *OJ_HOME = "/home/judger";
int OJ_SLEEP_TIME = 1;
int OJ_CNT_WORKERS = 1;
const char *OJ_URL = "localhost:8080/";
const char *OJ_TOKEN = "123456";

void print_help(const char *name){
	printf("Usage: %s [options]\n", name);
	printf("Options:\n");
	printf("-h, --help\t\tDisplay help message.\n");
	printf("-v, --version\t\tDisplay version message.\n");
	printf("-w, --nworkcpu=<n>\tDedicate n cpus for judging.\n");
	printf("-i, --once\t\tRun once: judge one solution and exit.\n");
}

void print_version(const char *name){
}

void call_for_exit(int s){
	OJ_RUNNING = false;
	DLOG(WARNING)<<"Shutting down...";
}

int main(int argc, char* argv[])
{
	//google::InitGoogleLogging(argv[0]);
	int nworkcpu = 1;
	bool run_once = false;

	static const struct option longopts[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"nworkcpu", required_argument, NULL, 'w'},
		{"once", no_argument, NULL, 'i'},
		{NULL, 0, NULL, 0},
	};
	int optc;
	while((optc = getopt_long(argc, argv, ":hvw:i", longopts, NULL)) != -1){
		switch(optc){
			case 'h':
				print_help(argv[0]);
				return 0;
			case 'v':
				print_version(argv[0]);
				return 0;
			case 'w':
				nworkcpu = strtol(optarg, NULL, 0);
				break;
			case 'i':
				run_once = true;
				break;
			default:
				fprintf(stderr,"Try `%s --help` for more information.\n", argv[0]);
				return 0;
		}
	}

	safecall_err(SIG_ERR, signal, SIGQUIT, call_for_exit);
	safecall_err(SIG_ERR, signal, SIGKILL, call_for_exit);
	safecall_err(SIG_ERR, signal, SIGTERM, call_for_exit);
	safecall_err(SIG_ERR, signal, SIGINT, call_for_exit);

	safecall(chdir, OJ_HOME);

	Judger judger(nworkcpu);

	if(run_once){
	}else{
		judger.work();
	}
	
	return 0;
}

