package com.antgroup.antv.f2.kotlinsamples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * 层叠面积图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class MultiAreasChart : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "MultiAreasChart",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_multiAreasChart.json"))
        mChart!!.area().position("date*value").color("city").adjust("stack")
        mChart!!.line().position("date*value").color("city").adjust("stack")
        mChart!!.setAxis(
            "date", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
        )
        mChart!!.setScale("date", ScaleConfigBuilder().tickCount(5))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}