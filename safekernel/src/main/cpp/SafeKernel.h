//
// Created by wengzulan on 2025/6/13.
//

#ifndef OLLVMMODULE_SAFEKERNEL_H
#define OLLVMMODULE_SAFEKERNEL_H

#include "json/json.h"

const char* checkApkSign(const char* apkPath);
void writeToLogFile(Json::Value const& root);

#endif //OLLVMMODULE_SAFEKERNEL_H
