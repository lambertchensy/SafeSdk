package com.example.safekernel;

import android.content.Context;

public class Api {
    public  static String stringFromJNI(Context context){
        return SafeHelper.stringFromJNI(context);
    }

    public  static String checkApkSign(Context context){
        return SafeHelper.checkApkSign(context);
    }
}
