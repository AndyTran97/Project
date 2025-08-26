package com.example.dragon_descendants

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName ="drawings")
data class Drawing(
    @PrimaryKey var id: String = "",
    var userId: String = "",     // Add this to link the drawing to a specific user
    var filename: String = "",   // This will be the name of the file in Firebase Storage
    var title: String = "",      // Title of the drawing
    var imageUrl: String = ""  // URL to access the image stored in Firebase Storage
)

//data class Drawing(var filename:String,
//    var title:String
//    ){
//    @PrimaryKey(autoGenerate = true)
//    var id: Int = 0
//}