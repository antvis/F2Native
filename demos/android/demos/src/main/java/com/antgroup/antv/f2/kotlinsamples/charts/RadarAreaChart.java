package com.antgroup.antv.f2.kotlinsamples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * 雷达面积图
 */
public class RadarAreaChart implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "MultiAreasChart", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 5, 20, 10);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_radarArea.json"));
        mChart.line().position("item*score").color("user");
        mChart.area().position("item*score").color("user");
        mChart.point().position("item*score").color("user")
                .style(new F2Config.Builder().setOption("lineWidth", 3).setOption("stroke", "#fff").setOption("lineWidth", 1).build());
        mChart.setCoord(new F2Chart.CoordConfigBuilder().type("polar"));
        mChart.setScale("score", new F2Chart.ScaleConfigBuilder().min(0).max(120).nice(false).tickCount(4));
        mChart.setAxis("score", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder().type("arc").stroke("#E8E8E8FF").lineDash(new double[]{4, 2})));
        mChart.setAxis("item", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(30))
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash").stroke("#E8E8E8FF").lineDash(new double[]{4, 2})));
        mChart.legend("user", new F2Chart.LegendConfigBuild().position("top"));
        mChart.animate(new F2Chart.ChartAnimateConfigBuild().geom(
                "line", new F2Chart.GeomAnimateConfigBuild().animateType("GroupScaleInXY"))); // GroupWaveIn GroupScaleInXY GroupScaleInX GroupScaleInY
        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}
