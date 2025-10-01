package com.antgroup.antv.f2.kotlinsamples

import android.os.Bundle
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Log
import com.antgroup.antv.f2.kotlinsamples.MainActivity.Companion.chartModels
import java.lang.ref.WeakReference
import java.util.*

class ChartListActivity : AppCompatActivity() {
    private var listView: RecyclerView? = null
    private val canvasViews: MutableList<WeakReference<F2CanvasView?>> = ArrayList()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.setDisplayHomeAsUpEnabled(true)
        setContentView(R.layout.activity_main)
        listView = findViewById(R.id.listView) as RecyclerView
        listView!!.setLayoutManager(LinearLayoutManager(this))
        listView!!.setAdapter(CanvasViewAdapter(chartModels, canvasViews))
    }

    override fun onDestroy() {
        super.onDestroy()
        F2Log.get().i("ChartListActivity", "canvasViews Size: " + canvasViews.size)
        for (canvasView in canvasViews) {
            if (canvasView.get() != null) {
                canvasView.get()!!.destroy()
            }
        }
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == android.R.id.home) {
            finish()
        }
        return super.onOptionsItemSelected(item)
    }

    internal class CanvasViewHolder(
        itemView: View,
        canvasViews: MutableList<WeakReference<F2CanvasView?>>
    ) : ViewHolder(itemView) {
        var chartName: TextView
        var canvasView: F2CanvasView

        init {
            chartName = itemView.findViewById(R.id.chart_name)
            canvasView = itemView.findViewById(R.id.canvasView)
            canvasView.setCanvasBizId("ChartListActivity")
            canvasViews.add(WeakReference(canvasView))
        }
    }

    internal class CanvasViewAdapter(
        private val mChartModels: List<ChartModel>,
        private val mCanvasViews: MutableList<WeakReference<F2CanvasView?>>
    ) : RecyclerView.Adapter<CanvasViewHolder>() {
        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): CanvasViewHolder {
            val itemView = LayoutInflater.from(parent.context)
                .inflate(R.layout.layout_chart_canvasview, parent, false)
            return CanvasViewHolder(itemView, mCanvasViews)
        }

        override fun onBindViewHolder(holder: CanvasViewHolder, position: Int) {
            val chartModel = mChartModels[position]
            holder.chartName.text = chartModel.title
            try {
                val adapter = chartModel.adapterClass.newInstance()!!
                holder.canvasView.setAdapter(adapter)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }

        override fun getItemCount(): Int {
            return mChartModels.size
        }
    }
}