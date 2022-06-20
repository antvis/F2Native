package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 雷达面积图
 */
class RadarAreaChart : F2CanvasView.Adapter {
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
        mChart!!.padding(10.0, 5.0, 20.0, 10.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_radarArea.json"))
        mChart!!.line().position("item*score").color("user")
        mChart!!.area().position("item*score").color("user")
        mChart!!.point().position("item*score").color("user")
            .style(
                F2Config.Builder<Any?>().setOption("lineWidth", 3).setOption("stroke", "#fff")
                    .setOption("lineWidth", 1).build()
            )
        mChart!!.setCoord(CoordConfigBuilder().type("polar"))
        mChart!!.setScale(
            "score",
            ScaleConfigBuilder().min(0.0).max(120.0).nice(false).tickCount(4)
        )
        mChart!!.setAxis(
            "score", AxisConfigBuilder()
                .grid(
                    AxisGridConfigBuilder().type("arc").stroke("#E8E8E8FF")
                        .lineDash(doubleArrayOf(4.0, 2.0))
                )
        )
        mChart!!.setAxis(
            "item", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(30f))
                .grid(
                    AxisGridConfigBuilder().type("dash").stroke("#E8E8E8FF")
                        .lineDash(doubleArrayOf(4.0, 2.0))
                )
        )
        mChart!!.legend("user", LegendConfigBuild().position("top"))
        mChart!!.animate(
            ChartAnimateConfigBuild().geom(
                "line", GeomAnimateConfigBuild().animateType("GroupScaleInXY")
            )
        ) // GroupWaveIn GroupScaleInXY GroupScaleInX GroupScaleInY
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}