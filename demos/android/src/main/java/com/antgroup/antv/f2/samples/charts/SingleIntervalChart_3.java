package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 基础条状图
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleIntervalChart_3 implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleIntervalChart_3", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart.json"));
        mChart.interval().position("year*sales");
        mChart.setAxis("year", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                .gridHidden());
        mChart.setCoord(new F2Chart.CoordConfigBuilder().type("rect").transposed(true));

        mChart.setAxis("sales", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5))
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash"))
                .line(new F2Chart.AxisLineConfigBuilder().lineWidth(1).color("#E6E6E6").type("dash")));
        mChart.setScale("year", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));
        mChart.setScale("sales", new F2Chart.ScaleConfigBuilder().nice(true));

        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}