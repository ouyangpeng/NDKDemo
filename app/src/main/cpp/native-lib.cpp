#include <jni.h>
#include <string>


/*C字符串转JNI字符串*/
jstring stoJstring(JNIEnv *env, const char *pat) {
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
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

/*从C层返回一个字符串*/
extern "C" JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++ , OuyangPeng's Blog : https://blog.csdn.net/ouyang_peng/";
    return env->NewStringUTF(hello.c_str());
}

/*从C层返回一个加密后的字符串*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_encode(JNIEnv *env, jobject thiz, jstring pass, jint length) {
    // 将java字符串转换成C字符串
    char *cstr = jstringTostring(env, pass);
    // 把所有元素取出来，+1
    int i;
    for (i = 0; i < length; i++) {
        *(cstr + i) += 1;
    }
    return env->NewStringUTF(cstr);
}

/*从C层返回一个解密后的字符串*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_decode(JNIEnv *env, jobject thiz, jstring pass, jint length) {
    char *cstr = jstringTostring(env, pass);
    // 把所有元素取出来，+1
    int i;
    for (i = 0; i < length; i++) {
        *(cstr + i) -= 1;
    }
    return env->NewStringUTF(cstr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_transmit(JNIEnv *env, jobject thiz, jintArray intarray) {
    // 给数组的每个数字加10

    // 拿到数组首地址
    int *p = env->GetIntArrayElements(intarray,JNI_FALSE);
    // 拿到数组长度
    jsize size = env-> GetArrayLength(intarray);
    int i;
    for (i = 0; i < size; i++) {
        *(p + i) += 10;
    }
}
