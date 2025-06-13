package com.example.safekernel;

import android.content.Context;

public class Api {
    public  static String getApkInfo(Context context){
        return SafeHelper.getApkInfo(context);
    }
}
