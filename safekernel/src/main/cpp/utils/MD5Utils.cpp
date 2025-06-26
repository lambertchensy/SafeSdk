//
// Created by wengzulan on 2025/6/11.
//

#include "MD5Utils.h"
#include <sstream>
#include <iomanip>
#include <cmath>

// Rotates x left n bits.
inline uint32_t MD5Utils::rotate_left(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

// MD5 F, G, H and I transformations.
inline uint32_t MD5Utils::F(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) | (~x & z);
}

inline uint32_t MD5Utils::G(uint32_t x, uint32_t y, uint32_t z) {
    return (x & z) | (y & ~z);
}

inline uint32_t MD5Utils::H(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ y ^ z;
}

inline uint32_t MD5Utils::I(uint32_t x, uint32_t y, uint32_t z) {
    return y ^ (x | ~z);
}

// Function to dynamically calculate K constants
//方法二:md5会进行64轮运算，每轮运算都会用到一个常量，组成一个常量表K。
//K原始值的计算方式是 2^32 * |sin i |，而后取其整数部分。
//那么有理想的同学就可以更改这个K值，比如把 sin改成 cos或者tan之类的
std::array<uint32_t, 64> generate_k_constants() {
    std::array<uint32_t, 64> k_constants;
    for (int i = 0; i < 64; ++i) {
        k_constants[i] = static_cast<uint32_t>(std::floor(std::abs(std::cos(static_cast<double>(i + 1))) * std::pow(2.0, 32.0)));
    }
    return k_constants;
}

//方法一：最简单的魔改方法就是改变MD5的初始参数
std::string MD5Utils::md5(const std::string& input) {
    std::array<uint32_t, 64> K = generate_k_constants();
    uint32_t state[4] = {K[8], K[5], K[2], K[0]}; //{0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    unsigned char padding[64] = {0x80}; // Padding starts with 0x80
    uint64_t input_length_bits = input.length() * 8; // Length in bits

    // Pad the input to a multiple of 512 bits (64 bytes)
    size_t padding_length = 64 - (input.length() % 64);
    if (padding_length <= 8) {
        padding_length += 64; // Need an extra block for length
    }

    std::string padded_input = input;

    // Append 0x80
    padded_input += static_cast<char>(padding[0]);  // Convert unsigned char to char

    // Append 0x00 padding
    padded_input.append(padding_length - 9, 0x00);

    // Append length in bits as 64-bit little-endian integer.
    for (int i = 0; i < 8; ++i) {
        padded_input += static_cast<char>((input_length_bits >> (i * 8)) & 0xFF);
    }

    // Process the input in 64-byte blocks
    for (size_t i = 0; i < padded_input.length(); i += 64) {
        MD5Transform(state, (const unsigned char*)padded_input.substr(i, 64).c_str());
    }

//    // Format the output as a hexadecimal string(大端字节序，这个结果可能不对)
//    std::stringstream ss;
//    for (int i = 0; i < 4; ++i) {
//        ss << std::hex << std::setw(8) << std::setfill('0') << state[i];
//    }

    // Format the output as a hexadecimal string(小端字节序)
    std::stringstream ss;
    for (int i = 0; i < 4; ++i) {
        // Reverse the byte order for each 32-bit word
        uint32_t value = state[i];
        unsigned char bytes[4];
        bytes[0] = (value >> 0) & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;
        bytes[2] = (value >> 16) & 0xFF;
        bytes[3] = (value >> 24) & 0xFF;

        // Append the reversed bytes to the stringstream
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[0];
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[1];
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[2];
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[3];
    }
    return ss.str();
}

// Apply the MD5 transformations for each round.
void MD5Utils::MD5Transform(uint32_t state[4], const unsigned char block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    // Decode the block into x[i].
    for (int i = 0; i < 16; i++) {
        x[i] = (block[i * 4]) | (block[i * 4 + 1] << 8) | (block[i * 4 + 2] << 16) | (block[i * 4 + 3] << 24);
    }

    // Generate K constants dynamically
    std::array<uint32_t, 64> K = generate_k_constants();

    // Round 1
    a = b + rotate_left(a + F(b, c, d) + x[0] + K[0], 7);
    d = a + rotate_left(d + F(a, b, c) + x[1] + K[1], 12);
    c = d + rotate_left(c + F(d, a, b) + x[2] + K[2], 17);
    b = c + rotate_left(b + F(c, d, a) + x[3] + K[3], 22);
    a = b + rotate_left(a + F(b, c, d) + x[4] + K[4], 7);
    d = a + rotate_left(d + F(a, b, c) + x[5] + K[5], 12);
    c = d + rotate_left(c + F(d, a, b) + x[6] + K[6], 17);
    b = c + rotate_left(b + F(c, d, a) + x[7] + K[7], 22);
    a = b + rotate_left(a + F(b, c, d) + x[8] + K[8], 7);
    d = a + rotate_left(d + F(a, b, c) + x[9] + K[9], 12);
    c = d + rotate_left(c + F(d, a, b) + x[10] + K[10], 17);
    b = c + rotate_left(b + F(c, d, a) + x[11] + K[11], 22);
    a = b + rotate_left(a + F(b, c, d) + x[12] + K[12], 7);
    d = a + rotate_left(d + F(a, b, c) + x[13] + K[13], 12);
    c = d + rotate_left(c + F(d, a, b) + x[14] + K[14], 17);
    b = c + rotate_left(b + F(c, d, a) + x[15] + K[15], 22);

    // Round 2
    a = b + rotate_left(a + G(b, c, d) + x[1] + K[16], 5);
    d = a + rotate_left(d + G(a, b, c) + x[6] + K[17], 9);
    c = d + rotate_left(c + G(d, a, b) + x[11] + K[18], 14);
    b = c + rotate_left(b + G(c, d, a) + x[0] + K[19], 20);
    a = b + rotate_left(a + G(b, c, d) + x[5] + K[20], 5);
    d = a + rotate_left(d + G(a, b, c) + x[10] + K[21], 9);
    c = d + rotate_left(c + G(d, a, b) + x[15] + K[22], 14);
    b = c + rotate_left(b + G(c, d, a) + x[4] + K[23], 20);
    a = b + rotate_left(a + G(b, c, d) + x[9] + K[24], 5);
    d = a + rotate_left(d + G(a, b, c) + x[14] + K[25], 9);
    c = d + rotate_left(c + G(d, a, b) + x[3] + K[26], 14);
    b = c + rotate_left(b + G(c, d, a) + x[8] + K[27], 20);
    a = b + rotate_left(a + G(b, c, d) + x[13] + K[28], 5);
    d = a + rotate_left(d + G(a, b, c) + x[2] + K[29], 9);
    c = d + rotate_left(c + G(d, a, b) + x[7] + K[30], 14);
    b = c + rotate_left(b + G(c, d, a) + x[12] + K[31], 20);

    // Round 3
    a = b + rotate_left(a + H(b, c, d) + x[5] + K[32], 4);
    d = a + rotate_left(d + H(a, b, c) + x[8] + K[33], 11);
    c = d + rotate_left(c + H(d, a, b) + x[11] + K[34], 16);
    b = c + rotate_left(b + H(c, d, a) + x[14] + K[35], 23);
    a = b + rotate_left(a + H(b, c, d) + x[1] + K[36], 4);
    d = a + rotate_left(d + H(a, b, c) + x[4] + K[37], 11);
    c = d + rotate_left(c + H(d, a, b) + x[7] + K[38], 16);
    b = c + rotate_left(b + H(c, d, a) + x[10] + K[39], 23);
    a = b + rotate_left(a + H(b, c, d) + x[13] + K[40], 4);
    d = a + rotate_left(d + H(a, b, c) + x[0] + K[41], 11);
    c = d + rotate_left(c + H(d, a, b) + x[3] + K[42], 16);
    b = c + rotate_left(b + H(c, d, a) + x[6] + K[43], 23);
    a = b + rotate_left(a + H(b, c, d) + x[9] + K[44], 4);
    d = a + rotate_left(d + H(a, b, c) + x[12] + K[45], 11);
    c = d + rotate_left(c + H(d, a, b) + x[15] + K[46], 16);
    b = c + rotate_left(b + H(c, d, a) + x[2] + K[47], 23);

    // Round 4
    a = b + rotate_left(a + I(b, c, d) + x[0] + K[48], 6);
    d = a + rotate_left(d + I(a, b, c) + x[7] + K[49], 10);
    c = d + rotate_left(c + I(d, a, b) + x[14] + K[50], 15);
    b = c + rotate_left(b + I(c, d, a) + x[5] + K[51], 21);
    a = b + rotate_left(a + I(b, c, d) + x[12] + K[52], 6);
    d = a + rotate_left(d + I(a, b, c) + x[3] + K[53], 10);
    c = d + rotate_left(c + I(d, a, b) + x[10] + K[54], 15);
    b = c + rotate_left(b + I(c, d, a) + x[1] + K[55], 21);
    a = b + rotate_left(a + I(b, c, d) + x[8] + K[56], 6);
    d = a + rotate_left(d + I(a, b, c) + x[15] + K[57], 10);
    c = d + rotate_left(c + I(d, a, b) + x[6] + K[58], 15);
    b = c + rotate_left(b + I(c, d, a) + x[13] + K[59], 21);
    a = b + rotate_left(a + I(b, c, d) + x[4] + K[60], 6);
    d = a + rotate_left(d + I(a, b, c) + x[11] + K[61], 10);
    c = d + rotate_left(c + I(d, a, b) + x[2] + K[62], 15);
    b = c + rotate_left(b + I(c, d, a) + x[9] + K[63], 21);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}