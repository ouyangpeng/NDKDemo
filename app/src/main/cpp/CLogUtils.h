//
// Created by OuyangPeng on 2021/10/27 0027.
//

#ifndef NDKDEMO_CLOGUTILS_H
#define NDKDEMO_CLOGUTILS_H

// 实现 C语言中打印log到android控制台
// 导入 头文件 android/log.h
#include <android/log.h>
#include <string.h>

// 文件名
#define __FILENAME__ (strrchr(__FILE__, '/') + 1)

#ifdef LOG_SWITCH_STATUS_ON
// 定义LOG 标签
#define TAG "NDK_JNI_LOG_TAG"
// 定义几个打印日志的方法
#define LOGV(format, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGI(format, ...) __android_log_print(ANDROID_LOG_INFO, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, TAG,\
        "[%s][%s][%d]: " format, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#else
#define LOGV(format, ...);
#define LOGD(format, ...);
#define LOGI(format, ...);
#define LOGW(format, ...);
#define LOGE(format, ...);
#endif


#endif //NDKDEMO_CLOGUTILS_H
