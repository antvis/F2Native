package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.ScaleConfigBuilder
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 分组柱状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class MultiIntervalChart : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "MultiIntervalChart",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_multiIntervalsChart.json"))
        mChart!!.interval().position("月份*月均降雨量").color("name").adjust("dodge")
        mChart!!.setScale("月份", ScaleConfigBuilder().range(doubleArrayOf(0.065, 0.935)))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}