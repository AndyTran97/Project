package com.example.dragon_descendants

import android.graphics.Bitmap
import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.example.dragon_descendants.databinding.DrawingItemBinding

class DrawingListAdapter(private var drawings: List<Drawing>, private val onclick:(item: Drawing) -> Unit):RecyclerView.Adapter<DrawingListAdapter.ViewHolder>(){


    inner class ViewHolder(val binding: DrawingItemBinding): RecyclerView.ViewHolder(binding.root)

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        return ViewHolder(DrawingItemBinding.inflate(LayoutInflater.from(parent.context), parent, false))
    }

    override fun getItemCount(): Int = drawings.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
//        val drawing = drawings[position]
//        holder.binding.drawingFileBtn.text = drawing.title
//        holder.binding.drawingFileBtn.setOnClickListener(){
//            onclick(drawing)
//        }
    }

    fun updateData(newDrawings: List<Drawing>) {
        drawings = newDrawings
        notifyDataSetChanged()
    }


}