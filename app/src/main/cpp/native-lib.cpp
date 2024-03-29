#include <jni.h>
#include <string>
#include<vector>
// 导入定义的日志打印工具
#include "CLogUtils.h"

//// 实现 C语言中打印log到android控制台
//// 导入 头文件 android/log.h
//#include <android/log.h>
//// 定义一些宏
//#define DEBUG 1 //日志开关，1为开，其它为关
//#if(DEBUG==1)
//// 定义LOG 标签
//#define LOG_TAG "JNI_LOG_TAG"
//// 定义几个打印日志的方法
//// log打印的方法中不能传入jstring，必须将jstring转为c中的字符数组在传入到方法里面打印，否则会报错。
//#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , LOG_TAG, __VA_ARGS__)
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , LOG_TAG, __VA_ARGS__)
//#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , LOG_TAG, __VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , LOG_TAG, __VA_ARGS__)
//#else
//#define LOGV(...) NULL
//#define LOGD(...) NULL
//#define LOGI(...) NULL
//#define LOGW(...) NULL
//#define LOGE(...) NULL
//#endif

/*C字符串转JNI字符串*/
jstring stringToJstring(JNIEnv *env, const char *pat) {
//    LOGD("C字符串转JNI字符串");
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    jstring encoding = env->NewStringUTF("utf-8");
    jstring rstStr = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
    return rstStr;
}

/*JNI字符串转C字符串*/
char *jstringTostring(JNIEnv *env, jstring jstr) {
    LOGD("调用JNI字符串转C字符串的方法")
    // 字符指针，也就是C的字符串
    char *rtn = nullptr;
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
    LOGD("调用C层返回字符串的方法")
    std::string hello = "Hello from C++ , OuyangPeng's Blog : https://blog.csdn.net/ouyang_peng/";
    return env->NewStringUTF(hello.c_str());
}

/*从C层返回一个加密后的字符串*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_encode(JNIEnv *env, jobject thiz, jstring pass, jint length) {
    LOGD("调用加密方法")
    // 将java字符串转换成C字符串
    char *cstr = jstringTostring(env, pass);
    LOGD("加密前的字符串为:【%s】", cstr)
    // 把所有元素取出来，+1
    int i;
    for (i = 0; i < length; i++) {
        *(cstr + i) += 1;
    }
    LOGD("加密后的字符串为:【%s】", cstr)
    jstring result = env->NewStringUTF(cstr);
    delete[] cstr;
    return result;
}

/*从C层返回一个解密后的字符串*/
extern "C"
JNIEXPORT jstring JNICALL
Java_com_oyp_ndkdemo_JNI_decode(JNIEnv *env, jobject thiz, jstring pass, jint length) {
    LOGD("调用解密方法")
    // 将java字符串转换成C字符串
    char *cstr = jstringTostring(env, pass);
    LOGD("加密前的字符串为:【%s】", cstr)
    // 把所有元素取出来，+1
    int i;
    for (i = 0; i < length; i++) {
        *(cstr + i) -= 1;
    }
    LOGD("加密前的字符串为:【%s】", cstr)
    jstring result = env->NewStringUTF(cstr);
    delete[] cstr;
    return result;
}

int com(const void *a, const void *b) {
    return *(int *) a - *(int *) b;//升序
}

extern "C"
JNIEXPORT jintArray  JNICALL
Java_com_oyp_ndkdemo_JNI_transmit(JNIEnv *env, jobject thiz, jintArray intArray) {
    LOGD("调用数组操作的方法")
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

void test(int *Data) {
    Data[0] = 1;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_testStackAndHeap(JNIEnv *env, jobject thiz) {
    // 申请一个超级大的数组

    // 1. 直接在栈空间申请大内存，会crash
    //    int sig_data[180000000] = {0};

    // 2. 通过new和delete关键字，这样的对象内存分配在堆内存中，不会crash
//    int* sig_data = new int[180000000];
//    test(sig_data);
//    delete[] sig_data;

    // 3.通过malloc和free关键字, 这样的对象内存分配在堆内存中，不会crash
    int *sig_data = (int *) malloc(180000000 * sizeof(int));
    test(sig_data);
    free(sig_data);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_oyp_ndkdemo_JNI_intArraySum(JNIEnv *env, jobject thiz, jintArray intArray) {
    LOGD("调用数组求和的方法")
    jint *receivedint = env->GetIntArrayElements(intArray, JNI_FALSE);
    if (receivedint == NULL) {
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

extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_testLog(JNIEnv *env, jobject thiz) {
    LOGD("Let's test JNI print Android Log")
    LOGD("Let's test JNI print Android Log,  test print one number: [%d]", 10)
    LOGD("Let's test JNI print Android Log,  test print a string:[%s] ,and one number : [%d]",
         "num", 20)
}


typedef struct {
    float x;
    float y;
} NativePointF;

struct NativeFaceFeatureBean {
    NativeFaceFeatureBean() {
        faceId = 0;
        yaw = 0;
        pitch = 0;
        roll = 0;
        boundingBox = nullptr;
        landmarks = nullptr;
        visibilities = nullptr;
    }

    ~NativeFaceFeatureBean() {
        if (boundingBox) {
            delete[] boundingBox;
            boundingBox = nullptr;
        }
        if (landmarks) {
            delete[] landmarks;
            landmarks = nullptr;
        }
        if (visibilities) {
            delete[] visibilities;
            visibilities = nullptr;
        }
    }

    int faceId;
    float yaw;
    float pitch;
    float roll;
    NativePointF *boundingBox;
    NativePointF *landmarks;
    float *visibilities;
};


void showNativeFaceFeatureBean(NativeFaceFeatureBean &faceFeatureBean, int boundingBoxLength,
                               int landmarksLength, int visibilitiesLength) {
    LOGD("showNativeFaceFeatureBean() faceFeatureBean中faceId = %d,yaw = %f,pitch = %f,roll = %f",
         faceFeatureBean.faceId,
         faceFeatureBean.yaw, faceFeatureBean.pitch, faceFeatureBean.roll)

    for (int i = 0; i < boundingBoxLength; i++) {
        LOGD("showNativeFaceFeatureBean() faceFeatureBean.boundingBoxArray数组中 第 %d 个值为：x = %f , y = %f",
             i + 1,
             faceFeatureBean.boundingBox[i].x, faceFeatureBean.boundingBox[i].y)
    }

    for (int i = 0; i < landmarksLength; i++) {
        LOGD("showNativeFaceFeatureBean() faceFeatureBean.landmarksArray 数组中 第 %d 个值为：x = %f , y = %f",
             i + 1,
             faceFeatureBean.landmarks[i].x, faceFeatureBean.landmarks[i].y)
    }

    for (int i = 0; i < visibilitiesLength; i++) {
        LOGD("showNativeFaceFeatureBean() faceFeatureBean.visibilities 数组中 第 %d 个值为： %f ", i + 1,
             faceFeatureBean.visibilities[i])
    }
}


/**
 *
字符 Java类型 C类型
V       void            void
Z       jboolean       boolean
I       jint              int
J       jlong            long
D      jdouble         double
F      jfloat            float
B      jbyte            byte
C      jchar           char
S      jshort          short

数组则以"["开始，用两个字符表示
[I       jintArray      int[]
[F     jfloatArray    float[]
[B     jbyteArray    byte[]
[C    jcharArray    char[]
[S    jshortArray   short[]
[D    jdoubleArray double[]
[J     jlongArray     long[]
[Z    jbooleanArray boolean[]

上面的都是基本类型。如果Java函数的参数是class，则以"L"开头，以";"结尾中间是用"/" 隔开的包及类名。
 而其对应的C函数名的参数则为jobject. 一个例外是String类，其对应的类为jstring

Ljava/lang/String; String jstring
Ljava/net/Socket; Socket jobject

如果JAVA函数位于一个嵌入类，则用$作为类名间的分隔符。
例如 "(Ljava/lang/String;Landroid/os/FileUtils$FileStatus;)Z"
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_nativeSetFaceFeatureBean(JNIEnv *env, jobject thiz, jobject feature) {
    LOGD("=================================Java_com_oyp_ndkdemo_JNI_nativeSetFaceFeatureBean=================================")
    // 获取FaceFeatureBean类
    jclass jFeature = env->GetObjectClass(feature);

    // 获取FaceFeatureBean对象的methodID
    jmethodID getFaceId = env->GetMethodID(jFeature, "getFaceId", "()I");
    jmethodID getYaw = env->GetMethodID(jFeature, "getYaw", "()F");
    jmethodID getPitch = env->GetMethodID(jFeature, "getPitch", "()F");
    jmethodID getRoll = env->GetMethodID(jFeature, "getRoll", "()F");

    // 执行方法 拿到属性
    jint faceId = env->CallIntMethod(feature, getFaceId);
    jfloat yaw = env->CallFloatMethod(feature, getYaw);
    jfloat pitch = env->CallFloatMethod(feature, getPitch);
    jfloat roll = env->CallFloatMethod(feature, getRoll);
//    LOGD("faceId = %d,yaw = %f,pitch = %f,roll = %f", faceId, yaw, pitch, roll)


    auto *faceFeatureBean = new NativeFaceFeatureBean();
    faceFeatureBean->faceId = faceId;
    faceFeatureBean->yaw = yaw;
    faceFeatureBean->pitch = pitch;
    faceFeatureBean->roll = roll;


    jmethodID getVisibilities = env->GetMethodID(jFeature, "getVisibilities", "()Ljava/util/List;");
    jobject visibilities = env->CallObjectMethod(feature, getVisibilities);

    // 遍历 visibilities，拿到List的各个Item
    // 获取ArrayList对象
    jclass jcs_alist = env->GetObjectClass(visibilities);
    // 获取ArrayList的methodid
    jmethodID alist_get = env->GetMethodID(jcs_alist, "get", "(I)Ljava/lang/Object;");
    jmethodID alist_size = env->GetMethodID(jcs_alist, "size", "()I");
    jint visibilitiesSize = env->CallIntMethod(visibilities, alist_size);

    faceFeatureBean->visibilities = new float[visibilitiesSize];

    for (int i = 0; i < visibilitiesSize; i++) {
        // 获取 Float 对象
        jobject float_obj = env->CallObjectMethod(visibilities, alist_get, i);
        // 获取 Float 类
        jclass float_cls = env->GetObjectClass(float_obj);
        jmethodID getFloatValue = env->GetMethodID(float_cls, "floatValue", "()F");
        jfloat floatValue = env->CallFloatMethod(float_obj, getFloatValue);
//        LOGD("visibilities列表中 第 %d 个值为：floatValue = %f", i + 1, floatValue)

        faceFeatureBean->visibilities[i] = floatValue;
    }


    jmethodID getBoundingBox = env->GetMethodID(jFeature, "getBoundingBox",
                                                "()[Landroid/graphics/PointF;");
    jobject boundingBox = env->CallObjectMethod(feature, getBoundingBox);
    auto *boundingBoxArray = reinterpret_cast<jobjectArray *>(&boundingBox);

    const jint boundingBoxSize = env->GetArrayLength(*boundingBoxArray);

    faceFeatureBean->boundingBox = new NativePointF[boundingBoxSize];

    for (int i = 0; i < boundingBoxSize; i++) {
        jobject point = env->GetObjectArrayElement(*boundingBoxArray, i);
        //1.获得实例对应的class类
        jclass jcls = env->GetObjectClass(point);
        //2.通过class类找到对应的field id
        //num 为java类中变量名，I 为变量的类型int
        jfieldID xID = env->GetFieldID(jcls, "x", "F");
        jfieldID yID = env->GetFieldID(jcls, "y", "F");

        jfloat x = env->GetFloatField(point, xID);
        jfloat y = env->GetFloatField(point, yID);
//        LOGD("boundingBoxArray数组中 第 %d 个值为：x = %f , y = %f", i + 1, x, y)

        faceFeatureBean->boundingBox[i].x = x;
        faceFeatureBean->boundingBox[i].y = y;
    }

    jmethodID getLandmarks = env->GetMethodID(jFeature, "getLandmarks",
                                              "()[Landroid/graphics/PointF;");
    jobject landmarks = env->CallObjectMethod(feature, getLandmarks);
    auto *landmarksArray = reinterpret_cast<jobjectArray *>(&landmarks);
    const jint landmarksSize = env->GetArrayLength(*landmarksArray);

    faceFeatureBean->landmarks = new NativePointF[landmarksSize];

    for (int i = 0; i < landmarksSize; i++) {
        jobject point = env->GetObjectArrayElement(*landmarksArray, i);
        //1.获得实例对应的class类
        jclass jcls = env->GetObjectClass(point);
        //2.通过class类找到对应的field id
        //num 为java类中变量名，I 为变量的类型int
        jfieldID xID = env->GetFieldID(jcls, "x", "F");
        jfieldID yID = env->GetFieldID(jcls, "y", "F");

        jfloat x = env->GetFloatField(point, xID);
        jfloat y = env->GetFloatField(point, yID);
//        LOGD("landmarksArray 第 %d 个值为：x = %f , y = %f", i + 1, x, y)
        faceFeatureBean->landmarks[i].x = x;
        faceFeatureBean->landmarks[i].y = y;
    }
    showNativeFaceFeatureBean(*faceFeatureBean, boundingBoxSize, landmarksSize, visibilitiesSize);

    delete faceFeatureBean;
}

/////////////////////////////////////// 第二种实现

class NativeFaceFeatureBeanClass {
public:
    NativeFaceFeatureBeanClass() {
        faceId = 0;
        yaw = 0;
        pitch = 0;
        roll = 0;
    }

    ~NativeFaceFeatureBeanClass() {
        if (!boundingBox.empty()) boundingBox.clear();
        if (!landmarks.empty()) landmarks.clear();
        if (!visibilities.empty()) visibilities.clear();
    }

    int faceId;
    float yaw;
    float pitch;
    float roll;
    std::vector<NativePointF> boundingBox;
    std::vector<NativePointF> landmarks;
    std::vector<float> visibilities;
};

void showNativeFaceFeatureBeanClass(NativeFaceFeatureBeanClass &faceFeatureBean) {
    LOGD("showNativeFaceFeatureBeanClass() faceFeatureBean中faceId = %d,yaw = %f,pitch = %f,roll = %f",
         faceFeatureBean.faceId,
         faceFeatureBean.yaw, faceFeatureBean.pitch, faceFeatureBean.roll)

    for (int i = 0; i < faceFeatureBean.boundingBox.size(); i++) {
        LOGD("showNativeFaceFeatureBeanClass() faceFeatureBean.boundingBoxArray数组中 第 %d 个值为：x = %f , y = %f",
             i + 1,
             faceFeatureBean.boundingBox[i].x, faceFeatureBean.boundingBox[i].y)
    }


    for (int i = 0; i < faceFeatureBean.landmarks.size(); i++) {
        LOGD("showNativeFaceFeatureBeanClass() faceFeatureBean.landmarksArray 数组中 第 %d 个值为：x = %f , y = %f",
             i + 1,
             faceFeatureBean.landmarks[i].x, faceFeatureBean.landmarks[i].y)
    }

    for (int i = 0; i < faceFeatureBean.visibilities.size(); i++) {
        LOGD("showNativeFaceFeatureBeanClass() faceFeatureBean.visibilities 数组中 第 %d 个值为： %f ",
             i + 1,
             faceFeatureBean.visibilities[i])
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_nativeSetFaceFeatureBean2(JNIEnv *env, jobject thiz, jobject feature) {
    LOGD("=================================Java_com_oyp_ndkdemo_JNI_nativeSetFaceFeatureBean=================================")
    // 获取FaceFeatureBean类
    jclass jFeature = env->GetObjectClass(feature);

    // 获取FaceFeatureBean对象的methodID
    jmethodID getFaceId = env->GetMethodID(jFeature, "getFaceId", "()I");
    jmethodID getYaw = env->GetMethodID(jFeature, "getYaw", "()F");
    jmethodID getPitch = env->GetMethodID(jFeature, "getPitch", "()F");
    jmethodID getRoll = env->GetMethodID(jFeature, "getRoll", "()F");

    // 执行方法 拿到属性
    jint faceId = env->CallIntMethod(feature, getFaceId);
    jfloat yaw = env->CallFloatMethod(feature, getYaw);
    jfloat pitch = env->CallFloatMethod(feature, getPitch);
    jfloat roll = env->CallFloatMethod(feature, getRoll);
//    LOGD("faceId = %d,yaw = %f,pitch = %f,roll = %f", faceId, yaw, pitch, roll)


    auto *faceFeatureBean = new NativeFaceFeatureBeanClass();
    faceFeatureBean->faceId = faceId;
    faceFeatureBean->yaw = yaw;
    faceFeatureBean->pitch = pitch;
    faceFeatureBean->roll = roll;


    jmethodID getVisibilities = env->GetMethodID(jFeature, "getVisibilities", "()Ljava/util/List;");
    jobject visibilities = env->CallObjectMethod(feature, getVisibilities);

    // 遍历 visibilities，拿到List的各个Item
    // 获取ArrayList对象
    jclass jcs_alist = env->GetObjectClass(visibilities);
    // 获取ArrayList的methodid
    jmethodID alist_get = env->GetMethodID(jcs_alist, "get", "(I)Ljava/lang/Object;");
    jmethodID alist_size = env->GetMethodID(jcs_alist, "size", "()I");
    jint visibilitiesSize = env->CallIntMethod(visibilities, alist_size);

    faceFeatureBean->visibilities = std::vector<float>(visibilitiesSize);
    for (int i = 0; i < visibilitiesSize; i++) {
        // 获取 Float 对象
        jobject float_obj = env->CallObjectMethod(visibilities, alist_get, i);
        // 获取 Float 类
        jclass float_cls = env->GetObjectClass(float_obj);
        jmethodID getFloatValue = env->GetMethodID(float_cls, "floatValue", "()F");
        jfloat floatValue = env->CallFloatMethod(float_obj, getFloatValue);
//        LOGD("visibilities列表中 第 %d 个值为：floatValue = %f", i + 1, floatValue)

        faceFeatureBean->visibilities[i] = floatValue;
    }

    jmethodID getBoundingBox = env->GetMethodID(jFeature, "getBoundingBox",
                                                "()[Landroid/graphics/PointF;");
    jobject boundingBox = env->CallObjectMethod(feature, getBoundingBox);
    auto *boundingBoxArray = reinterpret_cast<jobjectArray *>(&boundingBox);

    const jint boundingBoxSize = env->GetArrayLength(*boundingBoxArray);

    faceFeatureBean->boundingBox = std::vector<NativePointF>(boundingBoxSize);
    for (int i = 0; i < boundingBoxSize; i++) {
        jobject point = env->GetObjectArrayElement(*boundingBoxArray, i);
        //1.获得实例对应的class类
        jclass jcls = env->GetObjectClass(point);
        //2.通过class类找到对应的field id
        //num 为java类中变量名，I 为变量的类型int
        jfieldID xID = env->GetFieldID(jcls, "x", "F");
        jfieldID yID = env->GetFieldID(jcls, "y", "F");

        jfloat x = env->GetFloatField(point, xID);
        jfloat y = env->GetFloatField(point, yID);
//        LOGD("boundingBoxArray数组中 第 %d 个值为：x = %f , y = %f", i + 1, x, y)

        faceFeatureBean->boundingBox[i].x = x;
        faceFeatureBean->boundingBox[i].y = y;
    }

    jmethodID getLandmarks = env->GetMethodID(jFeature, "getLandmarks",
                                              "()[Landroid/graphics/PointF;");
    jobject landmarks = env->CallObjectMethod(feature, getLandmarks);
    auto *landmarksArray = reinterpret_cast<jobjectArray *>(&landmarks);
    const jint landmarksSize = env->GetArrayLength(*landmarksArray);

    faceFeatureBean->landmarks = std::vector<NativePointF>(landmarksSize);

    for (int i = 0; i < landmarksSize; i++) {
        jobject point = env->GetObjectArrayElement(*landmarksArray, i);
        //1.获得实例对应的class类
        jclass jcls = env->GetObjectClass(point);
        //2.通过class类找到对应的field id
        //num 为java类中变量名，I 为变量的类型int
        jfieldID xID = env->GetFieldID(jcls, "x", "F");
        jfieldID yID = env->GetFieldID(jcls, "y", "F");

        jfloat x = env->GetFloatField(point, xID);
        jfloat y = env->GetFloatField(point, yID);
//        LOGD("landmarksArray 第 %d 个值为：x = %f , y = %f", i + 1, x, y)
        faceFeatureBean->landmarks[i].x = x;
        faceFeatureBean->landmarks[i].y = y;
    }
    showNativeFaceFeatureBeanClass(*faceFeatureBean);

    delete faceFeatureBean;
}

struct Student {
    std::string id;
    std::string name;
    int age;
};

Student getStudent() {
    Student student;
    student.age = 18;
    student.id = "001";
    student.name = "OuyangPeng";
    return student;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_oyp_ndkdemo_JNI_getStudentFromJNI(JNIEnv *env, jobject thiz) {
    Student stu;
    // 调用算法 获取Student数据
    stu = getStudent();
    LOGD("student's id is %s", stu.id.c_str())
    LOGD("student's name is %s", stu.name.c_str())
    LOGD("student's age is %d", stu.age)
    // 1）获取java ReturnInfo对象的jclass；
    jclass StudentClass = env->FindClass("com/oyp/ndkdemo/Student");
    // 2）获取构造方法ID；
    jmethodID jmId = env->GetMethodID(StudentClass, "<init>", "()V");
    // 3）通过构造方法ID创建Java WashingHandResult对象；
    jobject StudentObject = env->NewObject(StudentClass, jmId);
    // 4）获取Student对象的字段ID；
    jfieldID idField = env->GetFieldID(StudentClass, "id", "Ljava/lang/String;");
    jfieldID nameField = env->GetFieldID(StudentClass, "name", "Ljava/lang/String;");
    jfieldID ageField = env->GetFieldID(StudentClass, "age", "I");
    // 5）通过字段ID给每个字段赋值
    jstring id = env->NewStringUTF(stu.id.c_str());
    env->SetObjectField(StudentObject, idField, id);
    jstring name = env->NewStringUTF(stu.name.c_str());
    env->SetObjectField(StudentObject, nameField, name);
    jint age = stu.age;
    env->SetIntField(StudentObject, ageField, age);
    // 6） 返回Java对象
    return StudentObject;
}

#define CHARSET_UTF8 "UTF-8"

char *jstringToChar(JNIEnv *env, jstring jstr) {
    char *returnCharArray = nullptr;
    jclass jclassName = env->FindClass("java/lang/String");
    jmethodID jmethodName = env->GetMethodID(jclassName, "getBytes", "(Ljava/lang/String;)[B");
    // 调用java.lang.String类的getBytes(String charsetName)方法，得到byte[]数组
    auto byteArray = (jbyteArray) env->CallObjectMethod(jstr, jmethodName, env->NewStringUTF(CHARSET_UTF8));
    // 获取jbyteArray数组长度
    jsize byteArraySize = env->GetArrayLength(byteArray);
    jbyte *jData = env->GetByteArrayElements(byteArray, JNI_FALSE);
    // byte数组转char数组
    if (byteArraySize > 0) {
        returnCharArray = (char *) malloc(byteArraySize + 1); //new char[byteArraySize+1];
        memcpy(returnCharArray, jData, byteArraySize);
        returnCharArray[byteArraySize] = 0;
    }
    env->ReleaseByteArrayElements(byteArray, jData, 0);
    return returnCharArray;
}

jstring charToJstring(JNIEnv *env, const char *charTobeConvert) {
    jclass strClass = env->FindClass("java/lang/String");
    // 调用 String(byte bytes[], String charsetName)构造方法
    jmethodID initMethodName = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(charTobeConvert));
    env->SetByteArrayRegion(bytes, 0, strlen(charTobeConvert), (jbyte *) charTobeConvert);
    jstring encoding = env->NewStringUTF(CHARSET_UTF8);
    return (jstring) env->NewObject(strClass, initMethodName, bytes, encoding);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_testJstringToChar(JNIEnv *env, jobject thiz, jstring jstr) {
    char *charArray = jstringToChar(env, jstr);
    LOGD("转换后的charArray为: %s", charArray)
}
extern "C"
JNIEXPORT void JNICALL
Java_com_oyp_ndkdemo_JNI_testCharToJstring(JNIEnv *env, jobject thiz) {
    char *charArray = "===============测试Char字符串转换Jstring===============";
    jstring jst = charToJstring(env,charArray);
    char *charArray2 = jstringToChar(env, jst);
    LOGD("转换后的jstring内容为: %s", charArray2)
}