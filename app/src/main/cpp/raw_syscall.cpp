//
// Created by wengzulan on 2025/6/10.
//

#include "raw_syscall.h"
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/fcntl.h>  // For AT_FDCWD and other flags
#include <sys/syscall.h>  // For __NR_openat

int my_openat(int fd, const char *path, int oflag, mode_t mode) {
    return raw_syscall(__NR_openat, fd, path, oflag, mode);
}

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}
