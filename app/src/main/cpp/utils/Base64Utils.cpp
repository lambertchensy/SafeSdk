//
// Created by wengzulan on 2025/6/11.
//

#include "Base64Utils.h"
#include <string>

const std::string Base64Utils::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64Utils::VTEncode(const std::vector<unsigned char>& input) {
    std::string encoded_string;
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    size_t in_len = input.size();

    for (i = 0; i < in_len;) {
        size_t j = 0;
        while (j < 3 && i < in_len) {
            char_array_3[j++] = input[i++];
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((j > 1 ? (char_array_3[1] & 0xf0) >> 4 : 0));
        char_array_4[2] = ((j > 1 ? (char_array_3[1] & 0x0f) << 2 : 0)) + ((j > 2 ? (char_array_3[2] & 0xc0) >> 6 : 0));
        char_array_4[3] = (j > 2 ? char_array_3[2] & 0x3f : 0);

        for (size_t k = 0; k < j + 1; k++) {
            encoded_string += base64_chars[char_array_4[k]];
        }

        while (j++ < 3) {
            encoded_string += '=';
        }
    }

    return encoded_string;
}


std::string Base64Utils::VTEncode(const std::string &input) {
    // 将std::string转换为std::vector<uint8_t>
    std::vector<uint8_t> bytes(input.begin(), input.end());
    // 调用原始的VTEncode方法
    return VTEncode(bytes);
}

std::string Base64Utils::VTDecode(const std::string &input) {
    std::string decoded_string;
    std::vector<int> char_values;

    for (char c : input) {
        if (c == '=') {
            char_values.push_back(-1); // 标记填充字符
        } else {
            size_t pos = base64_chars.find(c);
            if (pos == std::string::npos) {
                // Invalid character
                return ""; // Or throw an exception
            }
            char_values.push_back(pos);
        }
    }

    for (size_t i = 0; i < char_values.size(); i += 4) {
        int char_array_4[4];
        for (int j = 0; j < 4; ++j) {
            if (i + j < char_values.size()) {
                char_array_4[j] = char_values[i + j];
            } else {
                char_array_4[j] = -1; // Handle incomplete blocks
            }
        }

        if (char_array_4[0] == -1 || char_array_4[1] == -1) break;

        decoded_string += (char)(((char_array_4[0] << 2) | ((char_array_4[1] & 0x30) >> 4)));

        if (char_array_4[2] != -1) {
            decoded_string += (char)(((char_array_4[1] & 0x0f) << 4) | ((char_array_4[2] & 0x3c) >> 2));
        }

        if (char_array_4[3] != -1) {
            decoded_string += (char)(((char_array_4[2] & 0x03) << 6) | char_array_4[3]);
        }
    }
    return decoded_string;
}
