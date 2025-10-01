package com.antgroup.antv.f2.kotlinsamples

import android.os.Bundle
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import com.antgroup.antv.f2.F2CanvasView

class SampleActivityHorizontal : AppCompatActivity() {
    private var mCanvasView: F2CanvasView? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        supportActionBar!!.setDisplayHomeAsUpEnabled(true)
        setContentView(R.layout.activity_sample_horizontal)
        mCanvasView = findViewById(R.id.canvasView)
        mCanvasView!!.setCanvasBizId("F2NativeDemo")
        val chartModel = intent.getSerializableExtra("ChartModel") as ChartModel
        title = chartModel.title
        try {
            val adapter = chartModel.adapterClass.newInstance()
            mCanvasView!!.setAdapter(adapter)
        } catch (e: IllegalAccessException) {
            e.printStackTrace()
        } catch (e: InstantiationException) {
            e.printStackTrace()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        mCanvasView!!.destroy()
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == android.R.id.home) {
            finish()
        }
        return super.onOptionsItemSelected(item)
    }
}