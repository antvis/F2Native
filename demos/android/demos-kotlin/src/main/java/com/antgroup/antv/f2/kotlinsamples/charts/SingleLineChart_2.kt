package com.antgroup.antv.f2.kotlinsamples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * 基础折线图(双Y轴)
 * @author qingyuan.yl
 */
class SingleLineChart_2 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleLineChart_2",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 10.0, 10.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_multiAxiesLine.json"))
        mChart!!.line().position("date*value").fixedColor("#1890FF")
        mChart!!.line().position("date*offset").fixedColor("#2FC25B")
        mChart!!.setAxis(
            "date", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
                .gridHidden()
        )
        mChart!!.setAxis(
            "value", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
        )
        mChart!!.setAxis(
            "offset", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
        )
        mChart!!.setScale("date", ScaleConfigBuilder().tickCount(3))
        mChart!!.setScale("value", ScaleConfigBuilder().nice(true))
        mChart!!.setScale("offset", ScaleConfigBuilder().nice(true))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}