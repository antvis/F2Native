package com.antgroup.antv.f2.kotlinsamples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * 基础柱状图-2
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleIntervalChart_2 implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleIntervalChart_2", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(20, 10, 10, 10);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart_2.json"));
        mChart.interval().position("genre*sold").color("genre");
        mChart.setScale("sold", new F2Chart.ScaleConfigBuilder().min(0));
        mChart.setScale("genre", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));
        mChart.legend("genre", new F2Chart.LegendConfigBuild().enable(true).position("top").symbol("circle").setOption("radius", 3));
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}