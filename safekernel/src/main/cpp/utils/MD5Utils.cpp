//
// Created by wengzulan on 2025/6/11.
//

#include "MD5Utils.h"
#include <sstream>
#include <iomanip>

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

// Apply the MD5 transformations for each round.
void MD5Utils::MD5Transform(uint32_t state[4], const unsigned char block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    // Decode the block into x[i].
    for (int i = 0; i < 16; i++) {
        x[i] = (block[i * 4]) | (block[i * 4 + 1] << 8) | (block[i * 4 + 2] << 16) | (block[i * 4 + 3] << 24);
    }

    // Round 1
    a = b + rotate_left(a + F(b, c, d) + x[0] + 0xd76aa478, 7);
    d = a + rotate_left(d + F(a, b, c) + x[1] + 0xe8c7b756, 12);
    c = d + rotate_left(c + F(d, a, b) + x[2] + 0x242070db, 17);
    b = c + rotate_left(b + F(c, d, a) + x[3] + 0xc1bdceee, 22);
    a = b + rotate_left(a + F(b, c, d) + x[4] + 0xf57c0faf, 7);
    d = a + rotate_left(d + F(a, b, c) + x[5] + 0x4787c62a, 12);
    c = d + rotate_left(c + F(d, a, b) + x[6] + 0xa8304613, 17);
    b = c + rotate_left(b + F(c, d, a) + x[7] + 0xfd469501, 22);
    a = b + rotate_left(a + F(b, c, d) + x[8] + 0x698098d8, 7);
    d = a + rotate_left(d + F(a, b, c) + x[9] + 0x8b44f7af, 12);
    c = d + rotate_left(c + F(d, a, b) + x[10] + 0xffff5bb1, 17);
    b = c + rotate_left(b + F(c, d, a) + x[11] + 0x895cd7be, 22);
    a = b + rotate_left(a + F(b, c, d) + x[12] + 0x6b901122, 7);
    d = a + rotate_left(d + F(a, b, c) + x[13] + 0xfd987193, 12);
    c = d + rotate_left(c + F(d, a, b) + x[14] + 0xa679438e, 17);
    b = c + rotate_left(b + F(c, d, a) + x[15] + 0x49b40821, 22);

    // Round 2
    a = b + rotate_left(a + G(b, c, d) + x[1] + 0xf61e2562, 5);
    d = a + rotate_left(d + G(a, b, c) + x[6] + 0xc040b340, 9);
    c = d + rotate_left(c + G(d, a, b) + x[11] + 0x265e5a51, 14);
    b = c + rotate_left(b + G(c, d, a) + x[0] + 0xe9b6c7aa, 20);
    a = b + rotate_left(a + G(b, c, d) + x[5] + 0xd62f105d, 5);
    d = a + rotate_left(d + G(a, b, c) + x[10] + 0x02441453, 9);
    c = d + rotate_left(c + G(d, a, b) + x[15] + 0xd8a1e681, 14);
    b = c + rotate_left(b + G(c, d, a) + x[4] + 0xe7d3fbc8, 20);
    a = b + rotate_left(a + G(b, c, d) + x[9] + 0x21e1cde6, 5);
    d = a + rotate_left(d + G(a, b, c) + x[14] + 0xc33707d6, 9);
    c = d + rotate_left(c + G(d, a, b) + x[3] + 0xf4d50d87, 14);
    b = c + rotate_left(b + G(c, d, a) + x[8] + 0x455a14ed, 20);
    a = b + rotate_left(a + G(b, c, d) + x[13] + 0xa9e3e905, 5);
    d = a + rotate_left(d + G(a, b, c) + x[2] + 0xfcefa3f8, 9);
    c = d + rotate_left(c + G(d, a, b) + x[7] + 0x676f02d9, 14);
    b = c + rotate_left(b + G(c, d, a) + x[12] + 0x8d2a4c8a, 20);

    // Round 3
    a = b + rotate_left(a + H(b, c, d) + x[5] + 0xfffa3942, 4);
    d = a + rotate_left(d + H(a, b, c) + x[8] + 0x8771f681, 11);
    c = d + rotate_left(c + H(d, a, b) + x[11] + 0x6d9d6122, 16);
    b = c + rotate_left(b + H(c, d, a) + x[14] + 0xfde5380c, 23);
    a = b + rotate_left(a + H(b, c, d) + x[1] + 0xa4beea44, 4);
    d = a + rotate_left(d + H(a, b, c) + x[4] + 0x4bdecfa9, 11);
    c = d + rotate_left(c + H(d, a, b) + x[7] + 0xf6bb4b60, 16);
    b = c + rotate_left(b + H(c, d, a) + x[10] + 0xbebfbc70, 23);
    a = b + rotate_left(a + H(b, c, d) + x[13] + 0x289b7ec6, 4);
    d = a + rotate_left(d + H(a, b, c) + x[0] + 0xeaa127fa, 11);
    c = d + rotate_left(c + H(d, a, b) + x[3] + 0xd4ef3085, 16);
    b = c + rotate_left(b + H(c, d, a) + x[6] + 0x04881d05, 23);
    a = b + rotate_left(a + H(b, c, d) + x[9] + 0xd9d4d039, 4);
    d = a + rotate_left(d + H(a, b, c) + x[12] + 0xe6db99e5, 11);
    c = d + rotate_left(c + H(d, a, b) + x[15] + 0x1fa27cf8, 16);
    b = c + rotate_left(b + H(c, d, a) + x[2] + 0xc4ac5665, 23);

    // Round 4
    a = b + rotate_left(a + I(b, c, d) + x[0] + 0xf4292244, 6);
    d = a + rotate_left(d + I(a, b, c) + x[7] + 0x432aff97, 10);
    c = d + rotate_left(c + I(d, a, b) + x[14] + 0xab9423a7, 15);
    b = c + rotate_left(b + I(c, d, a) + x[5] + 0xfc93a039, 21);
    a = b + rotate_left(a + I(b, c, d) + x[12] + 0x655b59c3, 6);
    d = a + rotate_left(d + I(a, b, c) + x[3] + 0x8f0ccc92, 10);
    c = d + rotate_left(c + I(d, a, b) + x[10] + 0xffeff47d, 15);
    b = c + rotate_left(b + I(c, d, a) + x[1] + 0x85845dd1, 21);
    a = b + rotate_left(a + I(b, c, d) + x[8] + 0x6fa87e4f, 6);
    d = a + rotate_left(d + I(a, b, c) + x[15] + 0xfe2ce6e0, 10);
    c = d + rotate_left(c + I(d, a, b) + x[6] + 0xa3014314, 15);
    b = c + rotate_left(b + I(c, d, a) + x[13] + 0x4e0811a1, 21);
    a = b + rotate_left(a + I(b, c, d) + x[4] + 0xf7537e82, 6);
    d = a + rotate_left(d + I(a, b, c) + x[11] + 0xbd3af235, 10);
    c = d + rotate_left(c + I(d, a, b) + x[2] + 0x2ad7d2bb, 15);
    b = c + rotate_left(b + I(c, d, a) + x[9] + 0xeb86d391, 21);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

std::string MD5Utils::md5(const std::string& input) {
    uint32_t state[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
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