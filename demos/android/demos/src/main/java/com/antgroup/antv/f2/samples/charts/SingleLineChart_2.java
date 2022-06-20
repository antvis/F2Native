package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 基础折线图(双Y轴)
 * @author qingyuan.yl
 */
public class SingleLineChart_2 implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleLineChart_2", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 10, 10, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_multiAxiesLine.json"));
        mChart.line().position("date*value").fixedColor("#1890FF");
        mChart.line().position("date*offset").fixedColor("#2FC25B");
        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                .gridHidden());

        mChart.setAxis("value", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
                );
        mChart.setAxis("offset", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
        );

        mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(3));
        mChart.setScale("value", new F2Chart.ScaleConfigBuilder().nice(true));
        mChart.setScale("offset", new F2Chart.ScaleConfigBuilder().nice(true));
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}