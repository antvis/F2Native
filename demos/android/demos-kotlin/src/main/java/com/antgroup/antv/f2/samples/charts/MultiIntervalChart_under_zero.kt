package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 分组柱状图(带负值)
 * @author qingyuan.yl
 * @date 2020-12-22
 */
class MultiIntervalChart_under_zero : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "MultiIntervalChart_under_zero",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(20.0, 10.0, 10.0, 10.0)
        mChart!!.source(
            loadAssetFile(
                canvasView.context,
                "mockData_singleIntervalChart_under_zero.json"
            )
        )
        mChart!!.setAxis(
            "tem", AxisConfigBuilder().grid(
                AxisGridConfigBuilder().stroke("#000")
            )
        )
        mChart!!.interval().position("time*tem").color("city").adjust("dodge")
        mChart!!.setScale("tem", ScaleConfigBuilder().nice(true))
        mChart!!.setScale("time", ScaleConfigBuilder().range(doubleArrayOf(0.1, 0.9)))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}