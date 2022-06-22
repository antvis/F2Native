package com.antgroup.antv.f2.kotlinsamples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.kotlinsamples.Utils.loadAssetFile

/**
 * 基础条状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleIntervalChart_3 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleIntervalChart_3",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleIntervalChart.json"))
        mChart!!.interval().position("year*sales")
        mChart!!.setAxis(
            "year", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
                .gridHidden()
        )
        mChart!!.setCoord(CoordConfigBuilder().type("rect").transposed(true))
        mChart!!.setAxis(
            "sales", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
                .grid(AxisGridConfigBuilder().type("dash"))
                .line(AxisLineConfigBuilder().lineWidth(1f).color("#E6E6E6").type("dash"))
        )
        mChart!!.setScale("year", ScaleConfigBuilder().range(doubleArrayOf(0.1, 0.9)))
        mChart!!.setScale("sales", ScaleConfigBuilder().nice(true))
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}