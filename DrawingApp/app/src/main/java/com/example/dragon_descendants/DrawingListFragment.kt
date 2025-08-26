package com.example.dragon_descendants

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.fragment.app.activityViewModels
import androidx.navigation.fragment.findNavController
import com.example.dragon_descendants.databinding.FragmentDrawingListBinding
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column

import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.LazyColumn

import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import kotlinx.coroutines.flow.Flow
import java.io.File

class DrawingListFragment : Fragment() {
    private lateinit var binding: FragmentDrawingListBinding
//    val viewModel: DrawViewModel by activityViewModels {
//        DrawViewModel.DrawViewModelFactory((requireActivity().application as DrawingApplication).drawingRepository)
//    }

    private val viewModel: DrawViewModel by activityViewModels()
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?):View{

        binding = FragmentDrawingListBinding.inflate(layoutInflater)

        binding.AddBtn.setContent{
            AddButton {
                viewModel.addDrawing()
                findNavController().navigate(R.id.action_drawingListFragment2_to_drawPanelFragment2)
            }
        }

        binding.composeView.setContent {
            DrawingListScreen(
                viewModel.drawings,
                onItemClick = { filename, title ->
                    viewModel.loadDrawingImage(viewModel.userId, filename, title)
                    findNavController().navigate(R.id.action_drawingListFragment2_to_drawPanelFragment2)
                }
            ) { filename, title ->
                viewModel.deleteDrawing(filename)
            }
        }

        return binding.root
    }

}

fun loadBitmapFromFile(filePath: String): Bitmap? {
    val file = File(filePath)
    if(!file.exists()){
        Log.e("LoadBitMap", "File does not exist: $filePath")
        return null
    }

    return BitmapFactory.decodeFile(filePath)
}

//Compose method to generate the drawing list
@Composable
fun DrawingListScreen(
    drawings: Flow<List<Drawing>>,
    onItemClick: (String, String) -> Unit = { _, _ -> },
    onDeleteClick: (String, String) -> Unit = { _, _ -> }) {
    val allDrawings by drawings.collectAsState(listOf())

    LazyColumn(verticalArrangement = Arrangement.spacedBy(10.dp)) {
        items(allDrawings) { drawing ->
            DrawingItem(drawing.filename, drawing.title, onItemClick = onItemClick, onDeleteClick = onDeleteClick)
        }
    }
}

//Compose method to create UI for Drawing Items
@Composable
fun DrawingItem(filename: String, title: String,
                onItemClick: (String, String) -> Unit, onDeleteClick: (String, String) -> Unit) {
    val context = LocalContext.current
    var showDialog by remember { mutableStateOf(false) }
    var bitmap by remember { mutableStateOf<Bitmap?>(null) }

//    LaunchedEffect(filename) {
//        bitmap = loadBitmapFromFile(context.filesDir.absolutePath + File.separator + filename)
//    }

    Card(
        modifier = Modifier
            .padding(vertical = 10.dp, horizontal = 32.dp)
            .aspectRatio(1f)
            .clickable { onItemClick(filename, title) },
        elevation = CardDefaults.cardElevation(defaultElevation = 6.dp) ,
        colors = CardDefaults.cardColors(containerColor = Color(0xFFd4e9fa))
        )
     {
            Row(
                modifier = Modifier.fillMaxWidth(),
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = title,
                    fontSize = 16.sp,
                    fontFamily = MaterialTheme.typography.bodyMedium.fontFamily,
                    overflow = TextOverflow.Ellipsis,
                    maxLines = 1,
                    modifier = Modifier
                        .weight(1f)
                        .padding(start = 16.dp, end = 8.dp)
                )
                IconButton(
                    onClick = { showDialog = true },
                    modifier = Modifier.size(56.dp)
                ) {
                    Icon(Icons.Default.Delete, contentDescription = "Delete")
                }
            }
        Column(
            modifier = Modifier.padding(8.dp),
            verticalArrangement = Arrangement.Top,
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            bitmap?.let {
                Image(
                    bitmap = it.asImageBitmap(),
                    contentDescription = "Drawing Preview",
                    modifier = Modifier
                        .fillMaxHeight()
                        .fillMaxWidth()
                )
                Spacer(modifier = Modifier.height(8.dp))
            }
        }
    }

    // Dialog for deletion confirmation
    if (showDialog) {
        AlertDialog(
            onDismissRequest = { showDialog = false },
            title = { Text("Confirm Deletion") },
            text = { Text("Are you sure you want to delete this item?") },
            confirmButton = {
                TextButton(
                    onClick = {
                        onDeleteClick(filename, title)
                        showDialog = false
                    }
                ) {
                    Text("Delete")
                }
            },
            dismissButton = {
                TextButton(
                    onClick = { showDialog = false }
                ) {
                    Text("Cancel")
                }
            }
        )
    }

}

//Compose method to create UI for the add button
@Composable
fun AddButton(onClick: () -> Unit){
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .padding(10.dp),
        horizontalArrangement = Arrangement.End
    ) {
        Spacer(modifier = Modifier.weight(3f))  // Takes up the left half of the row
        Button(
            onClick = onClick,
            colors = ButtonDefaults.buttonColors(containerColor = Color(0xFF011C28)),
            modifier = Modifier.weight(1f)  // Button takes up the right half of the row
        ) {
            Text(text = "+ New")
        }
    }
}