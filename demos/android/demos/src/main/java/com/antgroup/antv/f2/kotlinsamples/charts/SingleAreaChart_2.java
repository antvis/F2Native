package com.antgroup.antv.f2.kotlinsamples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleAreaChart_2 implements F2CanvasView.Adapter {

    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleAreaChart_2", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleAreaChart_2.json"));
        mChart.line().position("month*value");
        mChart.area().position("month*value");
        mChart.setAxis("month", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder()
                        .labelMargin(10)
                        .labelOffset(5.f))
                .lineHidden()
                .gridHidden());

        mChart.setAxis("value", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelMargin(10))
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));
        mChart.setScale("value", new F2Chart.ScaleConfigBuilder().nice(true));

        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}
