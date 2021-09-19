package com.oyp.ndkdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import com.oyp.ndkdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        val text1 = JNI.stringFromJNI()
        val text2 = JNI.encode(text1,text1.length)
        val text3 = JNI.decode(text2,text2.length)
        binding.sampleText.text = "$text1\n 秘文为：$text2\n 原文为：$text3"
    }
}