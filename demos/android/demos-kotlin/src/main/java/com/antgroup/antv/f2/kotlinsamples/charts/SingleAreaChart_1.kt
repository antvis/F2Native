package com.antgroup.antv.f2.kotlinsamples.charts

import com.alibaba.fastjson.JSON
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.F2Function
import com.antgroup.antv.f2.F2Util.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleAreaChart_1 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleAreaChart_1",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleAreaChart.json"))
        mChart!!.setAxis(
            "time", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
                .gridHidden()
        )
        mChart!!.setAxis(
            "tem", AxisConfigBuilder()
                .grid(AxisGridConfigBuilder().type("dash"))
        )
        mChart!!.setScale("tem", ScaleConfigBuilder().min(800.0).max(3000.0))
        mChart!!.line().position("time*tem")
        mChart!!.area().position("time*tem").fixedColor(
            ColorLinearGradient()
                .addColorStop(0f, "red")
                .addColorStop(1f, "white")
                .setPosition(0f, 0f, 0f, canvasView.height.toFloat())
        )
        mChart!!.point().position("time*tem").fixedColor("#FFB6C1").style(
            F2Config.Builder<F2Config.Builder<*>>()
                .setOption("custom", mChart, object : F2Function() {
                    override fun execute(param: String): F2Config {
                        val jsonObject = JSON.parseObject(param)
                        return if (jsonObject.getIntValue("_index") % 2 == 0) {
                            F2Config.Builder<F2Config.Builder<*>>().setOption("size", 0).build()
                        } else F2Config.Builder<F2Config.Builder<*>>().build()
                    }
                })
                .build()
        )
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}