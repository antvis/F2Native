package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 分组柱状图(带负值)
 * @author qingyuan.yl
 * @date 2020-12-22
 */
public class MultiIntervalChart_under_zero implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "MultiIntervalChart_under_zero", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.setCoord(new F2Chart.CoordConfigBuilder().transposed(true));
        mChart.padding(20, 10, 10, 10);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleIntervalChart_under_zero.json"));
        mChart.setAxis("tem", new F2Chart.AxisConfigBuilder().grid(
                new F2Chart.AxisGridConfigBuilder().stroke("#000")
        ));
        mChart.interval().position("time*tem").style(new F2Config.Builder()
                .setOption("radius", new float[]{10, 10, 10, 10 }) // [tl, tr, bl, br]
                .build());
        mChart.setScale("tem", new F2Chart.ScaleConfigBuilder().nice(true));
        mChart.setScale("time", new F2Chart.ScaleConfigBuilder().range(new double[]{0.1, 0.9}));
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}