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

#include <unistd.h>
#include <glog/logging.h>
#include <queue>
#include <set>

#include <thread>
#include <mutex>
#include <condition_variable>

extern volatile bool OJ_RUNNING;
extern int OJ_SLEEP_TIME;
extern int OJ_CNT_WORKERS;
extern const char *OJ_URL;
extern const char *OJ_TOKEN;

const uint64_t STD_MB = 1048576;
const int OJ_PROBLEM_TYPE_SUBMIT_SOLUTION = 3;

#endif // _COMMON_H_