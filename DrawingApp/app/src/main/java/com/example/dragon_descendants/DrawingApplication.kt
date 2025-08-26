package com.example.dragon_descendants

import android.app.Application
import android.content.Context
import androidx.fragment.app.activityViewModels
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.SupervisorJob
import androidx.room.Room
import kotlinx.coroutines.MainScope
import androidx.fragment.app.viewModels

class DrawingApplication:Application() {
    companion object {
        lateinit var appContext: Context
    }

    override fun onCreate() {
        super.onCreate()

        appContext = applicationContext
    }

    val scope = CoroutineScope(SupervisorJob())
    //get a reference to the DB singleton
    val db by lazy {Room.databaseBuilder(
        applicationContext,
        DrawingDatabase::class.java,
        "drawings_database"
    ).build()}
    //create our repository singleton, using lazy to access the DB when we need it
    val drawingRepository by lazy {DrawingRepository(scope, db.drawingDAO(), appContext)}
}