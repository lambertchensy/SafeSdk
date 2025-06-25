
#ifndef _LOG_H
#define _LOG_H

#include <jni.h>
#include <android/log.h>

#define TAG "XNTest"

#define LOG_SWITCH 0

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

//***************取apk签名md5的前10字符，然后再base64编码****************
//1.测试的debug包
//#define TEMP_APK_SIGN_STR "ZmI0Mjc2Y2YwNQ=="
//2.香肠正式版
//#define TEMP_APK_SIGN_STR "YmFiZGQyZmFjZA=="
//3.香肠先行版
#define TEMP_APK_SIGN_STR "MjgyOTljYWQ1Nw=="

#endif //_LOG_H
