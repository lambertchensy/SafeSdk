//
// Created by wengzulan on 2025/6/11.
//

#ifndef OLLVMMODULE_MD5UTILS_H
#define OLLVMMODULE_MD5UTILS_H

#include <string>
#include <cstdint>

class MD5Utils {
public:
    static std::string md5(const std::string& input ,bool isOrigin);

private:
    // Helper functions and constants
    static inline uint32_t rotate_left(uint32_t x, int n);
    static inline uint32_t F(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t G(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t H(uint32_t x, uint32_t y, uint32_t z);
    static inline uint32_t I(uint32_t x, uint32_t y, uint32_t z);
    static void MD5Transform(uint32_t state[4], const unsigned char block[64], bool isOrigin);
};


#endif //OLLVMMODULE_MD5UTILS_H
