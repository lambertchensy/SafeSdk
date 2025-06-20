#include <jni.h>
#include <string>
#include "json/json.h"
#include "utils/ApkUtils.h"
#include "aes/aes_utils.h"
#include "aes/hex_utils.h"
#include "Log.h"
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif



JNIEXPORT jstring JNICALL Java_com_qa_test_DeviceHelper_getDeviceInfo(JNIEnv *env, jclass clz, jobject appContext) {
//    std::string hello = "Hello from C++";
//    int a = atoi("sfvweg");
//    if(a == 0)
//        printf("%d",1);
//    else
//        printf("%d--f", 10);

    Json::Value json;
    json["info"]="This is jni getDeviceInfo test";
    string apkPath = getApkPath(env, appContext) ;
    if(!apkPath.empty()){
        json["apkPath"] = apkPath.c_str();
    }
    string deviceBrand = getDeviceBrand() ;
    json["deviceBrand"] = deviceBrand.c_str();

    string androidVersion = getAndroidVersion() ;
    json["androidVersion"] = androidVersion.c_str();

    string dataDir = getDataDirectory(env, appContext);
    json["dataDir"] = dataDir.c_str();

    Json::StreamWriterBuilder fastWriter;
    string strSendJson = Json::writeString(fastWriter, json);
    return env->NewStringUTF(strSendJson.c_str());
}

#ifdef __cplusplus
}
#endif






