package com.antgroup.antv.f2.samples.charts

import com.alibaba.fastjson.JSON
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2CanvasView.OnCanvasTouchListener
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.F2Function
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 蜡烛图(日K)
 * @author qingyuan.yl
 */
class CandleChart_klineDay : F2CanvasView.Adapter, OnCanvasTouchListener {
    private var mCandleChart: F2Chart? = null
    private var mSubChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mCandleChart == null) {
            canvasView.setOnCanvasTouchListener(this)
            mCandleChart = F2Chart.create(
                canvasView.context,
                "KlineDay",
                canvasView.width.toDouble(),
                (canvasView.height * 3 / 4).toDouble()
            )
            mSubChart = F2Chart.create(
                canvasView.context,
                "KlineDay-Sub",
                canvasView.width.toDouble(),
                (canvasView.height / 4).toDouble()
            )
        }
        mCandleChart!!.setCanvas(canvasView)
        mCandleChart!!.padding(15.0, 10.0, 0.0, 0.0)
        mCandleChart!!.source(loadAssetFile(canvasView.context, "mockData_klineDay.json"))
        mCandleChart!!.candle().position("date*values")
        mCandleChart!!.line().position("date*m5").fixedColor("#A46FFE")
        mCandleChart!!.line().position("date*m10").fixedColor("#46ACFF")
        mCandleChart!!.line().position("date*m20").fixedColor("#FF8C53")
        mCandleChart!!.setAxis("m5", AxisConfigBuilder().hidden())
        mCandleChart!!.setAxis("m10", AxisConfigBuilder().hidden())
        mCandleChart!!.setAxis("m20", AxisConfigBuilder().hidden())
        mCandleChart!!.setAxis(
            "date", AxisConfigBuilder()
                .label(AxisLabelConfigBuilder().labelOffset(5f))
        )
        mCandleChart!!.setScale(
            "date",
            ScaleConfigBuilder().type("kline-day").setOption("domain", doubleArrayOf(40.0, 80.0))
        )
        mCandleChart!!.setScale("values", ScaleConfigBuilder().nice(true))
        mCandleChart!!.setScale(
            "m5",
            ScaleConfigBuilder().setOption("assign", "values")
        ) // 复用 values 字段的度量
        mCandleChart!!.setScale(
            "m10",
            ScaleConfigBuilder().setOption("assign", "values")
        ) // 复用 values 字段的度量
        mCandleChart!!.setScale(
            "m20",
            ScaleConfigBuilder().setOption("assign", "values")
        ) // 复用 values 字段的度量
        mCandleChart!!.interaction("pan")
        mCandleChart!!.interaction("pinch")
        mCandleChart!!.tooltip(
            ToolTipConfigBuilder().setOption(
                "onPress",
                mCandleChart,
                object : F2Function() {
                    override fun execute(param: String): F2Config {
                        drawMAIndicators(canvasView, param)
                        return null
                    }
                })
        )
        mCandleChart!!.render()

        // sub chart
        mSubChart!!.setCanvas(canvasView)
        mSubChart!!.margin(
            0.0,
            (canvasView.height * 3 / 4 / canvasView.resources.displayMetrics.density).toDouble(),
            0.0,
            0.0
        )
        mSubChart!!.padding(12.0, 0.0, 0.0, 20.0)
        mSubChart!!.source(loadAssetFile(canvasView.context, "mockData_klineDay.json"))
        mSubChart!!.setAxis("date", AxisConfigBuilder().hidden())
        mSubChart!!.interval().position("date*volumn")
            .style(F2Config.Builder<Any?>().setOption("widthRatio", 0.9).build())
        mSubChart!!.setScale(
            "date",
            ScaleConfigBuilder().type("kline-day").setOption("domain", doubleArrayOf(40.0, 80.0))
        )
        mSubChart!!.setAxis("volumn", AxisConfigBuilder()
            .label(AxisLabelConfigBuilder().item(mSubChart, object : F2Function() {
                override fun execute(param: String): F2Config {
                    return F2Config.Builder<Any?>().setOption("content", "1亿手").build()
                }
            }))
        )
        mSubChart!!.setScale("volumn", ScaleConfigBuilder().nice(true).tickCount(2))
        mSubChart!!.interaction("pan")
        mSubChart!!.interaction("pinch")
        mSubChart!!.tooltip(ToolTipConfigBuilder().setOption("xTip", false))
        mSubChart!!.render()
    }

    override fun onDestroy() {
        if (mCandleChart != null) {
            mCandleChart!!.destroy()
        }
        if (mSubChart != null) {
            mSubChart!!.destroy()
        }
    }

    override fun onTouch(canvasView: F2CanvasView, event: F2CanvasView.TouchEvent) {
        val tm = System.currentTimeMillis()
        if (mCandleChart != null) {
            mCandleChart!!.postTouchEvent(event)
        }
        if (mSubChart != null) {
            mSubChart!!.postTouchEvent(event)
        }
        // System.out.println("@@@@ touchDuration: " + (System.currentTimeMillis() - tm) +"ms");
    }

    protected fun drawMAIndicators(canvasView: F2CanvasView?, param: String?) {
        val maIndicator = JSON.parseObject(param)
        val tooltips = maIndicator.getJSONArray("tooltip")
        for (i in tooltips.indices) {
            val item = tooltips.getJSONObject(i)
            if ("values" == item.getString("name")) {
                continue
            }
        }
    }
}