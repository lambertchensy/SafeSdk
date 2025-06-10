#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/fcntl.h>  // For AT_FDCWD and other flags
#include <sys/syscall.h>  // For __NR_openat
#include <sys/stat.h>
#include "Log.h"
#include "raw_syscall.h"
#include "sign/CheckSign.h"
using namespace std;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ollvmmodule_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    int a = atoi("sfvweg");
    if(a == 0)
        printf("%d",1);
    else
        printf("%d--f", 10);
    return env->NewStringUTF(hello.c_str());
}


const char* getApkPath(JNIEnv* env, jobject context) {
    jclass contextClass = env->GetObjectClass( context);
    if (contextClass == NULL) {
        LOGE( "APK_PATH", "Failed to get context class");
        return nullptr;
    }

    // 获取 getPackageCodePath 方法
    jmethodID getPackageCodePathMethod = env->GetMethodID(
            contextClass,
            "getPackageCodePath",
            "()Ljava/lang/String;");

    if (getPackageCodePathMethod == NULL) {
        LOGE( "APK_PATH", "Failed to get method ID");
        env->DeleteLocalRef(contextClass);
        return nullptr;
    }

    // 调用方法获取路径字符串
    jstring pathString = (jstring)env->CallObjectMethod( context, getPackageCodePathMethod);
    if (pathString == NULL) {
        LOGE( "APK_PATH", "Failed to get package code path");
        env->DeleteLocalRef(contextClass);
        return nullptr;
    }

    // 转换为 C 字符串
    const char* path = env->GetStringUTFChars(pathString, NULL);

    // 清理本地引用
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(pathString);

    return path;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ollvmmodule_MainActivity_checkApkSign(JNIEnv* env,jobject context) {
    const char *path = getApkPath(env,context);
    LOGD("apkPath=%s",path);
    if(path == nullptr){
        LOGE("getApkPath == null");
    }

    //打开base.apk文件，获得fd
    int fd = my_openat(AT_FDCWD, reinterpret_cast<const char *>(path), //AT_FDCWD 表示当前工作目录
                       O_RDONLY | O_CLOEXEC,
                       0640);
    if(fd < 0){
        LOGE("openat error => %s",path);
    }

    //check svc apk sign
    const string &svcSign = checkSign(env, path).substr(0, 10);
    LOGD("svc get apk sign=%s" ,svcSign.c_str());
    //1、check svc get sign match

    //3、check apk path(Readlinkat反查apkPath)
    char buff[PATH_MAX] = {0};
    std::string fdPath("/proc/");
    fdPath.append(to_string(getpid())).append("/fd/").append(to_string(fd));
    long len = raw_syscall(__NR_readlinkat, AT_FDCWD, fdPath.c_str(), buff, PATH_MAX);
    if (len < 0) {
        if(fd>0){
            close(fd);
        }
        LOGE("readlinkat error");
    }

    //4、Readlinkat返回值截断
    buff[len] = '\0'; //加这个，如果攻击者没修改readlinkat的返回值，就可以检测出来
    LOGD("check apk sign path =%s",buff);
    if (my_strcmp(path, buff) == 0) {
        LOGD("check apk  path success ");
        //2.文件权限检测(确认我们打开的fd是系统文件 。因为/data/app/包名/base.apk是一个系统文件，系统文件的gid和uid都是1000)
        struct stat statBuff = {0};
        long stat = raw_syscall(__NR_fstat, fd, &statBuff);
        if (stat < 0) {
            LOGE("check apk sign path fail __NR_fstat<0");
        }
        if (statBuff.st_uid != 1000 && statBuff.st_gid != 1000) {  //check uid&gid (1000 = system group)
            LOGE("check apk sign gid&uid fail ");
        }
        //5、check Inode

    }else {
        LOGE("check apk sign path fail ");
    }

    return env->NewStringUTF(path);   // 真实签名和签名检测结果同步上传
}

