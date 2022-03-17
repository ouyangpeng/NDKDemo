#include <jni.h>
#include <jni.h>
#include <jni.h>
#include "libleaktracer/include/MemoryTrace.hpp"

//
// Created by OuyangPeng on 2022/3/17.
//
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_startMonitoringAllThreads(JNIEnv *env, jobject clazz) {
    leaktracer::MemoryTrace::GetInstance().startMonitoringAllThreads();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_startMonitoringThisThread(JNIEnv *env, jobject clazz) {
    leaktracer::MemoryTrace::GetInstance().startMonitoringThisThread();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_stopMonitoringAllocations(JNIEnv *env, jobject clazz) {
    leaktracer::MemoryTrace::GetInstance().stopMonitoringAllocations();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_stopAllMonitoring(JNIEnv *env, jobject clazz) {
    leaktracer::MemoryTrace::GetInstance().stopAllMonitoring();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_writeLeaksToFile(JNIEnv *env, jobject clazz, jstring filePath) {
    const char *path = env->GetStringUTFChars(filePath, JNI_FALSE);
    leaktracer::MemoryTrace::GetInstance().writeLeaksToFile(path);
    env->ReleaseStringUTFChars(filePath, path);
}