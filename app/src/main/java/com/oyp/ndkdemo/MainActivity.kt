package com.oyp.ndkdemo

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.PointF
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.oyp.ndkdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private val TAG = "NDKDemo"
    private lateinit var binding: ActivityMainBinding

    private val ACTION_REQUEST_PERMISSIONS = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                ACTION_REQUEST_PERMISSIONS
            )
        } else {
            testNativeLeak()
        }
    }

    private fun testNativeLeak() {
        // 开启监控
        JNI.startMonitoringAllThreads()

        // 执行Native方法
        testNativeMethod()

        JNI.stopAllMonitoring()
        JNI.writeLeaksResultToFile("sdcard/NativeLeakLog.txt")
    }

    private fun testNativeMethod() {
        JNI.testLog()

        JNI.testStackAndHeap()

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

        val student = JNI.getStudentFromJNI();

        binding.sampleText.text = "从C层返回的字符串为：\n$text1" +
                "\n\n 秘文为：\n$text2" +
                "\n\n 原文为：\n$text3" +
                "\n\n $text4" +
                "\n\n $text5 " +
                "\n\n $text6  " +
                "\n\n $text7" +
                "\n\n 调用C函数返回的结构体转换java bean为； ${student.getStudentInfo()}"
    }


    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String?>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == ACTION_REQUEST_PERMISSIONS) {
            var isAllGranted = true
            for (grantResult in grantResults) {
                isAllGranted = isAllGranted and (grantResult == PackageManager.PERMISSION_GRANTED)
            }
            if (isAllGranted) {
                testNativeLeak()
            } else {
                Toast.makeText(this.applicationContext, "权限被拒绝", Toast.LENGTH_SHORT).show()
            }
        }
    }
}