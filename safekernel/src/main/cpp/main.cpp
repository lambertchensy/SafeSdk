#include <jni.h>
#include <string>
#include "json/json.h"
#include "utils/ApkUtils.h"
using namespace std;




extern "C" JNIEXPORT jstring JNICALL
Java_com_qa_test_DeviceHelper_getDeviceInfo(JNIEnv *env, jclass clz, jobject appContext) {
//    std::string hello = "Hello from C++";
//    int a = atoi("sfvweg");
//    if(a == 0)
//        printf("%d",1);
//    else
//        printf("%d--f", 10);

    Json::Value json;
    json["info"]="This is jni getDeviceInfo test";
    jstring pathString = getApkPath(env, appContext) ;
    if(pathString != nullptr){
        const char* apkPath = env->GetStringUTFChars(pathString, nullptr);
        json["apkPath"] = apkPath;
        env->ReleaseStringUTFChars(pathString, apkPath);
        env->DeleteLocalRef(pathString);
    }
    string deviceBrand = getDeviceBrand() ;
    json["deviceBrand"] = deviceBrand.c_str();

    string androidVersion = getAndroidVersion() ;
    json["androidVersion"] = androidVersion.c_str();

    Json::StreamWriterBuilder fastWriter;
    string strSendJson = Json::writeString(fastWriter, json);
    return env->NewStringUTF(strSendJson.c_str());
}






