package com.antgroup.antv.f2.samples.charts

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.util.Log
import com.alibaba.fastjson.JSON
import com.antgroup.antv.f2.*
import com.antgroup.antv.f2.F2CanvasView.OnCanvasTouchListener
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.samples.Utils.loadAssetFile
import java.io.InputStream

/**
 * 基础折线图
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleLineChart_1 : F2CanvasView.Adapter, OnCanvasTouchListener {
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
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleLineChart.json"))
        mChart!!.interaction("pinch")
        mChart!!.interaction("pan")
        mChart!!.tooltip(ToolTipConfigBuilder()
            .setOption("onPressStart", mChart, object : F2Function() {
                override fun execute(param: String): F2Config {
                    F2Log.i("tooltip", "onPressStart: $param")
                    return F2Config.Builder<Any?>().build()
                }
            })
            .setOption("onPressEnd", mChart, object : F2Function() {
                override fun execute(param: String): F2Config {
                    F2Log.i("tooltip", "onPressEnd: $param")
                    return F2Config.Builder<Any?>().build()
                }
            })
            .setOption("onPress", mChart, object : F2Function() {
                override fun execute(param: String): F2Config {
                    return F2Config.Builder<Any?>().build()
                }
            })
        )
        mChart!!.animate(true)
        mChart!!.line().position("date*value")
            .attrs(F2Config.Builder<Any?>().setOption("connectNulls", true).build())
        mChart!!.setAxis(
            "date", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f).item(mChart, object : F2Function() {
                    override fun execute(paramStr: String): F2Config {
                        val jsonObject = JSON.parseObject(paramStr)
                        val param = jsonObject.getString("content")
                        val arr = param.split("-").toTypedArray()
                        val rst: String
                        rst = if (arr.size == 3) {
                            arr[1] + "-" + arr[2]
                        } else {
                            param
                        }
                        val index = jsonObject.getIntValue("index")
                        return F2Config.Builder<Any?>()
                            .setOption("textColor", if (index % 2 == 0) "#000000" else "#DC143C")
                            .setOption("xOffset", if (index % 2 == 0) 0 else -30)
                            .setOption("content", rst)
                            .build()
                    }
                }))
                .gridHidden()
        )
        mChart!!.setAxis(
            "value", AxisConfigBuilder()
                .grid(
                    AxisGridConfigBuilder()
                        .type("dash")
                        .lineDash(doubleArrayOf(8.0, 3.0, 3.0, 8.0))
                )
        )
        mChart!!.setScale("date", ScaleConfigBuilder().tickCount(5))
        mChart!!.setScale("value", ScaleConfigBuilder().setOption("nice", true))
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