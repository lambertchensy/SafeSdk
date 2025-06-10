# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("ollvmmodule")

# 将 ollvm 的参数添加到构建命令中。(经测试，fla 与 bcf 同时启用会闪退，原因未知)
# cppFlags '-mllvm -sobf'
# cppFlags '-mllvm -sub -mllvm -sub_loop=3'
# cppFlags '-mllvm -fla -mllvm -split -mllvm -split_num=2' // 只有-split_num=3会奔溃  （混淆）TODO ZTG  配置 3、4运行会不会崩溃
# cppFlags "-fvisibility=hidden -Wl,--exclude-libs,ALL"
set(OLLVM "-mllvm -sub -mllvm -fla -mllvm -sobf -mllvm -split -mllvm -bcf")
#add_definitions(${OLLVM})

# 启用 C 和汇编语言的支持
enable_language(C ASM)
# 根据系统处理器架构选择不同的汇编文件
if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")  # 检查当前系统是否为 AArch64 (ARM 64-bit) 架构
    # 为 `syscall64.s` 设置编译标志
    # `-x assembler-with-cpp` 表示使用 C 预处理器来编译汇编文件
    set_source_files_properties(syscall64.s PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
    SET(SYSCALL_FILE_NAME "syscall64.s")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
    # 为 `syscall32.s` 设置编译标志
    # `-x assembler-with-cpp` 表示使用 C 预处理器来编译汇编文件
    set_source_files_properties(syscall32.s PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
    SET(SYSCALL_FILE_NAME "syscall32.s")
endif ()

message("-- SYSCALL_FILE_NAME=${SYSCALL_FILE_NAME}")

# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        ${SYSCALL_FILE_NAME}   #添加 ARM 32(ARM64) 位汇编源文件
        sign/CheckSign.cpp
)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log)