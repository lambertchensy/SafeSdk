//
// Created by wengzulan on 2025/6/13.
//

#ifndef OLLVMMODULE_APKUTILS_H
#define OLLVMMODULE_APKUTILS_H

#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
using namespace std;

jstring getApkPath(JNIEnv* env, jobject context);
string getDeviceBrand();
string getAndroidVersion();

#endif //OLLVMMODULE_APKUTILS_H
