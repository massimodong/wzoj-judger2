#ifndef _SYSCALL_WHITELIST_H_
#define _SYSCALL_WHITELIST_H_

#include <sys/syscall.h>

#define SYSCALL_ALLOWED(ACTION)\
	ACTION(SYS_exit_group),\
	ACTION(SYS_brk),\
	ACTION(SYS_mmap),\
	ACTION(SYS_munmap),\
	ACTION(SYS_write),\
	ACTION(SYS_read),\
	ACTION(SYS_fstat),\
	ACTION(SYS_newfstatat)

#define SYSCALL_ALLOWED_EXTENDED(ACTION)\
	ACTION(SYS_execve),\
	ACTION(SYS_prctl),\
	ACTION(SYS_rt_sigaction)

#define SYSCALL_ALLOWED_ALL(ACTION)\
	SYSCALL_ALLOWED(ACTION),\
	SYSCALL_ALLOWED_EXTENDED(ACTION)

#endif