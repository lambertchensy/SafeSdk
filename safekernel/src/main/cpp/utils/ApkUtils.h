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

jstring getApkPath(JNIEnv* env, jobject context);


#endif //OLLVMMODULE_APKUTILS_H
