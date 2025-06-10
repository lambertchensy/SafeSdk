//
// Created by wengzulan on 2025/6/10.
//

#ifndef OLLVMMODULE_CHECKSIGN_H
#define OLLVMMODULE_CHECKSIGN_H

#include <jni.h>
#include <string>
using namespace std;

string checkSign(JNIEnv * env, const char* apkPath);

#endif //OLLVMMODULE_CHECKSIGN_H
