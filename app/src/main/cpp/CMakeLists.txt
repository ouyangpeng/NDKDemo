# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

# 设置个宏定义开关
# 编译的时候可以  cmake -DPRINT_LOG=1   或者  cmake -DPRINT_LOG=0
option(PRINT_LOG  "Set to switch to print log"  OFF)
message("PRINT_LOG is ${PRINT_LOG}")
IF(PRINT_LOG)
    add_definitions(-DLOG_SWITCH_STATUS_ON)  # 宏定义 LOG_SWITCH_STATUS_ON
    message(STATUS "Print LOG")
ELSE()
    message(STATUS "Do not Print LOG")
ENDIF(PRINT_LOG)

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("ndkdemo")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             ndkdemo

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp

             # 日志打印工具
             CLogUtils.h
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       ndkdemo

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )