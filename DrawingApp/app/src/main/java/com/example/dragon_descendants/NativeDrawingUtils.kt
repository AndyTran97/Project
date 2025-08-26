package com.example.dragon_descendants

import android.graphics.Bitmap

class NativeDrawingUtils {
    companion object {

        init{
            System.loadLibrary("dragon_descendants")
        }
    }

    external fun invertColor(bitmap: Bitmap)
    external fun addNoise(bitmap:Bitmap)
}