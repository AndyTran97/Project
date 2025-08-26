plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
    //for room
    id("com.google.devtools.ksp")

    // For Google Service Plugins
    id("com.google.gms.google-services")
}

android {
    namespace = "com.example.dragon_descendants"
    compileSdk = 34

    defaultConfig {
        applicationId = "com.example.dragon_descendants"
        minSdk = 24
        targetSdk = 34
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags += ""
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }

    buildFeatures{
        viewBinding = true
        compose = true
    }

    composeOptions{
        kotlinCompilerExtensionVersion = "1.5.8"
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }
}

dependencies {
    implementation("androidx.fragment:fragment-ktx:1.6.2")
    implementation("androidx.core:core-ktx:1.12.0")
    implementation("androidx.appcompat:appcompat:1.6.1")
    implementation("com.google.android.material:material:1.11.0")
    implementation("androidx.constraintlayout:constraintlayout:2.1.4")
    implementation("com.github.skydoves:colorpickerview:2.3.0")
    implementation ("androidx.core:core-splashscreen:1.0.1")
    implementation("androidx.compose.ui:ui:1.6.5")
    implementation("androidx.lifecycle:lifecycle-runtime-ktx:2.7.0")
    implementation("androidx.activity:activity-compose:1.8.2")
    implementation("androidx.lifecycle:lifecycle-runtime-testing:2.7.0")
    implementation("androidx.compose.foundation:foundation-android:1.6.5")
    implementation("androidx.compose.material3:material3-android:1.2.1")
    implementation("com.google.firebase:firebase-firestore-ktx:24.11.1")
    implementation("androidx.media3:media3-test-utils:1.3.1")
    implementation("androidx.activity:activity:1.8.0")
    implementation("com.google.firebase:firebase-auth-ktx:22.3.1")
    implementation("com.google.firebase:firebase-database-ktx:20.3.1")
    testImplementation("junit:junit:4.13.2")
    androidTestImplementation("androidx.test.ext:junit:1.1.5")
    androidTestImplementation("androidx.test.espresso:espresso-core:3.5.1")

    //For testing
    testImplementation("org.mockito:mockito-core:3.12.4")
    testImplementation("io.mockk:mockk:1.12.0")
    testImplementation("org.jetbrains.kotlinx:kotlinx-coroutines-test:1.7.3")


    //For composable
    implementation("androidx.compose.material3:material3-adaptive:1.0.0-alpha06")
    implementation("androidx.compose.ui:ui-tooling-preview:1.6.5")

    //to get livedata + viewmodel stuff
    implementation("androidx.activity:activity-ktx:1.8.2")
    implementation("androidx.lifecycle:lifecycle-viewmodel-ktx:2.7.0")
    implementation("androidx.lifecycle:lifecycle-livedata-ktx:2.7.0")

    // Room DB
    implementation("androidx.room:room-common:2.6.1")
    implementation("androidx.room:room-ktx:2.6.1")
    debugImplementation("androidx.compose.ui:ui-tooling:1.6.5")
    ksp("androidx.room:room-compiler:2.6.1")


    ////////////////////////////////////////////////////
    // NAVIGATION

    // Navigation Kotlin
    val navVersion = "2.7.7"
    implementation("androidx.navigation:navigation-fragment-ktx:$navVersion")
    implementation("androidx.navigation:navigation-ui-ktx:$navVersion")

    // Feature module Support
    implementation("androidx.navigation:navigation-dynamic-features-fragment:$navVersion")

    // Testing Navigation
    androidTestImplementation("androidx.navigation:navigation-testing:$navVersion")

    //Testing Compose
    // Compose dependencies updated to 1.6.4
    implementation("androidx.compose.ui:ui:1.6.5")
    implementation("androidx.compose.foundation:foundation-android:1.6.5")
    implementation("androidx.compose.material3:material3-android:1.2.1") // Check for compatible version
    implementation("androidx.compose.ui:ui-tooling-preview:1.6.5")
    debugImplementation("androidx.compose.ui:ui-tooling:1.6.5")

    // Testing Compose
    androidTestImplementation("androidx.compose.ui:ui-test-junit4:1.6.5")
    //androidTestImplementation(platform("androidx.compose:compose-bom:2024.04.00"))


    // Jetpack Compose Integration
    implementation("androidx.navigation:navigation-compose:$navVersion")
    ////////////////////////////////////////////////////

    implementation("androidx.activity:activity-compose:1.8.2")
    implementation(platform("androidx.compose:compose-bom:2024.04.00"))
    implementation("androidx.compose.ui:ui")
    implementation("androidx.compose.ui:ui-graphics")
    implementation("androidx.compose.ui:ui-tooling-preview")
    implementation("androidx.compose.material3:material3")
    androidTestImplementation("androidx.test.ext:junit:1.1.5")
    androidTestImplementation("androidx.test.espresso:espresso-core:3.5.1")
    androidTestImplementation(platform("androidx.compose:compose-bom:2024.04.00"))
    androidTestImplementation("androidx.compose.ui:ui-test-junit4")
    debugImplementation("androidx.compose.ui:ui-tooling")
    debugImplementation("androidx.compose.ui:ui-test-manifest")
    implementation("androidx.compose.runtime:runtime-livedata:1.6.5")


    // For Firebase
    implementation("com.google.firebase:firebase-auth")
    implementation("com.google.firebase:firebase-storage")
    implementation("com.google.firebase:firebase-firestore:24.11.1")
    // Import the Firebase BoM
    implementation(platform("com.google.firebase:firebase-bom:32.8.1"))

    implementation("org.jetbrains.kotlin:kotlin-stdlib-jdk7")

}