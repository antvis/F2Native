package com.antgroup.antv.f2.samples.charts

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Log
import com.alibaba.fastjson.JSON
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2CanvasView.OnCanvasTouchListener
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.F2Function
import com.antgroup.antv.f2.samples.Utils.loadAssetFile
import java.io.InputStream
import java.text.DateFormat
import java.util.*

/**
 * 股票日K趋势图
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleLineChart_KLine : F2CanvasView.Adapter, OnCanvasTouchListener {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleLineChart_KLine",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            canvasView.setOnCanvasTouchListener(this)
        }
        mChart!!.clear()
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 30.0, 10.0, 20.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_line_kline.json"))
        mChart!!.line().position("date*price")
        val scaleConfigBuilder = ScaleConfigBuilder()
        scaleConfigBuilder
            .tickCount(5)
            .type("kline-day")
            .setOption("timeZoneOffset", 28800)
            .setOption("domain", doubleArrayOf(1.0, 20.0))
        mChart!!.setScale("date", scaleConfigBuilder)
        mChart!!.setAxis("date", AxisConfigBuilder()
            .label(AxisLabelConfigBuilder().item(mChart, object : F2Function() {
                override fun execute(paramStr: String): F2Config {
                    val jsonObject = JSON.parseObject(paramStr)
                    val param = jsonObject.getString("content")
                    val timestamp = param.toLong()
                    val content = DateFormat.getDateInstance().format(Date(timestamp))
                    return F2Config.Builder<Any?>().setOption("content", content).build()
                }
            }))
        )
        mChart!!.setScale(
            "price", ScaleConfigBuilder()
                .tickCount(2).nice(true)
        )
        mChart!!.tooltip(
            ToolTipConfigBuilder()
                .setOption("yTip", F2Config.Builder<Any?>().setOption("inner", true))
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