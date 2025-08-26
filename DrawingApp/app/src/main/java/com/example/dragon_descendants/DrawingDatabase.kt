package com.example.dragon_descendants

import android.content.Context
import androidx.room.Dao
import androidx.room.Database
import androidx.room.Insert
import androidx.room.Query
import androidx.room.Room
import androidx.room.RoomDatabase
import androidx.room.Update
import kotlinx.coroutines.flow.Flow

@Database(entities = [Drawing::class], version = 1, exportSchema = false)
abstract class DrawingDatabase: RoomDatabase(){
    abstract fun drawingDAO() : DrawingDAO


    // Singleton object
    companion object {
        @Volatile
        private var INSTANCE: DrawingDatabase? = null

        fun getDatabase(context: Context): DrawingDatabase{
            return INSTANCE ?: synchronized(this){
                if(INSTANCE != null) return INSTANCE!!
                val instance = Room.databaseBuilder(
                    context.applicationContext,
                    DrawingDatabase::class.java,
                    "drawings_database"
                )
                    .fallbackToDestructiveMigration()

                    .build()
                INSTANCE = instance
                instance
            }
        }
    }
}

@Dao
interface DrawingDAO{
    @Insert
    suspend fun addDrawing(data: Drawing)

    @Query("DELETE FROM drawings WHERE filename = :filename")
    suspend fun deleteDrawing(filename: String)

    @Query("UPDATE drawings SET title = :newTitle WHERE filename = :targetFilename")
    suspend fun updateTitle(targetFilename: String, newTitle: String)

    @Query("SELECT * FROM drawings")
    fun getAllDrawings() : Flow<List<Drawing>>
}