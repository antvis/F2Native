package com.antgroup.antv.f2.kotlinsamples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * 环形图
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class CirclePieChart implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "PieChart", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 20, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_basePie.json"));
        mChart.setAxis("percent", new F2Chart.AxisConfigBuilder()
                .lineHidden()
                .labelHidden()
                .gridHidden());

        mChart.setAxis("a", new F2Chart.AxisConfigBuilder().labelHidden().lineHidden().gridHidden());
        mChart.setCoord(new F2Chart.CoordConfigBuilder().type("polar"));
        mChart.interval().position("a*percent").color("name").adjust("stack");
        mChart.render();
}

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}