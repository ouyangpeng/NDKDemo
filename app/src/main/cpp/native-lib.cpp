#include <jni.h>
#include <string>

// 实现 C语言中打印log到android控制台
// 导入 头文件 android/log.h
#include <android/log.h>
// 定义一些宏
#define DEBUG 1 //日志开关，1为开，其它为关
#if(DEBUG==1)
// 定义LOG 标签
#define LOG_TAG "JNI_LOG_TAG"
// 定义几个打印日志的方法
// log打印的方法中不能传入jstring，必须将jstring转为c中的字符数组在传入到方法里面打印，否则会报错。
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , LOG_TAG, __VA_ARGS__)
#else
#define LOGV(...) NULL
#define LOGD(...) NULL
#define LOGI(...) NULL
#define LOGW(...) NULL
#define LOGE(...) NULL
#endif

/*C字符串转JNI字符串*/
jstring stringToJstring(JNIEnv *env, const char *pat) {
    LOGD("C字符串转JNI字符串");
    jclass strClass = env->FindClass("Ljava/lang/String;");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    jstring encoding = env->NewStringUTF("utf-8");
    jstring rstStr = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
    return rstStr;
}

/*JNI字符串转C字符串*/
char *jstringTostring(JNIEnv *env, jstring jstr) {
    LOGD("调用JNI字符串转C字符串的方法");
    // 字符指针，也就是C的字符串
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    // 实际上就是调用了 java层的 String.getBytes 方法 , 得到字节数组
    // jstr 需要转换的java字符串
    // mid String类里面的getBytes 方法，参数是string
    // strencode java字符串，值为 "utf-8"
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    // 得到字节数组的长度
    jsize alen = env->GetArrayLength(barr);
    // 得到字节数组的首地址
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        // 申请堆内存，存放字节数组的所有数据，最后还要加一个结束符 \0
        rtn = (char *) malloc(alen + 1); // 为啥要加1 因为 需要有结尾 "\0"
        // 内存数据拷贝: 把字节数组的所有元素 拷贝 到 rtn所指的堆内存中
        memcpy(rtn, ba, alen);
        // 把最后一个元素 置为0 结束符
        rtn[alen] = 0;
    }
    // 释放 原来的字节数组
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

/*从C层返回一个字符串*/
extern "C" JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    LOGD("调用C层返回字符串的方法");
    std::string hello = "Hello from C++ , OuyangPeng's Blog : https://blog.csdn.net/ouyang_peng/";
    return env->NewStringUTF(hello.c_str());
}

/*从C层返回一个加密后的字符串*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_encode(JNIEnv *env, jobject thiz, jstring pass, jint length) {
    LOGD("调用加密方法");
    // 将java字符串转换成C字符串
    char *cstr = jstringTostring(env, pass);
    LOGD("加密前的字符串为:【%s】",cstr);
    // 把所有元素取出来，+1
    int i;
    for (i = 0; i < length; i++) {
        *(cstr + i) += 1;
    }
    LOGD("加密后的字符串为:【%s】",cstr);
    return  env->NewStringUTF(cstr);
}

/*从C层返回一个解密后的字符串*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_decode(JNIEnv *env, jobject thiz, jstring pass, jint length) {
    LOGD("调用解密方法");
    // 将java字符串转换成C字符串
    char *cstr = jstringTostring(env, pass);
    LOGD("加密前的字符串为:【%s】",cstr);
    // 把所有元素取出来，+1
    int i;
    for (i = 0; i < length; i++) {
        *(cstr + i) -= 1;
    }
    LOGD("加密前的字符串为:【%s】",cstr);
    return  env->NewStringUTF(cstr);
}

int com(const void *a, const void *b) {
    return *(int *) a - *(int *) b;//升序
}

extern "C"
JNIEXPORT jintArray  JNICALL
Java_com_oyp_ndkdemo_JNI_transmit(JNIEnv *env, jobject thiz, jintArray intArray) {
    LOGD("调用数组操作的方法");
    jint *receivedint = env->GetIntArrayElements(intArray, JNI_FALSE);
    // 拿到数组长度
    jsize size = env->GetArrayLength(intArray);

    // 把数组的每个数字 都*2
    for (int i = 0; i < size; i++) {
        // int value = (int) (*(receivedint + i) * 2);
        int value = receivedint[i] * 2;
        env->SetIntArrayRegion(intArray, i, 1, &value);
    }
    return intArray;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_oyp_ndkdemo_JNI_intArraySum(JNIEnv *env, jobject thiz, jintArray intArray) {
    LOGD("调用数组求和的方法");
    jint *receivedint = env->GetIntArrayElements(intArray, JNI_FALSE);
    if(receivedint == NULL){
        return 0;
    }
    // 拿到数组长度
    jsize size = env->GetArrayLength(intArray);
    int sum = 0;
    for (int i = 0; i < size; i++) {
        //sum += *(receivedint + i);
        sum += receivedint[i];
    }
    return sum;
}