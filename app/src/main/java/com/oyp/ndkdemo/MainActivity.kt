package com.oyp.ndkdemo

import android.graphics.PointF
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.oyp.ndkdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private val TAG = "NDKDemo"
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        JNI.testLog()

        // 调用jni方法 返回一个C++的字符串
        val text1 = JNI.stringFromJNI()
        // 调用jni方法 对字符串加密 然后返回
        val text2 = JNI.encode(text1, text1.length)
        // 调用jni方法 对字符串解密 然后返回
        val text3 = JNI.decode(text2, text2.length)

        val array = intArrayOf(5, 1, 2, 3, 4, 0, 9, 8, 7, 10, 6)
        val text4 = "调用C++ 方法 修改前：${array.contentToString()}"

        val text5 = "调用C++ 方法 修改前 数组的和为：${JNI.intArraySum(array)}"

        // 调用jni方法 把数组的每个元素都 * 2
        JNI.transmit(array)

        val text6 = "调用C++ 方法 修改后：${array.contentToString()}"
        val text7 = "调用C++ 方法 修改后 数组的和为：${JNI.intArraySum(array)}"

        binding.sampleText.text = "从C层返回的字符串为：\n$text1" +
                "\n\n 秘文为：\n$text2" +
                "\n\n 原文为：\n$text3" +
                "\n\n $text4" +
                "\n\n $text5 " +
                "\n\n $text6  " +
                "\n\n $text7"


        // 传递给C++ 实体类  去jni层解析
        val bean = FaceFeatureBean(
            faceId = 1,
            boundingBox = arrayOf(
                PointF(0.10f, 0.20f),
                PointF(0.30f, 0.40f)
            ),
            landmarks = arrayOf(
                PointF(0.50f, 0.60f),
                PointF(0.70f, 0.80f)
            ),
            visibilities = listOf(
                100.10f,
                100.20f,
                100.30f,
                100.40f
            ),
            yaw = 0.99f,
            pitch = 0.88f,
            roll = 0.77f
        )

        JNI.setFaceFeature(bean)

        JNI.setFaceFeature2(bean)

    }
}