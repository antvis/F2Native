package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 基础柱状图
 *
 * @author qingyuan.yl
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
        mChart.interval().position("year*sales")
                .color("year", new String[]{"#00A97F",
                        "#00ff00",
                        "#ff0000",
                        "#0000ff"})
                .style(new F2Config.Builder()
                        .setOption("radius", new float[]{2, 2, 2, 2}) // [tl, tr, bl, br]
                        .build());
        mChart.setAxis("year", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder()
                        .labelOffset(10.f).textSize(10))
        );

        mChart.setAxis("sales", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));
        mChart.setScale("year", new F2Chart.ScaleConfigBuilder().tickCount(4).type("cat").range(new double[]{0.1, 0.9}));
        mChart.setScale("sales", new F2Chart.ScaleConfigBuilder().tickCount(5).nice(true));
        mChart.tooltip(new F2Chart.ToolTipConfigBuilder());
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}