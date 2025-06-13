package com.qa.test;

import android.content.Context;
import android.util.Log;

public class DebugApi {
    public  static String getDeviceInfo(Context context){
        String deviceInfo = DeviceHelper.getDeviceInfo(context);
        Log.d("test","test getDeviceInfo=\n"+deviceInfo);
        return deviceInfo;
    }
}
