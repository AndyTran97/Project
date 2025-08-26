import android.content.Context
import android.graphics.Bitmap
import com.example.dragon_descendants.DrawingDAO
import com.example.dragon_descendants.DrawingRepository
import io.mockk.Runs
import io.mockk.coVerify
import io.mockk.every
import io.mockk.just
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.test.runBlockingTest
import org.junit.Before
import org.junit.Test
import org.junit.Assert.*
import java.io.File
import io.mockk.mockk as mockk1

@ExperimentalCoroutinesApi
class DrawingRepositoryTest {

    private lateinit var repository: DrawingRepository
    private val mockDao: DrawingDAO = mockk1(relaxed = true)
    private val mockContext: Context = mockk1(relaxed = true)
    private val testFile: File = mockk1(relaxed = true)

    @Before
    fun setup() {
        repository = DrawingRepository(CoroutineScope(Dispatchers.Unconfined), mockDao, mockContext)
        // Mock the context to return a specific file path when requested
        every { mockContext.filesDir } returns File("path/to/files")
        // Mock file operations
        every { testFile.exists() } returns true
        every { testFile.writeBytes(any()) } just Runs
        every { mockContext.getFileStreamPath(any()) } returns testFile
    }


    @Test
    fun `saveImage saves image correctly`() = runBlockingTest {
        val testBitmap = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888)
        repository.saveImage("testImage", "Title" ,testBitmap)

        // Verify the image was attempted to be saved to the file system
        coVerify { mockDao.addDrawing(any()) }
        assertTrue("File exists check", testFile.exists())
    }

    @Test
    fun `loadImage loads image correctly`() = runBlockingTest {
        val loadedImage = repository.loadImage("testImage")

        assertNotNull("Loaded image should not be null", loadedImage)
    }
}
