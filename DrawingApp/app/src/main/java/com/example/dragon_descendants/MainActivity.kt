package com.example.dragon_descendants

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.widget.ImageView
import com.example.dragon_descendants.databinding.ActivityMainBinding
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.lifecycle.ViewModelProvider

class MainActivity : AppCompatActivity() {
    private lateinit var viewModel: DrawViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        installSplashScreen()
        super.onCreate(savedInstanceState)
        setTheme(R.style.Theme_Dragon_descendants)


        val binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Retrieve userId from the intent
        val userId = intent.getStringExtra("userId") ?: throw IllegalStateException("User ID must be passed to MainActivity")

        // Initialize ViewModel with userId
        initializeViewModel(userId)
    }

    private fun initializeViewModel(userId: String) {
        val repository = (application as DrawingApplication).drawingRepository
        val viewModelFactory = DrawViewModelFactory(repository, userId)
        viewModel = ViewModelProvider(this, viewModelFactory)[DrawViewModel::class.java]

        // Optional: If you need to fetch data immediately or set up observers
        viewModel.fetchUserDrawings(userId)
    }
}