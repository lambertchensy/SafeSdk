//
// Created by wengzulan on 2025/6/11.
//

#ifndef OLLVMMODULE_BASE64UTILS_H
#define OLLVMMODULE_BASE64UTILS_H
#include <string>

class Base64Utils {
public:
    static std::string VTEncode(const std::string& input);
    static std::string VTEncode(const std::vector<unsigned char>& input);
    static std::string VTDecode(const std::string& input);

private:
    static const std::string base64_chars;
};


#endif //OLLVMMODULE_BASE64UTILS_H
