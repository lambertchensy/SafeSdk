//
// Created by wengzulan on 2025/6/13.
//

#include "SafeKernel.h"
#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/fcntl.h>  // For AT_FDCWD and other flags
#include <sys/syscall.h>  // For __NR_openat
#include <sys/stat.h>
#include "Log.h"
#include "raw_syscall.h"
#include "sign/CheckSign.h"
#include "utils/Base64Utils.h"
#include "utils/ApkUtils.h"
#include "aes/aes_utils.h"
#include "aes/hex_utils.h"

//取apk签名的base64的md5的前10字符，然后再base64编码
#define TEMP_APK_SIGN_STR "MGY4YmI4YWIwMQ=="

const char* checkApkSign(const char* apkPath) {
    // 初始化标志位(最后处于1到9之间的检测结果的都算是正确)
    int sign_match_flag = 0;
    int permission_flag = 0;
    int path_match_flag = 0;


    if(apkPath == nullptr){
        LOGE("getApkPath == null");
        // 所有标志位都为 0
        std::string result = std::to_string(sign_match_flag) +
                             std::to_string(permission_flag) +
                             std::to_string(path_match_flag);
        return result.c_str();   // 将检测结果返回
    }
    LOGD("apkPath=%s",apkPath);

    //打开base.apk文件，获得fd
    int fd = my_openat(AT_FDCWD, reinterpret_cast<const char *>(apkPath), //AT_FDCWD 表示当前工作目录
                       O_RDONLY | O_CLOEXEC,
                       0640);
    if(fd < 0){
        LOGE("openat error => %s",apkPath);
        // 所有标志位都为 0
        std::string result = std::to_string(sign_match_flag) +
                             std::to_string(permission_flag) +
                             std::to_string(path_match_flag);
        return result.c_str();   // 将检测结果返回
    }

    //check svc apk sign
    //测试签名的md5返回的正确结果: fb4276cf05221ac060b5e0c8772431d7
    // 测试签名的base64的md5返回的正确结果:0f8bb8ab0188d116444b2dee0aafdb97 (0f8bb8ab01)
    const string &svc_apk_sign_md5 = checkSign(apkPath).substr(0, 10);
    LOGD("svc get apk sign=%s" ,svc_apk_sign_md5.c_str());
    //1、check svc get sign match
    if (svc_apk_sign_md5 == Base64Utils::VTDecode(TEMP_APK_SIGN_STR)) {
        LOGD("check svc get sign match");
        sign_match_flag = 1;
    }

    //2.文件权限检测(确认我们打开的fd是系统文件 。因为/data/app/包名/base.apk是一个系统文件，系统文件的gid和uid都是1000)
    struct stat statBuff = {0};
    long stat = raw_syscall(__NR_fstat, fd, &statBuff);
    if (stat < 0) {
        LOGE("check apk sign path fail __NR_fstat<0");
    }else {
        if (statBuff.st_uid == 1000 && statBuff.st_gid == 1000) {  // check uid&gid (1000 = system group)
            LOGD("check apk sign gid&uid success ");
            permission_flag = 1;
        } else {
            LOGE("check apk sign gid&uid fail ");
        }
    }

    //3、check apk path(Readlinkat反查apkPath)
    //4、Readlinkat返回值截断(如果攻击者只改了Readlinkat的参数，改了路径，但是返回值忘记修改，这样他的返回值就会被阶段，也就是大小不匹配，也可以检测出来 路径和传入的原始路径是否相等)
    char buff[PATH_MAX] = {0};
    std::string fdPath("/proc/");
    fdPath.append(to_string(getpid())).append("/fd/").append(to_string(fd));
    long len = raw_syscall(__NR_readlinkat, AT_FDCWD, fdPath.c_str(), buff, PATH_MAX);
    if (len < 0) {
        if(fd>0){
            close(fd);
        }
        LOGE("readlinkat error");
    }else{
        buff[len] = '\0'; //加这个，如果攻击者没修改readlinkat的返回值，就可以检测出来
        LOGD("check apk sign path =%s",buff);
        if (my_strcmp(apkPath, buff) == 0) {
            LOGD("check apk  path success ");
            path_match_flag = 1;
        }else {
            LOGE("check apk sign path fail ");
        }
    }
    //5、check Inode
    //6、对已经打开的fd进行签名获取:

    // 将标志位组合成字符串
    std::string result = std::to_string(sign_match_flag) +
                         std::to_string(permission_flag) +
                         std::to_string(path_match_flag);
    return result.c_str();   // 将检测结果返回
}

void writeToLogFile(Json::Value const& root)
{
    if (root.isMember("apkPath") && root.isMember("dataDir")) {
        std::string apkPath = root["apkPath"].asString();
        std::string dataDir = root["dataDir"].asString();
        //一.开始检测签名数据
        const char* result = checkApkSign(apkPath.c_str());
        LOGD("checkApkSign result:%s",result);

        //二.构建检测结果对应的json
        Json::Value new_root = root;
        new_root["A"] = result;  //签名检测结果
        Json::FastWriter fastWriter; // 使用 Json::FastWriter 生成紧凑 JSON 字符串
        Json::String resultJson = fastWriter.write(new_root);

        // 将检测结果AES加密后保存到本地
        char *aesEncrypt = AES_128_CBC_PKCS5_Encrypt(resultJson.c_str());
        unsigned char *inputDes = hex_decode(aesEncrypt);
        if(inputDes != NULL){
            if(!dataDir.empty()){
                size_t bytes_size = strlen(aesEncrypt)/2;
                std::string filePath = dataDir + "/" + ".log.dat";
                // 以二进制模式打开文件("wb" 模式会覆盖已存在的文件)
                FILE *outfile = fopen(filePath.c_str(), "wb");
                if (outfile != NULL) {
                    size_t written = fwrite(inputDes, 1, bytes_size, outfile);
                    if (written == bytes_size)  {
                        LOGD("写入文件成功");
                    }
                    fclose(outfile);
                }
            }
            free(inputDes);
        }
        //测试AES解密
//       char *aesDecrypt = AES_128_CBC_PKCS5_Decrypt(aesEncrypt);
//       LOGD("aesEncrypt=%s\n,aesDecrypt=%s",aesEncrypt,aesDecrypt);
//       free(aesDecrypt);

        free(aesEncrypt);

    }
}

