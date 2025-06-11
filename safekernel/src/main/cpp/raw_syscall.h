//
// Created by wengzulan on 2025/6/10.
//

#ifndef OLLVMMODULE_RAW_SYSCALL_H
#define OLLVMMODULE_RAW_SYSCALL_H

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/fcntl.h>  // For AT_FDCWD and other flags
#include <sys/syscall.h>  // For __NR_openat


extern "C" long raw_syscall(long __number, ...); //extern“C” 实现C++与C及其它语言的混合编程

//以下是调用raw_syscall或者自定义的实现libc中的部分函数
int  my_openat(int fd, const char *path, int oflag, mode_t mode);
int  my_strcmp(const char *s1, const char *s2);


#endif //OLLVMMODULE_RAW_SYSCALL_H
