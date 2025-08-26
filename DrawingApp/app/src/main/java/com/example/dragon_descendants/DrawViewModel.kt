package com.example.dragon_descendants

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Path
import android.graphics.PorterDuff
import android.graphics.PorterDuffXfermode
import android.util.Log
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.storage.FirebaseStorage
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.Calendar
import java.util.Locale
import java.util.UUID

//import androidx.compose.runtime.getValue
//import androidx.compose.runtime.mutableStateOf
//import androidx.compose.runtime.setValue


class DrawViewModel(private val repository: DrawingRepository, var userId: String) : ViewModel(){
    private var bitmap = MutableLiveData(Bitmap.createBitmap(800, 800, Bitmap.Config.ARGB_8888))
    val observableBM = bitmap as LiveData<Bitmap>
    var selectedTool: DrawingTool = DrawingTool.Pencil
    private var strokeW: Float = 5f
    private val native = NativeDrawingUtils()

    //current filename and title in VM
    private var filenameVM = "FileName"
    var titleVM = "Title"

    //All drawings from FireBase realtime database
    private var _drawings = MutableStateFlow<List<Drawing>>(emptyList())
    val drawings: Flow<List<Drawing>> = _drawings.asStateFlow()
    init {
        loadDrawings()
    }
    private fun loadDrawings() {
        viewModelScope.launch {
            repository.fetchDrawings(userId).collect { drawingsList ->
                _drawings.value = drawingsList
            }
        }
    }

    //for drawing shape
    private var startX = 0f
    private var startY = 0f
    private var endX = 0f
    private var endY = 0f

    // Temporary bitmap for shape previews
    var tempBitmap: Bitmap? = null
    init {
        tempBitmap = Bitmap.createBitmap(800, 800, Bitmap.Config.ARGB_8888)
    }

    // Setting for Pen and Eraser Tool
    private val paint = MutableLiveData(Paint().apply {
        color = Color.BLACK
        style = Paint.Style.STROKE
        strokeWidth =  strokeW
        xfermode = if(selectedTool == DrawingTool.Eraser) PorterDuffXfermode(PorterDuff.Mode.CLEAR) else null
    })

    fun fetchUserDrawings(userId: String) = callbackFlow {
        val databaseRef = FirebaseDatabase.getInstance().getReference("drawings/$userId")
        val eventListener = databaseRef.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val drawings = snapshot.children.mapNotNull { it.getValue(Drawing::class.java) }
                trySend(drawings).isSuccess
            }

            override fun onCancelled(error: DatabaseError) {
                close(error.toException())
            }
        })
        awaitClose {
            databaseRef.removeEventListener(eventListener)
        }
    }


    fun addDrawing() {
        viewModelScope.launch {
            filenameVM = "Drawing_${createFileName()}"
            titleVM = "Drawing on ${getCurrentDateTimeAsString()}"

            // Create a mutable bitmap if the current one isn't mutable
            val mutableBitmap = bitmap.value?.let {
                if (it.isMutable) it else it.copy(Bitmap.Config.ARGB_8888, true)
            } ?: Bitmap.createBitmap(800, 800, Bitmap.Config.ARGB_8888) // Fallback mutable bitmap

            mutableBitmap.eraseColor(Color.TRANSPARENT) // Clear the bitmap
            bitmap.value = mutableBitmap
            bitmap.value?.eraseColor(Color.TRANSPARENT)

            repository.uploadDrawing(userId, bitmap.value!!, filenameVM, titleVM,
                onSuccess = {
                    fetchUserDrawings(userId)  // Refresh the list after adding
                },
                onError = { error ->
                    Log.e("ViewModel", "Failed to upload drawing: $error")
                }
            )
        }
    }
    fun updateDrawing() {
        viewModelScope.launch {
            repository.updateDrawing(userId, filenameVM, titleVM, bitmap.value!!,
                onSuccess = { /* Handle success */ },
                onError = { /* Handle error */ }
            )
        }
    }

    fun loadDrawingImage(userId: String, filename: String, title: String) {
        filenameVM = filename
        titleVM = title
        repository.loadImage(userId, filename, onSuccess = { newBitmap ->
            // Ensure the bitmap is mutable
            val mutableBitmap = newBitmap.copy(Bitmap.Config.ARGB_8888, true)
            bitmap.postValue(mutableBitmap)
        }, onError = { exception ->
            Log.e("ViewModel", "Error loading image: ${exception.message}")
        })
    }

    fun deleteDrawing(filename: String) {
        viewModelScope.launch {
            repository.deleteDrawing(userId, filename,
                onSuccess = {
                    fetchUserDrawings(userId)  // Refresh the list after deletion
                },
                onError = { /* Handle error */ }
            )
        }
    }

    fun setPaintColor(color: Int) {
        paint.value?.color = color
        paint.value = paint.value
    }

    fun createNewPaint(color:Int, tool: DrawingTool):Paint{
        return Paint().apply{
            this.color = color
            style = Paint.Style.STROKE
            strokeWidth = strokeW
            xfermode = if(tool == DrawingTool.Eraser) PorterDuffXfermode(PorterDuff.Mode.CLEAR) else null
        }
    }

    //Use for Pencil or Eraser
    fun drawPath(path: Path){
        //pencil and eraser set up
        val currentPaint:Paint = if (selectedTool == DrawingTool.Eraser){
            createNewPaint(Color.TRANSPARENT, DrawingTool.Eraser)
        }
        else{
            paint.value ?: createNewPaint(Color.BLACK, DrawingTool.Pencil)
        }

        val bitmapCanvas = Canvas(bitmap.value!!)
        bitmapCanvas.drawPath(path, currentPaint)
        bitmap.value = bitmap.value
    }

    fun setStrokeWidth(strokeWidth: Float) {
        strokeW = strokeWidth
        paint.value?.strokeWidth = strokeW
        paint.value = paint.value // Trigger LiveData update
    }

    //Use for Shape
    fun setStartPoint(x: Float, y: Float) {
        startX = x
        startY = y
    }
    fun setEndPoint(x: Float, y: Float) {
        endX = x
        endY = y
    }

    fun drawShapePreview() {
        val tempCanvas = Canvas(tempBitmap!!)
        tempCanvas.drawColor(0, PorterDuff.Mode.CLEAR) // Clear the temp canvas

        when (selectedTool) {
            DrawingTool.Circle -> drawCircle(tempCanvas, startX, startY, endX, endY)
            DrawingTool.Rectangle -> drawRectangle(tempCanvas, startX, startY, endX, endY)
            DrawingTool.Triangle -> drawTriangle(tempCanvas, startX, startY, endX, endY)
            else -> {}
        }
    }

    fun finalizeShapeDrawing() {
        val bitmapCanvas = Canvas(bitmap.value!!)
        val tempCanvas = Canvas(tempBitmap!!)
        when (selectedTool) {
            DrawingTool.Circle -> drawCircle(bitmapCanvas, startX, startY, endX, endY)
            DrawingTool.Rectangle -> drawRectangle(bitmapCanvas, startX, startY, endX, endY)
            DrawingTool.Triangle -> drawTriangle(bitmapCanvas, startX, startY, endX, endY)
            else -> {}
        }
        bitmap.value = bitmap.value
        tempCanvas.drawColor(0, PorterDuff.Mode.CLEAR)
    }

    private fun drawCircle(canvas: Canvas, startX: Float, startY: Float, endX: Float, endY: Float) {
        val radius = Math.hypot((endX - startX).toDouble(), (endY - startY).toDouble()).toFloat()
        canvas.drawCircle(startX, startY, radius, paint.value!!)
    }

    private fun drawRectangle(canvas: Canvas, startX: Float, startY: Float, endX: Float, endY: Float) {
        canvas.drawRect(startX, startY, endX, endY, paint.value!!)
    }


    private fun drawTriangle(canvas: Canvas, startX: Float, startY: Float, endX: Float, endY: Float) {
        val path = Path()

        // First point (start point)
        path.moveTo(startX, startY)

        // Second point (end point)
        path.lineTo(endX, endY)

        // Calculating the third point for an isosceles triangle
        val dx = endX - startX
        val dy = endY - startY
        val midX = (startX + endX) / 2
        val midY = (startY + endY) / 2

        // Perpendicular distance from the midpoint of the line to the third point
        val perpDist = Math.sqrt((dx * dx + dy * dy).toDouble()) / 2

        // Calculating the third point
        val thirdX = midX - perpDist * dy / Math.sqrt((dx * dx + dy * dy).toDouble())
        val thirdY = midY + perpDist * dx / Math.sqrt((dx * dx + dy * dy).toDouble())

        // Third point
        path.lineTo(thirdX.toFloat(), thirdY.toFloat())

        // Closing the path to form a triangle
        path.close()

        // Drawing the triangle
        canvas.drawPath(path, paint.value!!)
    }

    fun selectTool(tool: DrawingTool){
        selectedTool = tool
        paint.value = createNewPaint(paint.value!!.color, tool)
    }

    fun SetFilename(filename:String){
        filenameVM = filename
    }


    //Create the name for file using the format datetime
    fun createFileName(): String {
        val calendar = Calendar.getInstance()
        val formatter = SimpleDateFormat("yyyy_MM_dd_HH_mm_ss", Locale.getDefault())
        return formatter.format(calendar.time)
    }

    //Create the name for title using the format datetime
    fun getCurrentDateTimeAsString(): String {
        val calendar = Calendar.getInstance()
        val formatter = SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())
        return formatter.format(calendar.time)
    }


    fun getFileName():String{
        return filenameVM
    }

    // Call the invert color func from native drawing utils
    fun InvertColor(){
        // Call the JNI method to invert the colors
        native.invertColor(bitmap.value!!)
    }

    // Call the blur image func from native drawing utils
    fun AddNoise(){
        native.addNoise(bitmap.value!!)
    }

}

class DrawViewModelFactory(private val repository:DrawingRepository, private val userId: String):ViewModelProvider.Factory{
    override fun <T:ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(DrawViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return DrawViewModel(repository, userId) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}



