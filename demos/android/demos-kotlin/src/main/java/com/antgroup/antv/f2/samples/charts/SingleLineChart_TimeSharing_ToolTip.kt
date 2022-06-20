package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.samples.Utils.loadAssetFile
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2CanvasView.OnCanvasTouchListener
import com.antgroup.antv.f2.F2Chart.ScaleConfigBuilder
import com.antgroup.antv.f2.F2Chart.ToolTipConfigBuilder
import org.json.JSONArray

/**
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class SingleLineChart_TimeSharing_ToolTip : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "SingleIntervalChart_demo",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            canvasView.setOnCanvasTouchListener { canvasView, event -> mChart.postTouchEvent(event) }
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 0.0, 0.0, 0.0)
        mChart!!.source(
            loadAssetFile(
                canvasView.context,
                "mockData_singleLineChart_timeSharing_tooltip.json"
            )
        )
        mChart!!.line().position("date*mainNetIn").fixedColor("#1677FF").fixedSize(1.5f)
        mChart!!.setScale(
            "mainNetIn", ScaleConfigBuilder()
                .type("linear").tickCount(5).nice(true).precision(2)
        )
        val timeRange = JSONArray()
        val item1 = JSONArray()
        item1.put(1618882200000L)
        item1.put(1618889400000L)
        timeRange.put(item1)
        val item2 = JSONArray()
        item2.put(1618894800000L)
        item2.put(1618907400000L)
        timeRange.put(item2)
        mChart!!.setScale(
            "date", ScaleConfigBuilder()
                .type("timeSharing").setOption("timeRange", timeRange)
        )
        mChart!!.tooltip(ToolTipConfigBuilder())
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}