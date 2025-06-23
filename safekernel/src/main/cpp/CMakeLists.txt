# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("safekernel")

#  1. 注意: 将 ollvm 的参数添加到构建命令中。(经测试，fla 与 bcf 同时启用会闪退，原因未知?)
#     -mllvm -fla -mllvm -bcf: fla和bcf同时启用会导致编译速度很慢且部分函数无法使用(OLLVM-14.0.0上已复现，编译速度很慢，基本卡死)
#  2. 配置2： DreamSoule在git上写的功能全开的配置，DreamSoule OLLVM-17.0.2，同样在bcf不开的情况下效果要比下面的配置3效果更好。而如果全开的话，效果可以达到最好，且编译出的so库最大(编译速度会稍微慢一点)
#            配置2包含配置3的所有选项，且选项中的参数设置比默认值更高，而且增加了额外选项(1.间接调用 (call 寄存器) icall  2. 间接分支 ibr  3. 间接全局变量 igv)
#     -mllvm -fla -mllvm -bcf -mllvm -bcf_prob=80 -mllvm -bcf_loop=3 -mllvm -sobf -mllvm -icall -mllvm -ibr -mllvm -igv -mllvm -sub -mllvm -sub_loop=3 -mllvm -split -mllvm -split_num=5
#  3. 配置3: buffcow的另一个的全开配置。(DreamSoule OLLVM-17.0.2效果要比OLLVM-14.0.0的效果好很多 )
#    -mllvm -sub -mllvm -sobf -mllvm -split -mllvm -fla -mllvm -bcf
# ----------------------------------------------------------------------------------------------------------------------
# cppFlags '-mllvm -sobf'
# cppFlags '-mllvm -sub -mllvm -sub_loop=3'
# cppFlags '-mllvm -fla -mllvm -split -mllvm -split_num=2' // 只有-split_num=3会奔溃  （混淆）TODO ZTG  配置 3、4运行会不会崩溃
# cppFlags "-fvisibility=hidden -Wl,--exclude-libs,ALL"
# -------------------------------------------------------------------------------------------
# annotate使用说明: 可传入上面的混淆开关名使此函数强制开启对应混淆功能,参考: https://github.com/DreamSoule/ollvm17?tab=readme-ov-file

set(OLLVM_OPTIONS "-mllvm -fla -mllvm -bcf -mllvm -bcf_prob=80 -mllvm -bcf_loop=3 -mllvm -sobf -mllvm -icall -mllvm -ibr -mllvm -igv -mllvm -sub -mllvm -sub_loop=3 -mllvm -split -mllvm -split_num=5")
#add_definitions(${OLLVM_OPTIONS})

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
#SET(LIB_NAME "safekernel")
#message("-- lib_name=${LIB_NAME}")

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

# 设置 jsoncpp 源码目录
set(JSONCPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp)
# 包含头文件目录
include_directories(${JSONCPP_SOURCE_DIR}/include)
# 添加 jsoncpp 库
add_library( jsoncpp SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${JSONCPP_SOURCE_DIR}/src/lib_json/json_reader.cpp
        ${JSONCPP_SOURCE_DIR}/src/lib_json/json_value.cpp
        ${JSONCPP_SOURCE_DIR}/src/lib_json/json_writer.cpp
)
target_link_libraries(jsoncpp  #
        # List libraries link to the target library
        core  #链接 kernel 库
)

#  添加需要ollvm保护的静态库core
add_library(core STATIC
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        # 添加到jsoncpp库的反外挂SDK代码
        SafeKernel.cpp
        ${SYSCALL_FILE_NAME}   #添加 ARM 32(ARM64) 位汇编源文件
        raw_syscall.cpp
        sign/CheckSign.cpp
        utils/Base64Utils.cpp
        utils/MD5Utils.cpp
        # aes加密库
        aes/aes.c
        aes/hex_utils.c
        aes/aes_utils.c
)
#target_compile_options(core PRIVATE ${OLLVM_OPTIONS}) # 将 OLLVM 选项应用于该库
target_link_libraries(core #
        # List libraries link to the target library
        log)


# 添加自身库jnitest
add_library(
        jnitest SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        main.cpp
        utils/ApkUtils.cpp  #编译到jsoncpp模块的话也能引用到，只不过需要声明为导出函数，且通过导入表符号引用
)

target_link_libraries(jnitest #
        # List libraries link to the target library
        jsoncpp
        android
        log)