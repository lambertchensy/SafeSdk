package com.example.ollvmmodule.util;

import static android.content.pm.PackageManager.GET_SIGNATURES;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;

import java.security.MessageDigest;

public class SignatureUtil {

    public static String getSignatureStr(Context context) {
        Signature signature = getSignature(context);
        byte[] certBytes = signature.toByteArray();
        try {
            MessageDigest md5 = MessageDigest.getInstance("MD5");
            MessageDigest sha1 = MessageDigest.getInstance("SHA1");
            MessageDigest sha256 = MessageDigest.getInstance("SHA256");
            byte[] md5Key = md5.digest(certBytes);
            byte[] sha1Key = sha1.digest(certBytes);
            byte[] sha256Key = sha256.digest(certBytes);
            return String.format("MD5: %s\n\nSHA1: %s\n\nSHA-256: %s\n\ncert:%s",
                    byteArrayToString(md5Key),
                    byteArrayToString(sha1Key),
                    byteArrayToString(sha256Key),
                    byteArrayToString(certBytes)
            );
        } catch (Exception e) {
            return "";
        }
    }

    public static Signature getSignature(Context argContext) {
        Signature signature = null;
        try {
            String packageName = argContext.getPackageName();
            PackageManager packageManager = argContext.getPackageManager();
            PackageInfo packageInfo = packageManager.getPackageInfo(packageName, GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            signature = signatures[0];
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return signature;
    }

    private static String byteArrayToString(byte[] array) {
        StringBuilder hexString = new StringBuilder();
        for (int i = 0; i < array.length; i++) {
            String appendString = Integer.toHexString(0xFF & array[i]).toUpperCase();
            if (appendString.length() == 1)
                hexString.append("0");
            hexString.append(appendString);
            if(i < array.length - 1)
                hexString.append(":");
        }
        return hexString.toString();
    }
}