package com.example.safekernel;

import android.content.Context;

public class SafeHelper {
    // Used to load the 'safekernel' library on application startup.
    static {
        System.loadLibrary("safekernel");
    }

    /**
     * A native method that is implemented by the 'ollvmmodule' native library,
     * which is packaged with this application.
     */
    public native static String getApkInfo(Context context);
}
