package com.qa.test;

import android.content.Context;

public class DeviceHelper {
    // Used to load the 'safekernel' library on application startup.
    static {
        System.loadLibrary("jnitest");
    }

    /**
     * A native method that is implemented by the 'ollvmmodule' native library,
     * which is packaged with this application.
     */
    public native static String getDeviceInfo(Context context);
}
