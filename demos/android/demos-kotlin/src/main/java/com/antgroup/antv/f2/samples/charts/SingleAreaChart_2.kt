package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleAreaChart_2 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleAreaChart_2",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleAreaChart_2.json"))
        mChart!!.line().position("month*value")
        mChart!!.area().position("month*value")
        mChart!!.setAxis(
            "month", AxisConfigBuilder()
                .label(
                    AxisLabelConfigBuilder()
                        .labelMargin(10f)
                        .labelOffset(5f)
                )
                .lineHidden()
                .gridHidden()
        )
        mChart!!.setAxis(
            "value", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelMargin(10f))
                .grid(AxisGridConfigBuilder().type("dash"))
        )
        mChart!!.setScale("value", ScaleConfigBuilder().nice(true))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}