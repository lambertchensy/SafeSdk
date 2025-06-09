#include <jni.h>
#include <string>
#include <cstdlib>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_ollvmmodule_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    int a = atoi("sfvweg");
    if(a == 0)
        printf("%d",1);
    else
        printf("%d--f", 10);
    return env->NewStringUTF(hello.c_str());
}