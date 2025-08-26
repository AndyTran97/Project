package com.example.dragon_descendants

import android.annotation.SuppressLint
import android.app.AlertDialog
import android.graphics.Color
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import com.example.dragon_descendants.databinding.FragmentDrawPanelBinding
import com.skydoves.colorpickerview.ColorPickerDialog
import com.skydoves.colorpickerview.listeners.ColorEnvelopeListener
import android.graphics.Path
import android.graphics.drawable.ColorDrawable
import android.graphics.drawable.Drawable
import android.graphics.drawable.GradientDrawable
import android.text.InputType
import android.widget.EditText
import android.widget.PopupMenu
import android.widget.SeekBar
import androidx.fragment.app.activityViewModels
import android.content.Intent
import androidx.core.content.FileProvider
import androidx.fragment.app.viewModels
import androidx.navigation.fragment.findNavController
import java.io.File



class DrawPanelFragment : Fragment() {
    private lateinit var binding: FragmentDrawPanelBinding
    //private val viewModel : DrawViewModel by activityViewModels()
//    val viewModel: DrawViewModel by activityViewModels {
//        DrawViewModel.DrawViewModelFactory((requireActivity().application as DrawingApplication).drawingRepository)
//    }
    private val viewModel: DrawViewModel by activityViewModels()
    private val path: Path = Path()

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentDrawPanelBinding.inflate(inflater)

        //Need Refactor
        binding.textView.text = viewModel.titleVM
        binding.textView.setOnClickListener {
            showEditNameDialog()
        }
        setupToolButtons()
        setupColorPicker()
        setupSeekBarListener()

        viewModel.observableBM.observe(viewLifecycleOwner){ bitmap ->
            binding.drawView.updateBitmap(bitmap, viewModel.tempBitmap)
        }
        binding.drawView.setOnTouchListener { _, event ->
            handleTouchEvent(event)
        }

        binding.settingsBtn.setOnClickListener {
            showOptionsMenu(it)
        }


        return binding.root
    }

    private fun showEditNameDialog() {
        val editText = EditText(requireContext()).apply {
            inputType = InputType.TYPE_CLASS_TEXT
            setText(binding.textView.text)
        }

        AlertDialog.Builder(requireContext())
            .setTitle("Edit Name")
            .setView(editText)
            .setPositiveButton("OK") { _, _ ->
                // Update the TextView with new text
                binding.textView.text = editText.text.toString()
                // Optionally: Update viewModel.titleVM if you need to persist this data
                viewModel.titleVM = editText.text.toString()

                //Update with the new Title
                viewModel.updateDrawing()
            }
            .setNegativeButton("Cancel", null)
            .create()
            .show()
    }



    private fun setupColorPicker() {
        binding.colorBtn.setOnClickListener(){
            val colorPickerDialog = ColorPickerDialog.Builder(requireContext())
                .setTitle("ColorPicker Dialog")
                .setPreferenceName("MyColorPickerDialog")
                .setPositiveButton(getString(R.string.confirm),
                    ColorEnvelopeListener { envelope, _ ->
                        viewModel.setPaintColor(envelope.color)
                    })
                .setNegativeButton(getString(R.string.cancel)) { dialogInterface, _ ->
                    dialogInterface.dismiss()
                }
                .attachAlphaSlideBar(true) // the default value is true.
                .attachBrightnessSlideBar(true)  // the default value is true.
                .setBottomSpace(12) // set a bottom space between the last slidebar and buttons.
                .show()
        }
    }

    private fun createBorderDrawable(borderColor: Int, borderWidth: Int): Drawable {
        val drawable = GradientDrawable()
        drawable.shape = GradientDrawable.RECTANGLE
        drawable.setStroke(borderWidth, borderColor) // Set border width and color
        drawable.setColor(Color.TRANSPARENT) // Transparent background for the button
        return drawable
    }
    private fun setButtonState(button: View, isSelected: Boolean, normalBg: Drawable?, selectedBg: Drawable?) {
        button.isSelected = isSelected
        button.background = if (isSelected) selectedBg else normalBg
    }
    private fun updateSingleSelection(selectedButton: View, normalBg: Drawable?, selectedBg: Drawable?) {
        val buttons = listOf(binding.eraserBtn, binding.penBtn, binding.shapeBtn)

        buttons.forEach { button ->
            setButtonState(button, button == selectedButton, normalBg, selectedBg)
        }
    }
    private fun setupToolButtons() {
        val normalBackground = ColorDrawable(Color.TRANSPARENT)
        val selectedBackground = createBorderDrawable(Color.GREEN, 3)
        updateSingleSelection(binding.penBtn, normalBackground, selectedBackground)

        binding.eraserBtn.setOnClickListener {
            updateSingleSelection(binding.eraserBtn, normalBackground, selectedBackground)
            viewModel.selectTool(DrawingTool.Eraser)
        }

        binding.penBtn.setOnClickListener {
            updateSingleSelection(binding.penBtn, normalBackground, selectedBackground)
            viewModel.selectTool(DrawingTool.Pencil)
        }

        binding.shapeBtn.setOnClickListener {
            showShapeMenu(it)
            updateSingleSelection(binding.shapeBtn, normalBackground, selectedBackground)
            viewModel.selectTool(DrawingTool.Shape)
        }
    }

    //Stroke SeekBar
    private fun showSeekBar(visible: Boolean) {
        binding.sizeSeekBar.visibility = if (visible) View.VISIBLE else View.GONE
        if (visible) {
            // Update stroke width based on current seekbar progress
            viewModel.setStrokeWidth(binding.sizeSeekBar.progress.toFloat())
            setupSeekBarListener()
        }
    }

    private fun setupSeekBarListener() {
        binding.sizeSeekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                viewModel.setStrokeWidth(progress.toFloat())
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
                // Optional: Implement if needed
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                // Optional: Implement if needed
            }
        })
    }

    private fun showShapeMenu(anchor: View) {
        val popup = PopupMenu(requireContext(), anchor) // Use the anchor view here
        popup.menuInflater.inflate(R.menu.shape_menu, popup.menu)
        popup.setOnMenuItemClickListener { item ->
            when (item.itemId) {
                R.id.shape_circle -> viewModel.selectTool(DrawingTool.Circle)
                R.id.shape_rectangle -> viewModel.selectTool(DrawingTool.Rectangle)
                R.id.shape_triangle -> viewModel.selectTool(DrawingTool.Triangle)
            }
            true
        }
        popup.show()
    }
    private fun handleTouchEvent(event: MotionEvent?): Boolean {
        val x: Float = event!!.x
        val y: Float = event.y
        val viewX = x * (viewModel.observableBM.value!!.width.toFloat() / binding.drawView.width)
        val viewY = y * (viewModel.observableBM.value!!.height.toFloat() / binding.drawView.height)

        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                if (viewModel.selectedTool == DrawingTool.Pencil || viewModel.selectedTool == DrawingTool.Eraser) {
                    path.moveTo(viewX, viewY)
                }
                else{
                    //shape drawing
                    viewModel.setStartPoint(viewX, viewY)
                }
                return true
            }
            MotionEvent.ACTION_MOVE -> {
                if (viewModel.selectedTool == DrawingTool.Pencil || viewModel.selectedTool == DrawingTool.Eraser) {
                    path.lineTo(viewX, viewY)
                    viewModel.drawPath(path)
                } else {
                    // For shape tools, update end coordinates continuously
                    viewModel.setEndPoint(viewX, viewY)
                    viewModel.drawShapePreview()
                }

            }
            MotionEvent.ACTION_UP -> {
                // Finalize shape drawing
                if (viewModel.selectedTool in setOf(DrawingTool.Circle, DrawingTool.Rectangle, DrawingTool.Triangle)) {
                    viewModel.setEndPoint(viewX, viewY)
                    viewModel.finalizeShapeDrawing()
                }
                path.reset()
            }
            else -> return false
        }

        binding.drawView.invalidate()

        return true
    }

    private fun showOptionsMenu(anchor: View){
        val popup = PopupMenu(requireContext(), anchor)
        popup.menuInflater.inflate(R.menu.options_menu, popup.menu)
        popup.setOnMenuItemClickListener { item ->
            when (item.itemId){
                //Action for Save button
                R.id.action_save -> {
                    viewModel.updateDrawing()
                }
                //Action for Share button
                R.id.action_share -> {
                    ShareImage()
                }
                //Action for Back button
                R.id.action_back -> {
                    viewModel.updateDrawing()
                    findNavController().navigate(R.id.action_drawPanelFragment2_to_drawingListFragment2)
                }
                //Action for Invert Color Button
                R.id.action_invert_color -> {
                    viewModel.InvertColor()
                }
                //Action for Blur Image Button
                R.id.action_noise -> {
                    viewModel.AddNoise()
                }

            }
            true
        }
        popup.show()
    }

    //Share images feature
    private fun ShareImage(){
        val share = Intent(Intent.ACTION_SEND)
        share.setType("image/png")
        val file = File(context?.filesDir, "${viewModel.getFileName()}.png")
        val bmpUri = FileProvider.getUriForFile(requireContext(), "${context?.packageName}.provider",file)
        share.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK)
        share.putExtra(Intent.EXTRA_STREAM, bmpUri)
        share.putExtra(Intent.EXTRA_SUBJECT, "New App")
        startActivity(Intent.createChooser(share, "Share Content"))
    }


    override fun onPause() {
        super.onPause()
        viewModel.updateDrawing()
    }

}