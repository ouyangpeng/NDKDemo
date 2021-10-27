package com.oyp.ndkdemo

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

    external fun transmit(array: IntArray) : IntArray

    external fun intArraySum(array: IntArray) : Int

    external fun testLog()

}