package com.antgroup.antv.f2.samples.charts

import com.antgroup.antv.f2.F2CanvasView
import com.antgroup.antv.f2.F2Chart
import com.antgroup.antv.f2.F2Chart.AxisConfigBuilder
import com.antgroup.antv.f2.F2Chart.CoordConfigBuilder
import com.antgroup.antv.f2.F2Config
import com.antgroup.antv.f2.samples.Utils.loadAssetFile

/**
 * 饼图
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
class PieChart : F2CanvasView.Adapter {
    private var mChart: F2Chart? = null
    override fun onCanvasDraw(canvasView: F2CanvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(
                canvasView.context,
                "PieChart",
                canvasView.width.toDouble(),
                canvasView.height.toDouble()
            )
        }
        mChart!!.setCanvas(canvasView)
        mChart!!.padding(10.0, 20.0, 0.0, 0.0)
        mChart!!.source(loadAssetFile(canvasView.context, "mockData_basePie.json"))
        mChart!!.setAxis(
            "percent", AxisConfigBuilder()
                .lineHidden()
                .labelHidden()
                .gridHidden()
        )
        mChart!!.setAxis("a", AxisConfigBuilder().labelHidden().lineHidden().gridHidden())
        mChart!!.setCoord(CoordConfigBuilder().type("polar").transposed(true))
        mChart!!.interval().position("a*percent").fixedSize(1f)
            .style(F2Config.Builder<Any?>().setOption("lineWidth", 3).build()).color("name")
            .adjust("stack")
        mChart!!.render()
    }

    override fun onDestroy() {
        if (mChart != null) {
            mChart!!.destroy()
        }
    }
}