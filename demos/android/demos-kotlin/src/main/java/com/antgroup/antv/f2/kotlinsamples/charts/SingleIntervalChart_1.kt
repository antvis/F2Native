package com.antgroup.antv.f2.kotlinsamples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * 基础柱状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleIntervalChart_1 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleIntervalChart_1",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            canvasView.setOnCanvasTouchListener { canvasView, event -> mChart!!.postTouchEvent(event) }
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleIntervalChart.json"))
        mChart!!.interval().position("year*sales").style(
            F2Config.Builder<F2Config.Builder<*>>()
                .setOption("radius", floatArrayOf(3f, 3f, 0f, 0f)) // [tl, tr, bl, br]
                .build()
        )
        mChart!!.setAxis(
            "year", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
        )
        mChart!!.setAxis(
            "sales", AxisConfigBuilder()
                .grid(AxisGridConfigBuilder().type("dash"))
        )
        mChart!!.setScale(
            "year",
            ScaleConfigBuilder().tickCount(5).range(doubleArrayOf(0.05, 0.95))
        )
        mChart!!.setScale("sales", ScaleConfigBuilder().min(15.0).max(50.0).tickCount(5))
        mChart!!.interaction(
            "pinch",
            F2Config.Builder<F2Config.Builder<*>>().setOption("maxCount", 80).setOption("minCount", 10).build()
        ).interaction("pan")
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}