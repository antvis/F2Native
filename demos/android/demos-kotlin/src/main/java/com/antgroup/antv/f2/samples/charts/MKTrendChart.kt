package com.antgroup.antv.f2.samples.charts

import android.os.Handler
import com.alibaba.fastjson.JSON
import com.alibaba.fastjson.JSONArray
import com.alibaba.fastjson.JSONObject
import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.*
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.F2Function
import com.antgroup.antv.f2.F2Guide.GuideFlagConfigBuilder
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * @author qingyuan.yl
 * @version 10.1.1
 * @date 2020/12/22
 */
class MKTrendChart : F2CanvasView.Adapter {
    private val mHandler = Handler()
    private var mChart: F2Chart? = null
    var mJsonData: JSONObject? = null
    var mFlagsData: JSONArray? = null

    @Volatile
    var rangeIndex = 1
    var stopLoop = false
    private var mCanvasView: F2CanvasView? = null
    private fun startLoop() {
        if (stopLoop) return
        rangeIndex++
        if (mCanvasView != null) {
            mCanvasView!!.postCanvasDraw()
        }
        mHandler.postDelayed({ startLoop() }, 1000)
    }

    override fun onCanvasDraw(canvasView: F2CanvasView) {
        mCanvasView = canvasView
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "MKTrendChart",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
            mHandler.postDelayed({ startLoop() }, 1000)
        }
        mChart!!.clear()
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(0.0, 0.0, 0.0, 0.0)
        if (mJsonData == null) {
            mJsonData =
                JSON.parseObject(loadAssetFile(canvasView.context, "mockData_trendChart.json"))
        }
        val sourceData = mJsonData!!.getJSONArray("items")
        val subData: List<*> = sourceData.subList(0, rangeIndex % sourceData.size)
        mChart!!.source(JSON.toJSONString(subData))
        try {
            val timeRange = mJsonData!!.getJSONArray("timeRange")
            val timeRangeCopy = org.json.JSONArray(timeRange.toJSONString())
            mChart!!.setScale(
                "date",
                ScaleConfigBuilder().type("timeSharing").setOption("timeRange", timeRangeCopy)
            )
        } catch (e: Exception) {
            e.printStackTrace()
        }
        val priceScaleCfg = ScaleConfigBuilder()
        priceScaleCfg.tickCount(2)
        priceScaleCfg.min(mJsonData!!.getDouble("min") - 2).max(mJsonData!!.getDouble("max") + 2)
        mChart!!.setScale("price", priceScaleCfg)
        mChart!!.setAxis(
            "date", AxisConfigBuilder()
                .label(
                    AxisLabelConfigBuilder().labelMargin(20f).labelOffset(4f).textColor("#999999")
                        .item(mChart, object : F2Function() {
                            override fun execute(param: String): F2Config {
                                val json = JSON.parseObject(param)
                                val index = json.getIntValue("index")
                                return if (index == 0) {
                                    F2Config.Builder<Any?>().setOption("content", "09:30").build()
                                } else if (index == 1) {
                                    F2Config.Builder<Any?>().setOption("content", "11:30/13:00")
                                        .build()
                                } else if (index == 2) {
                                    F2Config.Builder<Any?>().setOption("content", "15:00").build()
                                } else {
                                    F2Config.Builder<Any?>().setOption("content", "").build()
                                }
                            }
                        })
                )
                .line(AxisLineConfigBuilder().color("#DDDDDD"))
        )
        mChart!!.setAxis("price", AxisConfigBuilder().hidden())
        mChart!!.line().position("date*price").fixedColor("#528EFF")
        mChart!!.area().position("date*price").fixedColor("#108EE9")
        val flagsData = JSONArray.parseArray(
            loadAssetFile(
                canvasView.context,
                "mockData_trendChart_flags.json"
            )
        )
        for (i in flagsData.indices) {
            try {
                val flagCfg = flagsData.getJSONObject(i)
                val cfg = GuideFlagConfigBuilder()
                val position = flagCfg.getJSONArray("position")
                cfg.position(org.json.JSONArray(position.toJSONString()))
                    .content(flagCfg.getString("content"))
                    .padding(doubleArrayOf(5.0, 3.0, 5.0, 1.0))
                    .textSize(10f)
                    .lineWidth(0.5)
                if (i == flagsData.size - 1) {
                    cfg.textColor("#ffffff")
                    cfg.backgroundColor("#108EE9")
                } else {
                    cfg.textColor("#F54346")
                    cfg.backgroundColor("#C7E7FF")
                }
                mChart!!.guide().flag(cfg)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
        mChart!!.render()
    }

    override fun onDestroy() {
        stopLoop = true
        if (mChart != null) {
            mChart!!.destroy()
        }
        mCanvasView = null
    }
}