plugins {
    alias(libs.plugins.android.library)
}

android {
    namespace = "com.example.safekernel"
    compileSdk = 35

    defaultConfig {
        minSdk = 24
        consumerProguardFiles("consumer-rules.pro")
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
    /**********************************
     ******所有与ndk编译有关配置
     **********************************/
    //ndkVersion = "24.0.8215888"  //指定ndk版本，配合OLLVM-14.0.0(自带的clang版本14.0.1,替换后具有ollvm混淆的clang版本是14.0.0)
    ndkVersion = "26.0.10792818"   // 指定ndk版本，配合DreamSoule的OLLVM-17.0.2 (自带的 clang version 17.0.2)
    //ndkVersion = "27.2.12479018"  //指定ndk版本，配合buffcow的OLLVM-18.0.3
    //ndkVersion = "28.0.13004108"  //指定ndk版本，配合buffcow的OLLVM-19.0.0
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
    defaultConfig {
        ndk {
            // 可选的 ABI 架构列表: arm64-v8a, armeabi-v7a, x86, x86_64
            abiFilters.addAll(listOf("armeabi-v7a","arm64-v8a"))  // 64位ARM
        }
    }

}

dependencies {

    implementation(libs.appcompat)
    implementation(libs.material)

}