package com.oyp.ndkdemo

import java.io.File

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
    // Kotlin对外public的方法，用于业务调用
    fun setFaceFeature2(feature: FaceFeatureBean) {
        nativeSetFaceFeatureBean2(feature)
    }

    // Native层方法
    private external fun nativeSetFaceFeatureBean(feature: FaceFeatureBean)

    private external fun nativeSetFaceFeatureBean2(feature: FaceFeatureBean)


    /**
     * starts monitoring memory allocations in all threads
     */
    external fun startMonitoringAllThreads()

    /**
     * starts monitoring memory allocations in current thread
     */
    external fun startMonitoringThisThread()

    /**
     * stops monitoring memory allocations (in all threads or in
     * this thread only, depends on the function used to start
     * monitoring
     */
    external fun stopMonitoringAllocations()

    /**
     * stops all monitoring - both of allocations and releases
     */
    external fun stopAllMonitoring()


    /**
     * writes report with all memory leaks
     */
    private external fun writeLeaksToFile(filePath: String)

    /**
     * writes report with all memory leaks
     */
    fun writeLeaksResultToFile(filePath: String?) {
        if (filePath == null) {
            throw NullPointerException("filePath is null")
        }
        val file = File(filePath)
        require(!(file.exists() && file.isDirectory)) { "can not write data to a directory" }
        require(!(!file.parentFile.exists() && !file.parentFile.mkdirs())) { "can not create parent folder for file '" + file.absolutePath + "'" }
        writeLeaksToFile(filePath)
    }

}