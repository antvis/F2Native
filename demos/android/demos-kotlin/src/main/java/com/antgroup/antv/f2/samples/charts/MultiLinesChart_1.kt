package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 多组折线图
 * @author qingyuan.yl
 * @date 2020-09-25
 */
class MultiLinesChart_1 : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "MultiLinesChart",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            canvasView.setOnCanvasTouchListener { canvasView, event -> mChart.postTouchEvent(event) }
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_multilines.json"))
        mChart!!.line().position("date*value").color("type").fixedSize(1f).fixedShape("smooth")
        mChart!!.setAxis(
            "date", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
        )
        mChart!!.setScale("date", ScaleConfigBuilder().tickCount(5))
        mChart!!.setScale("value", ScaleConfigBuilder().nice(true))
        mChart!!.tooltip(ToolTipConfigBuilder())
        mChart!!.interaction("pan")
        mChart!!.interaction("pinch")
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}