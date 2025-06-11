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

int my_strcmp(const char *src, const char *dst) {
    int ret = 0 ;
    while( ! (ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
        ++src, ++dst;
    if ( ret < 0 )
        ret = -1 ;
    else if ( ret > 0 )
        ret = 1 ;
    return( ret );
}
