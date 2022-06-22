package com.antgroup.antv.f2.kotlinsamples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * 区间柱状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SectionIntervalChart : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SectionIntervalChart",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            canvasView.setOnCanvasTouchListener { canvasView, event -> mChart!!.postTouchEvent(event) }
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_sectionInterval.json"))
        mChart!!.interval().position("x*y")
        mChart!!.setAxis(
            "x", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
                .grid(AxisGridConfigBuilder().stroke("#000").lineWidth(1f))
        )
        mChart!!.setAxis(
            "y", AxisConfigBuilder()
                .grid(AxisGridConfigBuilder().type("dash"))
        )
        mChart!!.setScale("x", ScaleConfigBuilder().range(doubleArrayOf(0.1, 0.9)))
        mChart!!.setScale("y", ScaleConfigBuilder().nice(true))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}