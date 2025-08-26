package com.example.dragon_descendants

import android.content.Intent
import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.example.dragon_descendants.databinding.ActivityRegisterPageBinding
import com.google.firebase.auth.FirebaseAuth

class RegisterPage : AppCompatActivity() {

    private lateinit var binding: ActivityRegisterPageBinding
    private lateinit var auth: FirebaseAuth

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityRegisterPageBinding.inflate(layoutInflater)
        setContentView(binding.root)

        auth = FirebaseAuth.getInstance()

        binding.registerBtn.setOnClickListener{
            val email = binding.email.text.toString().trim()
            val password = binding.password.text.toString().trim()

            if(email.isNotEmpty() && password.isNotEmpty()){
                // Create a new user with Firebase
                auth.createUserWithEmailAndPassword(email,password).addOnCompleteListener(this){
                    task ->
                    if(task.isSuccessful){
                        val user = auth.currentUser
                        Toast.makeText(baseContext, "Registration successful.",
                            Toast.LENGTH_SHORT).show()
                            finish()
                    }
                    else{
                        Toast.makeText(baseContext,"Registration failed",
                            Toast.LENGTH_LONG).show()
                    }
                }
            } else {
                Toast.makeText(baseContext,"Email and password can't be empty",
                    Toast.LENGTH_SHORT).show()
            }
        }

        binding.backBtn.setOnClickListener{
            val intent = Intent(this, LoginPage::class.java)
            //Clear all activities on the top of the stack
            intent.flags = Intent.FLAG_ACTIVITY_CLEAR_TOP or Intent.FLAG_ACTIVITY_SINGLE_TOP
            startActivity(intent)
            finish()
        }

        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }
    }
}