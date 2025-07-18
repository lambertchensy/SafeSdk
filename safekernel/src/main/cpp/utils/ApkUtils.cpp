//
// Created by wengzulan on 2025/6/13.
//
#include <sys/system_properties.h>  // For system properties
#include "ApkUtils.h"
#include "../Log.h"


string getApkPath(JNIEnv* env, jobject context) {
    std::string apkPath = ""; // 初始值为空字符串，作为出错时的默认返回值

    jclass contextClass = env->GetObjectClass( context);
    if (contextClass == NULL) {
        LOGE( "APK_PATH", "Failed to get context class");
        return apkPath;
    }

    // 获取 getPackageCodePath 方法
    jmethodID getPackageCodePathMethod = env->GetMethodID(
            contextClass,
            "getPackageCodePath",
            "()Ljava/lang/String;");

    if (getPackageCodePathMethod == NULL) {
        LOGE( "APK_PATH => Failed to get method ID");
        env->DeleteLocalRef(contextClass);
        return apkPath;
    }

    // 调用方法获取路径字符串
    jstring pathString = (jstring)env->CallObjectMethod( context, getPackageCodePathMethod);
    if (pathString == NULL) {
        LOGE( "APK_PATH => Failed to get package code path");
        env->DeleteLocalRef(contextClass);
        return apkPath;
    }

    // 转换为 C 字符串
    const char* apkPathChars = env->GetStringUTFChars(pathString, NULL);
    // 检查是否成功获取字符串
    if (apkPathChars == NULL) {
        LOGE("APK_PATH => Failed to convert jstring to const char*");
        env->DeleteLocalRef(pathString);
        env->DeleteLocalRef(contextClass);
        return apkPath;
    }
    apkPath = apkPathChars;

    // Release local references
    env->ReleaseStringUTFChars(pathString, apkPathChars);
    env->DeleteLocalRef(pathString);
    env->DeleteLocalRef(contextClass);

    return apkPath;
}


string getDataDirectory(JNIEnv *env, jobject context) {
    jclass contextCls = env->GetObjectClass(context);
    jmethodID getFilesDirMethod = env->GetMethodID(contextCls, "getFilesDir", "()Ljava/io/File;"); // Changed to getFilesDir
    if (getFilesDirMethod == nullptr) {
        LOGE("找不到 getFilesDir 方法");
        env->DeleteLocalRef(contextCls);
        return "";
    }

    jobject filesDirObj = env->CallObjectMethod(context, getFilesDirMethod);
    env->DeleteLocalRef(contextCls);
    if (filesDirObj == nullptr) {
        LOGE("getFilesDir 返回空对象");
        return "";
    }

    jclass fileCls = env->GetObjectClass(filesDirObj);
    jmethodID getAbsolutePathMethod = env->GetMethodID(fileCls, "getAbsolutePath", "()Ljava/lang/String;");
    if (getAbsolutePathMethod == nullptr) {
        LOGE("找不到 getAbsolutePath 方法");
        env->DeleteLocalRef(fileCls); // 释放 fileCls
        env->DeleteLocalRef(filesDirObj); // 释放 filesDirObj
        return "";
    }

    jstring pathStr = (jstring)env->CallObjectMethod(filesDirObj, getAbsolutePathMethod);
    env->DeleteLocalRef(fileCls); // 释放 fileCls
    env->DeleteLocalRef(filesDirObj); // 释放 filesDirObj
    if (pathStr == nullptr) {
        LOGE("getAbsolutePath 返回空字符串");
        return "";
    }

    const char *pathChars = env->GetStringUTFChars(pathStr, nullptr);
    std::string path(pathChars);
    env->ReleaseStringUTFChars(pathStr, pathChars);
    env->DeleteLocalRef(pathStr);
    return path;
}


// 获取设备品牌
string getDeviceBrand() {
    char brand[PROP_VALUE_MAX] = {0};
    __system_property_get("ro.product.brand", brand);
    return string(brand); // 从 char 数组创建 std::string 对象
}

// 获取 Android 系统版本
string getAndroidVersion() {
    char version[PROP_VALUE_MAX] = {0};
    __system_property_get("ro.build.version.release", version);
    return string(version); // 从 char 数组创建 std::string 对象
}