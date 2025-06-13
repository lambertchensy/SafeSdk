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
#include "utils/ApkUtils.h"
#include "SafeKernel.h"
using namespace std;




extern "C" JNIEXPORT jstring JNICALL
Java_com_example_safekernel_SafeHelper_getApkInfo(JNIEnv *env, jclass clz, jobject appContext) {
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
    jstring pathString = getApkPath(env, appContext) ;
    if(pathString != nullptr){
        const char* path = env->GetStringUTFChars(pathString, nullptr);
        const char* result = checkApkSign(path);
        LOGD("checkApkSign result:%s",result);
    }

    return env->NewStringUTF(strSendJson.c_str());
}






