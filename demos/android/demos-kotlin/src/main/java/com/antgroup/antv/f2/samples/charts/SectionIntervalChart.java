package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 区间柱状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SectionIntervalChart implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SectionIntervalChart", canvasView.getWidth(), canvasView.getHeight());
            canvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
                @Override
                public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
                    mChart.postTouchEvent(event);
                }
            });
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_sectionInterval.json"));
        mChart.interval().position("x*y");
        mChart.setAxis("x", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                .grid(new F2Chart.AxisGridConfigBuilder().stroke("#000").lineWidth(1))
                );

        mChart.setAxis("y", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));

        mChart.setScale("x", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));
        mChart.setScale("y", new F2Chart.ScaleConfigBuilder().nice(true));
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}