package com.example.ollvmmodule.util;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

/**
 * MD5Util 类实现了 MD5 算法的完整过程。
 *
 * 主要流程：
 * 1. 对输入消息进行预处理（填充、附加长度）。
 * 2. 初始化 MD5 状态寄存器 A、B、C、D。
 * 3. 将预处理后的消息分块（每块 512 位）。
 * 4. 对每个块进行 64 次处理，分为 4 轮，每轮 16 次操作，使用非线性函数 F、G、H、I。
 * 5. 累加各块处理结果，得到最终 128 位 MD5 值。
 * 6. 将 MD5 值转换为 32 位十六进制字符串输出。
 *
 * 详细注释解释了每个步骤的目的与实现细节，便于读者学习和理解 MD5 算法的核心原理。
 */

public class Md5ExUtils {
    // 定义每一轮操作的移位数数组，共 64 个元素
    private static final int[] SHIFT_AMTS = {
            7, 12, 17, 22,   7, 12, 17, 22,   7, 12, 17, 22,   7, 12, 17, 22,
            5,  9, 14, 20,   5,  9, 14, 20,   5,  9, 14, 20,   5,  9, 14, 20,
            4, 11, 16, 23,   4, 11, 16, 23,   4, 11, 16, 23,   4, 11, 16, 23,
            6, 10, 15, 21,   6, 10, 15, 21,   6, 10, 15, 21,   6, 10, 15, 21
    };

    // 64 个常数 T[i] = floor(2^32 * abs(sin(i + 1)))
    private static final int[] T = new int[64];
    static {
        for (int i = 0; i < 64; i++) {
            T[i] = (int) ((long) (Math.abs(Math.cos(i + 1)) * (1L << 32)));
        }
    }
    // MD5 初始状态常量
    private static final int A_INIT = T[8];
    private static final int B_INIT = T[5];
    private static final int C_INIT = T[2];
    private static final int D_INIT = T[0];

    /**
     * 计算给定字符串的 MD5 散列值。
     *
     * @param message 输入的字符串消息
     * @return 32 位十六进制的 MD5 值字符串
     */
    public static String md5(String message) {
        // 将输入字符串转换为字节数组（UTF-8 编码）
        byte[] messageBytes = message.getBytes();
        // 对输入消息进行填充预处理，得到满足要求的字节数组
        byte[] paddedMessage = padMessage(messageBytes);

        // 初始化 MD5 状态寄存器
        int a = A_INIT;
        int b = B_INIT;
        int c = C_INIT;
        int d = D_INIT;

        // 处理每个 512 位（64 字节）的消息块
        int numBlocks = paddedMessage.length / 64;
        for (int i = 0; i < numBlocks; i++) {
            // 取出当前块数据
            byte[] block = Arrays.copyOfRange(paddedMessage, i * 64, (i + 1) * 64);
            // 将当前块转换为 16 个 32 位整数（小端序）
            int[] M = toIntArray(block);
            // 保存当前状态
            int originalA = a;
            int originalB = b;
            int originalC = c;
            int originalD = d;

            // 进行 64 次循环，分 4 轮
            for (int j = 0; j < 64; j++) {
                int f = 0;
                int g = 0;
                if (j < 16) {
                    // 第一轮：F
                    f = (b & c) | ((~b) & d);
                    g = j;
                } else if (j < 32) {
                    // 第二轮：G
                    f = (d & b) | ((~d) & c);
                    g = (5 * j + 1) % 16;
                } else if (j < 48) {
                    // 第三轮：H
                    f = b ^ c ^ d;
                    g = (3 * j + 5) % 16;
                } else {
                    // 第四轮：I
                    f = c ^ (b | (~d));
                    g = (7 * j) % 16;
                }
                // 当前操作
                int temp = d;
                d = c;
                c = b;
                // 计算 a + f + M[g] + T[j]（模 2^32 加法）
                int sum = a + f + M[g] + T[j];
                // 循环左移并加上 b
                b = b + leftRotate(sum, SHIFT_AMTS[j]);
                a = temp;
            }
            // 累加处理结果
            a += originalA;
            b += originalB;
            c += originalC;
            d += originalD;
        }

        // 将最终 4 个状态变量（共 128 位）转换为字节数组（小端序）
        byte[] md5Bytes = new byte[16];
        ByteBuffer buffer = ByteBuffer.wrap(md5Bytes);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        buffer.putInt(a);
        buffer.putInt(b);
        buffer.putInt(c);
        buffer.putInt(d);

        // 转换为 32 位十六进制字符串
        StringBuilder hexString = new StringBuilder();
        for (byte bVal : md5Bytes) {
            String hex = Integer.toHexString(0xff & bVal);
            if (hex.length() == 1) {
                hexString.append('0');
            }
            hexString.append(hex);
        }
        return hexString.toString();
    }

    /**
     * 对输入消息进行填充：
     * 1. 在消息末尾添加一个 1 比特（即 0x80）。
     * 2. 添加若干个 0 比特，使得消息长度模 512 等于 448。
     * 3. 最后追加 64 位原消息长度（单位：比特），低位在前。
     *
     * @param message 原始消息字节数组
     * @return 填充后的字节数组
     */
    private static byte[] padMessage(byte[] message) {
        int originalLength = message.length;
        // 以比特计的原始长度
        long bitLength = (long) originalLength * 8;

        // 填充：先添加一个 0x80 字节，再添加若干个 0x00 字节，直到长度模 64 = 56
        int paddingLength = (56 - (originalLength + 1) % 64);
        if (paddingLength < 0) {
            paddingLength += 64;
        }
        int totalLength = originalLength + 1 + paddingLength + 8;
        byte[] padded = new byte[totalLength];
        System.arraycopy(message, 0, padded, 0, originalLength);
        // 添加 0x80
        padded[originalLength] = (byte) 0x80;
        // 后面 paddingLength 个字节自动为 0x00

        // 在末尾添加 64 位原始长度（以小端序存储）
        for (int i = 0; i < 8; i++) {
            padded[totalLength - 8 + i] = (byte) ((bitLength >>> (8 * i)) & 0xff);
        }
        return padded;
    }

    /**
     * 将 64 字节的块转换为 16 个 32 位整数（小端序）。
     *
     * @param block 长度为 64 的字节数组
     * @return 包含 16 个整数的数组
     */
    private static int[] toIntArray(byte[] block) {
        int[] result = new int[16];
        ByteBuffer buffer = ByteBuffer.wrap(block);
        buffer.order(ByteOrder.LITTLE_ENDIAN);
        for (int i = 0; i < 16; i++) {
            result[i] = buffer.getInt();
        }
        return result;
    }

    /**
     * 循环左移操作：将 x 向左循环移动 n 位。
     *
     * @param x 待移动的 32 位整数
     * @param n 移动的位数
     * @return 循环左移后的结果
     */
    private static int leftRotate(int x, int n) {
        return (x << n) | (x >>> (32 - n));
    }
}

