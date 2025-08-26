package com.example.dragon_descendants

import android.content.Intent
import android.os.Bundle
import android.util.Log

import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.example.dragon_descendants.databinding.ActivityLoginBinding

import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.auth.FirebaseUser


class LoginPage : AppCompatActivity() {

    private lateinit var binding: ActivityLoginBinding
    private lateinit var auth: FirebaseAuth
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)

        auth = FirebaseAuth.getInstance()

        binding.loginBtn.setOnClickListener {
            val email = binding.email.text.toString().trim()
            val password = binding.password.text.toString().trim()

            if (email.isNotEmpty() && password.isNotEmpty()) {
                auth.signInWithEmailAndPassword(email, password)
                    .addOnCompleteListener(this) { task ->
                        if (task.isSuccessful) {
                            Log.d("LoginActivity", "signInWithEmail: success")
                            updateUI(auth.currentUser)
                        } else {
                            Log.w("LoginActivity", "signInWithEmail:failure", task.exception)
                            Toast.makeText(
                                baseContext, "Email or Password is incorrect or doesn't exist.",
                                Toast.LENGTH_SHORT
                            ).show()
                            updateUI(null)
                        }
                    }
            } else {
                Toast.makeText(
                    baseContext, "Please enter both email and password.",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }

        binding.registerBtn.setOnClickListener {
            Log.d("LoginPage", "Register button clicked")
            val intent = Intent(this, RegisterPage::class.java)
            startActivity(intent)
        }

        enableEdgeToEdge()
            ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
                val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
                v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
                insets
            }
        }

    private fun updateUI(user: FirebaseUser?){
        if(user != null){
            val intent = Intent(this, MainActivity::class.java)

            intent.putExtra("userId", user.uid)  // Pass user ID to MainActivity

            startActivity(intent)
            finish()
        }

    }
}

