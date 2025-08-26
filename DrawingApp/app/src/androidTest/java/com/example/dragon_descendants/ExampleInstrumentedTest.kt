package com.example.dragon_descendants

import android.content.Context
import android.graphics.Bitmap
import androidx.compose.runtime.Composable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.Observer
import androidx.lifecycle.asLiveData
import androidx.media3.test.utils.TestUtil
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import androidx.test.espresso.Espresso.onView
import androidx.test.espresso.action.ViewActions.click
import androidx.test.espresso.matcher.ViewMatchers.withContentDescription
import androidx.test.espresso.matcher.ViewMatchers.withId
import androidx.test.espresso.matcher.ViewMatchers.withText
import androidx.test.platform.app.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.google.firebase.firestore.auth.User
import kotlinx.coroutines.CoroutineScope

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.withContext
import org.junit.After

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*
import org.junit.Before
import org.junit.Rule
import java.io.IOException
import java.util.concurrent.CountDownLatch
import java.util.concurrent.TimeUnit

import androidx.compose.ui.test.*
import androidx.compose.ui.test.junit4.createComposeRule


/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @get:Rule
    val composeTestRule = createComposeRule()

    @Test
    fun useAppContext() {
        // Context of the app under test.
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        assertEquals("com.example.dragon_descendants", appContext.packageName)
    }


    /**
     * Instrumented test, which will execute on an Android device.
     *
     * See [testing documentation](http://d.android.com/tools/testing).
     */
    private lateinit var userDao: DrawingDAO
    private lateinit var db: DrawingDatabase
    private lateinit var scope: CoroutineScope
    private lateinit var drawingRepository: DrawingRepository
    private lateinit var bitmap: MutableLiveData<Bitmap>
    @Before
    fun createDb() {

        val context = ApplicationProvider.getApplicationContext<Context>()

        db = Room.inMemoryDatabaseBuilder(
            context, DrawingDatabase::class.java).build()
        userDao = db.drawingDAO()

        scope = CoroutineScope(SupervisorJob())

        //create our repository singleton, using lazy to access the DB when we need it
        drawingRepository = DrawingRepository(scope, db.drawingDAO(), context)

        bitmap = MutableLiveData(Bitmap.createBitmap(800, 800, Bitmap.Config.ARGB_8888))

        // Clear the repository before each test
        clearRepository()
    }

    @After
    @Throws(IOException::class)
    fun closeDb() {
        db.close()
    }

    private fun clearRepository() {
        runBlocking(Dispatchers.IO){
                // Delete all drawings in the repository
                drawingRepository.allDrawings.asLiveData().value?.forEach { drawing ->
                    drawingRepository.deleteImage(drawing.filename)
                }
        }
    }

    @Test
    @Throws(Exception::class)
    fun addDrawing() {

        runBlocking {
            withContext(Dispatchers.Main) {

                // Set up LiveData observer to track changes
                val observer = Observer<List<Drawing>> { drawings ->
                    assertNotNull(drawings)
                    assertEquals(1, drawings.size)

                }
                drawingRepository.allDrawings.asLiveData().observeForever(observer)

                // Perform the operation that triggers LiveData updates
                drawingRepository.saveImage("filename.png", "Title 1", bitmap.value!!)

                // Wait for LiveData to update (timeout after 2 seconds)
                val latch = CountDownLatch(1)
                latch.await(2, TimeUnit.SECONDS)

                // Remove the observer to prevent memory leaks
                drawingRepository.allDrawings.asLiveData().removeObserver(observer)
            }
        }
    }

    @Test
    @Throws(Exception::class)
    fun deleteDrawing() {

        runBlocking {
            withContext(Dispatchers.Main) {

                // Set up LiveData observer to track changes
                val observer = Observer<List<Drawing>> { drawings ->
                    assertNotNull(drawings)
                    assertEquals(1, drawings.size)
                }
                drawingRepository.allDrawings.asLiveData().observeForever(observer)

                // Perform the delete operation
                drawingRepository.saveImage("filename.png", "Title1", bitmap.value!!)
                drawingRepository.saveImage("filename2.png", "Title2", bitmap.value!!)
                drawingRepository.deleteImage("filename.png")

                // Wait for LiveData to update (timeout after 2 seconds)
                val latch = CountDownLatch(1)
                latch.await(2, TimeUnit.SECONDS)

                // Remove the observer to prevent memory leaks
                drawingRepository.allDrawings.asLiveData().removeObserver(observer)
            }
        }
    }

    @Test
    @Throws(Exception::class)
    fun updateDrawing() {
        runBlocking {
            withContext(Dispatchers.Main) {
                // Set up LiveData observer to track changes
                val observer = Observer<List<Drawing>> { drawings ->
                    assertNotNull(drawings)
                    assertEquals(1, drawings.size)
                    // Additional assertions if needed
                }
                drawingRepository.allDrawings.asLiveData().observeForever(observer)

                // Perform the delete operation
                drawingRepository.saveImage("filename.png", "Title 1", bitmap.value!!)
                drawingRepository.updateImage("filename.png", "Title 3", bitmap.value!!)

                // Wait for LiveData to update (timeout after 2 seconds)
                val latch = CountDownLatch(1)
                latch.await(2, TimeUnit.SECONDS)

                // Remove the observer to prevent memory leaks
                drawingRepository.allDrawings.asLiveData().removeObserver(observer)
            }
        }
    }



    @Test
    fun ui_drawing_test() {
        val list: Flow<List<Drawing>> = flow {
            val list = mutableListOf<Drawing>()
            for (i in 1..4) {
                list += Drawing("PATH_$i", "DRAWING_$i")
            }
            emit(list)
        }

        composeTestRule.setContent { DrawingListScreen(drawings = list) }

        composeTestRule.onNodeWithText("DRAWING_1").assertIsDisplayed()
        composeTestRule.onNodeWithText("DRAWING_2").assertIsDisplayed()
        composeTestRule.onNodeWithText("DRAWING_3").assertIsDisplayed()
        composeTestRule.onNodeWithText("DRAWING_4").assertIsDisplayed()
    }
}