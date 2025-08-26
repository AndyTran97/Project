package com.example.dragon_descendants

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.util.Log
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import com.google.firebase.storage.FirebaseStorage
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.launch
import java.io.ByteArrayOutputStream
import java.io.File
import java.io.FileOutputStream
import java.util.UUID

class DrawingRepository(private val scope:CoroutineScope, private val dao:DrawingDAO, private val context: Context){
    val allDrawings = dao.getAllDrawings()
    private val appDir = context.filesDir

    private val storageRef = FirebaseStorage.getInstance().reference
    private val databaseRef = FirebaseDatabase.getInstance().getReference("drawings")

    // Function to clean and prepare filename
    private fun prepareFilename(filename: String): String {
        return filename.filter { it.isLetterOrDigit() || it == '_' }
    }

    //Fetch all drawings in database
    fun fetchDrawings(userId: String) = callbackFlow {
        val eventListener = databaseRef.child(userId).addValueEventListener(object : ValueEventListener {
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

    //Upload Drawing to database
    fun uploadDrawing(userId: String, image: Bitmap, file_name: String, title: String, onSuccess: () -> Unit, onError: (Exception) -> Unit) {
        val filename = prepareFilename(file_name)
        val fileRef = storageRef .child("$userId/$filename.png")

        // Convert Bitmap to ByteArray
        val baos = ByteArrayOutputStream()
        image.compress(Bitmap.CompressFormat.PNG, 100, baos)
        val data = baos.toByteArray()

        // Upload to Firebase Storage
        fileRef.putBytes(data)
            .continueWithTask { task ->
                if (!task.isSuccessful) {
                    task.exception?.let {
                        onError(it)  // Properly handle the error
                        throw it
                    }
                }
                fileRef.downloadUrl
            }.addOnCompleteListener { task ->
                if (task.isSuccessful) {
                    val imageUrl = task.result.toString()
                    val newDrawing = Drawing(userId = userId, filename = filename, title = title, imageUrl = imageUrl)
                    saveDrawingMetadata(userId, newDrawing, onSuccess, onError)
                } else {
                    onError(task.exception ?: Exception("Failed to upload image and retrieve URL"))
                }
            }
    }

    // Save or update drawing metadata in the Firebase Database
    private fun saveDrawingMetadata(userId: String, drawing: Drawing, onSuccess: () -> Unit, onError: (Exception) -> Unit) {
        val drawingRef = databaseRef.child(userId).child(drawing.filename)

        // Set the value in the correct location
        drawingRef.setValue(drawing)
            .addOnSuccessListener {
                onSuccess()
            }
            .addOnFailureListener {
                onError(Exception(it))
            }
    }

    //Update the drawing accordingly to database
    fun updateDrawing(userId: String, file_name: String, newTitle: String, newImage: Bitmap, onSuccess: () -> Unit, onError: (Exception) -> Unit) {
        val filename = prepareFilename(file_name)
        val fileRef = storageRef.child("$userId/$filename.png")

        // Convert bitmap to byte array
        val baos = ByteArrayOutputStream()
        newImage.compress(Bitmap.CompressFormat.PNG, 100, baos)
        val imageData = baos.toByteArray()

        // Update the image in Firebase Storage
        fileRef.putBytes(imageData)
            .continueWithTask { task ->
                if (!task.isSuccessful) {
                    task.exception?.let { throw it }
                }
                fileRef.downloadUrl
            }
            .addOnCompleteListener { task ->
                if (task.isSuccessful) {
                    val imageUrl = task.result.toString()
                    // Update the Realtime Database entry with new title and imageUrl
                    val updateMap = mapOf("title" to newTitle, "imageUrl" to imageUrl)
                    databaseRef.child(userId).child(file_name).updateChildren(updateMap)
                        .addOnSuccessListener { onSuccess() }
                        .addOnFailureListener { onError(it) }
                } else {
                    onError(task.exception ?: Exception("Failed to update image and metadata"))
                }
            }
    }

    // Function to fetch the image URL from Firebase Database and then download the Bitmap from Firebase Storage
    fun loadImage(userId: String, filename: String, onSuccess: (Bitmap) -> Unit, onError: (Exception) -> Unit) {
        val databaseRef = FirebaseDatabase.getInstance().getReference("drawings/$userId/$filename")
        databaseRef.child("imageUrl").get().addOnSuccessListener { dataSnapshot ->
            val imageUrl = dataSnapshot.value as String?
            if (imageUrl != null) {
                // Now fetch the image from Firebase Storage
                fetchImageFromUrl(imageUrl, onSuccess, onError)
            } else {
                onError(Exception("Image URL not found"))
            }
        }.addOnFailureListener {
            onError(it)
        }
    }

    // Helper function to fetch a Bitmap from a URL
    private fun fetchImageFromUrl(url: String, onSuccess: (Bitmap) -> Unit, onError: (Exception) -> Unit) {
        val imageRef = FirebaseStorage.getInstance().getReferenceFromUrl(url)
        val localFile = File.createTempFile("images", "png")
        imageRef.getFile(localFile).addOnSuccessListener {
            val bitmap = BitmapFactory.decodeFile(localFile.absolutePath)
            onSuccess(bitmap)
        }.addOnFailureListener {
            onError(it)
        }
    }

    //Delete the drawing
    fun deleteDrawing(userId: String, filename: String, onSuccess: () -> Unit, onError: (Exception) -> Unit) {
        // Create references to the storage and database
        val fileRef = FirebaseStorage.getInstance().reference.child("$userId/$filename.png")
        val drawingRef = FirebaseDatabase.getInstance().getReference("drawings/$userId").orderByChild("filename").equalTo(filename)

        // First, delete the file from Firebase Storage
        fileRef.delete().addOnSuccessListener {
            // If the file is deleted successfully, proceed to delete the metadata from the database
            drawingRef.addListenerForSingleValueEvent(object : ValueEventListener {
                override fun onDataChange(snapshot: DataSnapshot) {
                    for (child in snapshot.children) {
                        child.ref.removeValue().addOnSuccessListener {
                            onSuccess()
                        }.addOnFailureListener { e ->
                            onError(e)
                        }
                    }
                }

                override fun onCancelled(error: DatabaseError) {
                    onError(Exception(error.message))
                }
            })
        }.addOnFailureListener { e ->
            onError(e)
        }
    }

    //For testing
    fun generateFilename(title: String): String {
        val timestamp = System.currentTimeMillis()
        return "$title-$timestamp.png"
    }


}