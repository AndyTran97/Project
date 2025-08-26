package com.example.dragon_descendants

import android.view.View
import android.graphics.Canvas
import android.content.Context
import android.graphics.Bitmap
import android.graphics.Rect
import android.util.AttributeSet

import android.graphics.Path
import android.graphics.PorterDuff
import android.graphics.PorterDuffXfermode
import android.util.Log


class DrawingView(context: Context, attrs: AttributeSet) : View(context, attrs){
    var bitmap: Bitmap? = null
    var tempBitmap: Bitmap? = null
    private val rect:Rect by lazy {Rect(0,0,width,height)}
    private val path: Path = Path()

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        bitmap?.let {
            canvas.drawBitmap(it, null, rect, null)
        }
        tempBitmap?.let {
            canvas.drawBitmap(it, null, rect, null)
        }
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        rect.set(0, 0, w, h)
    }

    fun updateBitmap(newBitmap:Bitmap, newTempBitmap: Bitmap? = null){
        bitmap = newBitmap
        tempBitmap = newTempBitmap
        invalidate()
    }

}

