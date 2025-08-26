package com.example.dragon_descendants

import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.junit4.createComposeRule
import androidx.compose.ui.test.onNodeWithText
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow
import org.junit.Rule
import org.junit.Test

class ComposeTest {

    @get:Rule
    val composeTestRule = createComposeRule()

    @Test
    fun ui_drawing_test() {
        val list: Flow<List<Drawing>> = flow {
            val list = mutableListOf<Drawing>()
            for (i in 1..4) {
                list += Drawing("DRAWING_$i", "PATH_$i")
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
