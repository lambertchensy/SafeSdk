package com.example.ollvmmodule;

import static com.example.safekernel.Api.checkApkSign;
import static com.example.safekernel.Api.stringFromJNI;

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
        tv.setText(stringFromJNI(this));
        checkApkSign(this);
        String signMd5Base64 = SignUtils.getSignMd5Base64(this);
        Log.d("SignUtils", "MD5 (Base64): " + signMd5Base64);
        Log.d("SignUtils", "getSignatureStr: " + SignatureUtil.getSignatureStr(this));
    }


}