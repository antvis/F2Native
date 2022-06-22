package com.antgroup.antv.f2.kotlinsamples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * 分组柱状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class MultiIntervalChart implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "MultiIntervalChart", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_multiIntervalsChart.json"));
        mChart.interval().position("月份*月均降雨量").color("name").adjust("dodge");
        mChart.setScale("月份", new F2Chart.ScaleConfigBuilder().range(new double[]{0.065, 0.935}));
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}
