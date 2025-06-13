//
// Created by wengzulan on 2025/6/13.
//

#include "ApkUtils.h"
#include "../Log.h"

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