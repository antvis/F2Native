package com.antgroup.antv.f2.samples.charts;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.samples.Utils;

import org.json.JSONArray;

/**
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleLineChart_TimeSharing_ToolTip implements F2CanvasView.Adapter {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleIntervalChart_demo", canvasView.getWidth(), canvasView.getHeight());
            canvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
                @Override
                public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
                    mChart.postTouchEvent(event);
                }
            });
        }



        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleLineChart_timeSharing_tooltip.json"));
        mChart.line().position("date*mainNetIn").fixedColor("#1677FF").fixedSize(1.5f);
        mChart.setScale("mainNetIn", new F2Chart.ScaleConfigBuilder()
                .type("linear").tickCount(5).nice(true).precision(2));

        JSONArray timeRange = new JSONArray();
        JSONArray item1 = new JSONArray();
        item1.put(1618882200000L);
        item1.put(1618889400000L);
        timeRange.put(item1);

        JSONArray item2 = new JSONArray();
        item2.put(1618894800000L);
        item2.put(1618907400000L);
        timeRange.put(item2);
        mChart.setScale("date", new F2Chart.ScaleConfigBuilder()
            .type("timeSharing").setOption("timeRange", timeRange));
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