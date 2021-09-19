package com.oyp.ndkdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.oyp.ndkdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        val text1 = stringFromJNI()
        val text2 = encode(text1,text1.length)
        val text3 = decode(text2,text2.length)
        binding.sampleText.text = "$text1\n 秘文为：$text2\n 原文为：$text3"
    }

    /**
     * A native method that is implemented by the 'ndkdemo' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun encode(pass: String, length: Int): String

    external fun decode(pass: String, length: Int): String

    companion object {
        // Used to load the 'ndkdemo' library on application startup.
        init {
            System.loadLibrary("ndkdemo")
        }
    }
}