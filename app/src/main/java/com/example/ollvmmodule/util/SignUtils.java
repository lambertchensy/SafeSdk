package com.example.ollvmmodule.util;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Base64;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class SignUtils {

    /**
     * 获取当前应用的签名 MD5（Base64 编码）
     */
    public static String getSignMd5Base64(Context context) {
        try {
            // 获取 PackageInfo
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(
                    context.getPackageName(),
                    PackageManager.GET_SIGNATURES
            );

            // 获取签名数组（一般第一个签名就是应用的签名）
            Signature[] signatures = packageInfo.signatures;
            if (signatures == null || signatures.length == 0) {
                return null;
            }

            // 计算 MD5
            byte[] certBytes = signatures[0].toByteArray();
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] md5Bytes = md.digest(certBytes);

            // 返回 Base64 编码后的 MD5
            return Base64.encodeToString(md5Bytes, Base64.NO_WRAP);

        } catch (PackageManager.NameNotFoundException | NoSuchAlgorithmException e) {
            e.printStackTrace();
            return null;
        }
    }
}