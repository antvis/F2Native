package com.antgroup.antv.f2.kotlinsamples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SinglePointChart_1 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SinglePointChart_1",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singlePointChart.json"))
        mChart!!.setAxis(
            "x", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
                .gridHidden()
        )
        mChart!!.setAxis(
            "y", AxisConfigBuilder()
                .grid(AxisGridConfigBuilder().type("dash"))
        )
        mChart!!.setScale("y", ScaleConfigBuilder().nice(true))
        mChart!!.point().position("x*y").size("z", floatArrayOf(3f, 10f))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}