package com.example.ollvmmodule;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.ollvmmodule.databinding.ActivityMainBinding;
import com.example.ollvmmodule.util.SignUtils;
import com.example.ollvmmodule.util.SignatureUtil;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'ollvmmodule' library on application startup.
    static {
        System.loadLibrary("ollvmmodule");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        checkApkSign();
        String signMd5Base64 = SignUtils.getSignMd5Base64(this);
        Log.d("SignUtils", "MD5 (Base64): " + signMd5Base64);
        Log.d("SignUtils", "getSignatureStr: " + SignatureUtil.getSignatureStr(this));
    }

    /**
     * A native method that is implemented by the 'ollvmmodule' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String checkApkSign();
}