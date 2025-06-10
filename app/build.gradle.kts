plugins {
    alias(libs.plugins.android.application)
}

android {
    namespace = "com.example.ollvmmodule"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.example.ollvmmodule"
        minSdk = 24
        targetSdk = 35
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"

        ndk{
            // 可选的 ABI 架构列表:
            // arm64-v8a, armeabi-v7a, x86, x86_64
            abiFilters.add("arm64-v8a")  // 64位ARM
            abiFilters.add("armeabi-v7a") // 32位ARM
            // abiFilters.add("x86")       // x86 (模拟器)
            // abiFilters.add("x86_64")    // x86_64 (模拟器)
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    //ndkVersion = "24.0.8215888"  //指定ndk版本，配合OLLVM-14.0.0(自带的clang版本14.0.1,替换后具有ollvm混淆的clang版本是14.0.0)
    ndkVersion = "26.0.10792818"   // 指定ndk版本，配合DreamSoule的OLLVM-17.0.2 (自带的 clang version 17.0.2)
    //ndkVersion = "27.2.12479018"  //指定ndk版本，配合buffcow的OLLVM-18.0.3
    //ndkVersion = "28.0.13004108"  //指定ndk版本，配合buffcow的OLLVM-19.0.0

    buildFeatures {
        viewBinding = true
    }
}

dependencies {

    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.constraintlayout)
    testImplementation(libs.junit)
    androidTestImplementation(libs.ext.junit)
    androidTestImplementation(libs.espresso.core)
}