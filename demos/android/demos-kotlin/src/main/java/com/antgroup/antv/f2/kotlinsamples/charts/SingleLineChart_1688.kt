package com.antgroup.antv.f2.kotlinsamples.charts

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Log
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2CanvasView.OnCanvasTouchListener
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile
import java.io.InputStream

/**
 * 基础折线图-1
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleLineChart_1688 : F2CanvasView.Adapter, OnCanvasTouchListener {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleLineChart_1",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            canvasView.setOnCanvasTouchListener(this)
        }
        mChart!!.clear()
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 30.0, 10.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_1688.json"))
        mChart!!.interaction("pinch")
        mChart!!.line().fixedShape("smooth").fixedColor("#FF2900").position("date*value")
        mChart!!.area().position("date*value").fixedShape("smooth").fixedColor("#FFEDE9")
        mChart!!.point().position("date*value").fixedColor("#FF2900")
        mChart!!.setAxis(
            "date",
            AxisConfigBuilder()
                .line(AxisLineConfigBuilder().lineWidth(1f).color("#DCDEE3"))
                .label(
                    AxisLabelConfigBuilder()
                        .labelOffset(8.0f)
                        .labelMargin(8.0f)
                )
                .gridHidden()
        )
        //        mChart.setAxis("value",
//                new F2Chart.AxisConfigBuilder()
//                        .hidden());
        mChart!!.setScale(
            "date",
            ScaleConfigBuilder()
                .setOption("nice", true)
                .tickCount(6)
        )
        mChart!!.setScale(
            "value",
            ScaleConfigBuilder()
                .setOption("nice", true)
                .tickCount(3)
        )
        mChart!!.render()
    }

    override fun onTouch(canvasView: F2CanvasView, event: F2CanvasView.TouchEvent) {
        if (mChart != null && mChart!!.postTouchEvent(event)) {
        }
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }

    companion object {
        private fun getImageFromAssets(context: Context, filePath: String): Bitmap? {
            var bitmap: Bitmap? = null
            var ins: InputStream? = null
            try {
                val assetManager = context.assets
                ins = assetManager.open(filePath)
                bitmap = BitmapFactory.decodeStream(ins)
                ins.close()
            } catch (t: Throwable) {
                Log.e("SingleLineChart1", "getImageFromAssets error:$t")
            } finally {
                if (ins != null) {
                    try {
                        ins.close()
                    } catch (e: Throwable) {
                        Log.e("SingleLineChart1", "getImageFromAssets close error:$e")
                    }
                }
            }
            return bitmap
        }
    }
}