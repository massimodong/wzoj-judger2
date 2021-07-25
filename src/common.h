/*
 * common.h
 *
 * Copyright (C) 2021 - 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>

#include <glog/logging.h>
#include <queue>
#include <set>

#include <thread>
#include <mutex>
#include <condition_variable>

extern volatile bool OJ_RUNNING;

extern const char *OJ_HOME;
extern int OJ_SLEEP_TIME;
extern int OJ_CNT_WORKERS;
extern const char *OJ_URL;
extern const char *OJ_TOKEN;

const uint64_t STD_MB = 1048576;
const int OJ_PROBLEM_TYPE_SUBMIT_SOLUTION = 3;

#define safecall_err(err, fun, ...) if(fun(__VA_ARGS__) == err)\
  LOG(FATAL)<<#fun<<" "<<#__VA_ARGS__<<": "<<strerror(errno);

#define safecall(fun, ...) safecall_err(-1, fun, __VA_ARGS__)

#endif // _COMMON_H_