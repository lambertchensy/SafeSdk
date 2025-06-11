
#ifndef _LOG_H
#define _LOG_H

#include <jni.h>
#include <android/log.h>

#define TAG "XNTest"

#define LOG_SWITCH 1

#if LOG_SWITCH
    #define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
    #define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG ,__VA_ARGS__)
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG ,__VA_ARGS__)
    #define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG ,__VA_ARGS__)
#else
    #define LOGE(...)
    #define LOGI(...)
    #define LOGD(...)
    #define LOGW(...)
#endif

#endif //_LOG_H
