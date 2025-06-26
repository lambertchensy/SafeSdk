package com.example.ollvmmodule;

import com.example.ollvmmodule.util.Md5ExUtils;
import  com.qa.test.DebugApi;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.ollvmmodule.databinding.ActivityMainBinding;
import com.example.ollvmmodule.util.SignUtils;
import com.example.ollvmmodule.util.SignatureUtil;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(DebugApi.getDeviceInfo(this));
        String signMd5Base64 = SignUtils.getSignMd5Base64(this);
        Log.d("SignUtils", "MD5 (Base64): " + signMd5Base64);
        Log.d("SignUtils", "getSignatureStr: " + SignatureUtil.getSignatureStr(this));
        //魔改的Md5算法测试
        String testString = "59ef6c9f0c1efhskkfheeeaf166a8721c86";
        String md5Hash = Md5ExUtils.md5(testString);
        Log.d("Md5ExUtils", "MD5Ex hash of '" + testString + "': " + md5Hash);
    }
}