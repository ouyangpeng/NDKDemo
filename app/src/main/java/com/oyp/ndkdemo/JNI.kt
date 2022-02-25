package com.oyp.ndkdemo

import android.graphics.PointF

object JNI {
    // Used to load the 'ndkdemo' library on application startup.
    init {
        System.loadLibrary("ndkdemo")
    }

    /**
     * A native method that is implemented by the 'ndkdemo' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun encode(pass: String, length: Int): String

    external fun decode(pass: String, length: Int): String

    external fun transmit(array: IntArray): IntArray

    external fun intArraySum(array: IntArray): Int

    external fun testLog()


    // Kotlin对外public的方法，用于业务调用
    fun setFaceFeature(feature: FaceFeatureBean) {
        nativeSetFaceFeatureBean(feature)
    }

    // Native层方法
    private external fun nativeSetFaceFeatureBean(feature: FaceFeatureBean)

    // Kotlin对外public的方法，用于业务调用
    fun setFaceFeature2(feature: FaceFeatureBean) {
        nativeSetFaceFeatureBean2(feature)
    }

    // Native层方法
    private external fun nativeSetFaceFeatureBean2(feature: FaceFeatureBean)
}