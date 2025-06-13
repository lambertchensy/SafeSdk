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
#include "utils/Base64Utils.h"
#include "json/json.h"
using namespace std;

//取apk签名的base64的md5的前10字符，然后再base64编码
#define TEMP_APK_SIGN_STR "MGY4YmI4YWIwMQ=="


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_safekernel_SafeHelper_stringFromJNI(JNIEnv *env, jclass clz, jobject appContext) {
    std::string hello = "Hello from C++";
    int a = atoi("sfvweg");
    if(a == 0)
        printf("%d",1);
    else
        printf("%d--f", 10);

    Json::Value json;
    json["test"]= "hello";json["value"]="word";
    json["ret"]= 10;
    Json::StreamWriterBuilder fastWriter;
    string strSendJson = Json::writeString(fastWriter, json);
    return env->NewStringUTF(strSendJson.c_str());
}


jstring getApkPath(JNIEnv* env, jobject context) {
    jstring apkPath = nullptr;
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
        LOGE( "APK_PATH => Failed to get method ID");
        env->DeleteLocalRef(contextClass);
        return nullptr;
    }

    // 调用方法获取路径字符串
    jstring pathString = (jstring)env->CallObjectMethod( context, getPackageCodePathMethod);
    if (pathString == NULL) {
        LOGE( "APK_PATH => Failed to get package code path");
        env->DeleteLocalRef(contextClass);
        return nullptr;
    }

    // 转换为 C 字符串
    const char* apkPathChars = env->GetStringUTFChars(pathString, NULL);
    // 检查是否成功获取字符串
    if (apkPathChars == NULL) {
        LOGE("APK_PATH => Failed to convert jstring to const char*");
        env->DeleteLocalRef(pathString);
        env->DeleteLocalRef(contextClass);
        return nullptr;
    }
    apkPath = env->NewStringUTF(apkPathChars);

    // Release local references
    env->ReleaseStringUTFChars(pathString, apkPathChars);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(pathString);

    return apkPath;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_safekernel_SafeHelper_checkApkSign(JNIEnv *env, jclass clz, jobject appContext) {
    // 初始化标志位(最后处于1到9之间的检测结果的都算是正确)
    int sign_match_flag = 0;
    int permission_flag = 0;
    int path_match_flag = 0;


    jstring apkPath = getApkPath(env,appContext);
    if(apkPath == nullptr){
        LOGE("getApkPath == null");
        return env->NewStringUTF("000"); // 所有标志位都为 0
    }
    const char* path = env->GetStringUTFChars(apkPath, nullptr);
    LOGD("apkPath=%s",path);

    //打开base.apk文件，获得fd
    int fd = my_openat(AT_FDCWD, reinterpret_cast<const char *>(path), //AT_FDCWD 表示当前工作目录
                       O_RDONLY | O_CLOEXEC,
                       0640);
    if(fd < 0){
        LOGE("openat error => %s",path);
        env->ReleaseStringUTFChars(apkPath, path); // 释放 C 字符串
        env->DeleteLocalRef(apkPath); // 释放局部引用
        return env->NewStringUTF("000"); // 所有标志位都为 0
    }

    //check svc apk sign
    //测试签名的md5返回的正确结果: fb4276cf05221ac060b5e0c8772431d7
    // 测试签名的base64的md5返回的正确结果:0f8bb8ab0188d116444b2dee0aafdb97 (0f8bb8ab01)
    const string &svc_apk_sign_md5 = checkSign(env, path).substr(0, 10);
    LOGD("svc get apk sign=%s" ,svc_apk_sign_md5.c_str());
    //1、check svc get sign match
    if (svc_apk_sign_md5 == Base64Utils::VTDecode(TEMP_APK_SIGN_STR)) {
        LOGD("check svc get sign match");
        sign_match_flag = 1;
    }

    //2.文件权限检测(确认我们打开的fd是系统文件 。因为/data/app/包名/base.apk是一个系统文件，系统文件的gid和uid都是1000)
    struct stat statBuff = {0};
    long stat = raw_syscall(__NR_fstat, fd, &statBuff);
    if (stat < 0) {
        LOGE("check apk sign path fail __NR_fstat<0");
    }else {
        if (statBuff.st_uid == 1000 && statBuff.st_gid == 1000) {  // check uid&gid (1000 = system group)
            LOGD("check apk sign gid&uid success ");
            permission_flag = 1;
        } else {
            LOGE("check apk sign gid&uid fail ");
        }
    }

    //3、check apk path(Readlinkat反查apkPath)
    //4、Readlinkat返回值截断(如果攻击者只改了Readlinkat的参数，改了路径，但是返回值忘记修改，这样他的返回值就会被阶段，也就是大小不匹配，也可以检测出来 路径和传入的原始路径是否相等)
    char buff[PATH_MAX] = {0};
    std::string fdPath("/proc/");
    fdPath.append(to_string(getpid())).append("/fd/").append(to_string(fd));
    long len = raw_syscall(__NR_readlinkat, AT_FDCWD, fdPath.c_str(), buff, PATH_MAX);
    if (len < 0) {
        if(fd>0){
            close(fd);
        }
        LOGE("readlinkat error");
    }else{
        buff[len] = '\0'; //加这个，如果攻击者没修改readlinkat的返回值，就可以检测出来
        LOGD("check apk sign path =%s",buff);
        if (my_strcmp(path, buff) == 0) {
            LOGD("check apk  path success ");
            path_match_flag = 1;
        }else {
            LOGE("check apk sign path fail ");
        }
    }
    //5、check Inode
    //6、对已经打开的fd进行签名获取:

    // 将标志位组合成字符串
    std::string result = std::to_string(sign_match_flag) +
                         std::to_string(permission_flag) +
                         std::to_string(path_match_flag);
    LOGD("checkApkSign result:%s",result.c_str());
    env->ReleaseStringUTFChars(apkPath, path); // 释放 C 字符串
    env->DeleteLocalRef(apkPath); // 释放局部引用
    return env->NewStringUTF(result.c_str());   // 将检测结果返回
}

