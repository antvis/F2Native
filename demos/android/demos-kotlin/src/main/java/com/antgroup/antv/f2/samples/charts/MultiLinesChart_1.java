package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 多组折线图
 * @author qingyuan.yl
 * @date 2020-09-25
 */
public class MultiLinesChart_1 implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "MultiLinesChart", canvasView.getWidth(), canvasView.getHeight());

            canvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
                @Override
                public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
                    mChart.postTouchEvent(event);
                }
            });
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_multilines.json"));
        mChart.line().position("date*value").color("type").fixedSize(1.f).fixedShape("smooth");
        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f)));
        mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5));
        mChart.setScale("value", new F2Chart.ScaleConfigBuilder().nice(true));
        mChart.tooltip(new F2Chart.ToolTipConfigBuilder());
        mChart.interaction("pan");
        mChart.interaction("pinch");
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}