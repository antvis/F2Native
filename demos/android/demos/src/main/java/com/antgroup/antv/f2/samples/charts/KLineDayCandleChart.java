package com.antgroup.antv.f2.samples.charts;

import java.util.Collections;
import java.util.List;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.samples.Utils;

/**
 * author by luna
 * date on 2024/10/18
 */

public class KLineDayCandleChart implements F2CanvasView.Adapter, F2CanvasView.OnCanvasTouchListener {
    private F2Chart mCandleChart;

    @Override
    public void onCanvasDraw(final F2CanvasView canvasView) {
        if (mCandleChart == null) {
            canvasView.setOnCanvasTouchListener(this);
            mCandleChart = F2Chart.create(canvasView.getContext(), "KlineDay", canvasView.getWidth(), canvasView.getHeight() * 3 / 4);
        }
        mCandleChart.setCanvas(canvasView);
        mCandleChart.padding(15, 10, 0, 0);
        String s = Utils.loadAssetFile(canvasView.getContext(), "mockData_klineDay_1.json");
        List<JSONObject> objects = JSONArray.parseArray(s, JSONObject.class);
        int size = objects.size();
        Collections.reverse(objects);
        String s1 = JSONObject.toJSONString(objects);


        mCandleChart.source(s1);
        mCandleChart.candle().position("time*value")
                .style(new F2Config.Builder()
                        .setOption("radius", 4)
                        .setOption("lineWidth", 1)
                        .setOption("fill", new String[]{"#0E9976", "#E62C3B", "#E62C3B"})
                        .build());


        mCandleChart.setAxis("time",
                new F2Chart.AxisConfigBuilder()
                        .gridHidden()
                        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(10))
                        .line(new F2Chart.AxisLineConfigBuilder().type("line").lineWidth(1).color("#ddd"))
        );

        mCandleChart.setAxis("value",
                new F2Chart.AxisConfigBuilder()
                        .lineHidden()
                        .grid(new F2Chart.AxisGridConfigBuilder().type("dash").lineWidth(1).stroke("#ddd"))
                        .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(-5).textColor("#cccccc"))
        );


        mCandleChart.setScale("time",
                new F2Chart.ScaleConfigBuilder()
                        .type("kline-day")
                        .setOption("domain", new double[]{size -50, size - 1})
        );
        mCandleChart.setScale("value", new F2Chart.ScaleConfigBuilder().nice(true));

        mCandleChart.interaction("pan");
        mCandleChart.interaction("pinch");
        mCandleChart.tooltip(new F2Chart.ToolTipConfigBuilder().setOption("onPress", mCandleChart, new F2Function() {
            @Override
            public F2Config execute(String param) {
                return null;
            }
        }));
        mCandleChart.render();
    }

    @Override
    public void onDestroy() {
        if (mCandleChart != null) {
            mCandleChart.destroy();
        }
    }

    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        long tm = System.currentTimeMillis();
        if (mCandleChart != null) {
            mCandleChart.postTouchEvent(event);
        }
    }

}
