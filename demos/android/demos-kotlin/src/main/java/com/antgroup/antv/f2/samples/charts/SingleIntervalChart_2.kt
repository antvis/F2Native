package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.LegendConfigBuild
import com.antgroup.antv.f2.F2Chart.ScaleConfigBuilder
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 基础柱状图-2
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleIntervalChart_2 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleIntervalChart_2",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(20.0, 10.0, 10.0, 10.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_singleIntervalChart_2.json"))
        mChart!!.interval().position("genre*sold").color("genre")
        mChart!!.setScale("sold", ScaleConfigBuilder().min(0.0))
        mChart!!.setScale("genre", ScaleConfigBuilder().range(doubleArrayOf(0.1, 0.9)))
        mChart!!.legend(
            "genre",
            LegendConfigBuild().enable(true).position("top").symbol("circle").setOption("radius", 3)
        )
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}