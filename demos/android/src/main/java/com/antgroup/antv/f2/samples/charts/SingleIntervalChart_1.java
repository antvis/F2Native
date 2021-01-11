package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 基础柱状图-1
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleIntervalChart_1 implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleIntervalChart_1", canvasView.getWidth(), canvasView.getHeight());
            canvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
                @Override
                public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
                    mChart.postTouchEvent(event);
                }
            });
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart.json"));
        mChart.interval().position("year*sales");
        mChart.line().position("year*sales").fixedShape("smooth");
        mChart.setAxis("year", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                .grid(new F2Chart.AxisGridConfigBuilder().stroke("#000").lineWidth(1))
                );
//        mChart.setAxis("sales", new F2Chart.AxisConfigBuilder().hidden());
//        mChart.setAxis("year", new F2Chart.AxisConfigBuilder().labelHidden());

        mChart.setAxis("sales", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));
        mChart.setScale("year", new F2Chart.ScaleConfigBuilder().tickCount(5));
        mChart.setScale("sales", new F2Chart.ScaleConfigBuilder().min(15).max(50).tickCount(5));
        mChart.interaction("pinch", new F2Config.Builder().setOption("maxCount", 80).setOption("minCount", 10).build()).interaction("pan");


        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}