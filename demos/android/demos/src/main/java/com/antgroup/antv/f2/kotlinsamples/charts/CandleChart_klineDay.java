package com.antgroup.antv.f2.kotlinsamples.charts;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * 蜡烛图(日K)
 * @author qingyuan.yl
 */
public class CandleChart_klineDay implements F2CanvasView.Adapter, F2CanvasView.OnCanvasTouchListener {
    private F2Chart mCandleChart;
    private F2Chart mSubChart;

    @Override
    public void onCanvasDraw(final F2CanvasView canvasView) {
        if (mCandleChart == null) {
            canvasView.setOnCanvasTouchListener(this);
            mCandleChart = F2Chart.create(canvasView.getContext(), "KlineDay", canvasView.getWidth(), canvasView.getHeight() * 3 / 4);
            mSubChart = F2Chart.create(canvasView.getContext(), "KlineDay-Sub", canvasView.getWidth(), canvasView.getHeight() / 4);
        }

        mCandleChart.setCanvas(canvasView);
        mCandleChart.padding(15, 10, 0, 0);
        mCandleChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_klineDay.json"));

        mCandleChart.candle().position("date*values");
        mCandleChart.line().position("date*m5").fixedColor("#A46FFE");
        mCandleChart.line().position("date*m10").fixedColor("#46ACFF");
        mCandleChart.line().position("date*m20").fixedColor("#FF8C53");

        mCandleChart.setAxis("m5", new F2Chart.AxisConfigBuilder().hidden());
        mCandleChart.setAxis("m10", new F2Chart.AxisConfigBuilder().hidden());
        mCandleChart.setAxis("m20", new F2Chart.AxisConfigBuilder().hidden());

        mCandleChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5)));
        mCandleChart.setScale("date", new F2Chart.ScaleConfigBuilder().type("kline-day").setOption("domain", new double[]{40, 80}));
        mCandleChart.setScale("values", new F2Chart.ScaleConfigBuilder().nice(true));
        mCandleChart.setScale("m5", new F2Chart.ScaleConfigBuilder().setOption("assign", "values")); // 复用 values 字段的度量
        mCandleChart.setScale("m10", new F2Chart.ScaleConfigBuilder().setOption("assign", "values"));// 复用 values 字段的度量
        mCandleChart.setScale("m20", new F2Chart.ScaleConfigBuilder().setOption("assign", "values"));// 复用 values 字段的度量

        mCandleChart.interaction("pan");
        mCandleChart.interaction("pinch");
        mCandleChart.tooltip(new F2Chart.ToolTipConfigBuilder().setOption("onPress", mCandleChart, new F2Function() {
            @Override
            public F2Config execute(String param) {
                drawMAIndicators(canvasView, param);
                return null;
            }
        }));
        mCandleChart.render();

        // sub chart
        mSubChart.adjustScale(false);
        mSubChart.setCanvas(canvasView);
        mSubChart.margin(0, (canvasView.getHeight() * 3 / 4) / canvasView.getResources().getDisplayMetrics().density, 0, 0);
        mSubChart.padding(12, 0, 0, 20);
        mSubChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_klineDay.json"));
        mSubChart.setAxis("date", new F2Chart.AxisConfigBuilder().hidden());
        mSubChart.interval().position("date*volumn").style(new F2Config.Builder().setOption("widthRatio", 0.9).build());
        mSubChart.setScale("date", new F2Chart.ScaleConfigBuilder().type("kline-day").setOption("domain", new double[]{40, 80}));
        mSubChart.setAxis("volumn", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().item(mSubChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        return new F2Config.Builder().setOption("content", "1亿手").build();
                    }
                })));
        mSubChart.setScale("volumn", new F2Chart.ScaleConfigBuilder().nice(true).tickCount(2));
        mSubChart.interaction("pan");
        mSubChart.interaction("pinch");
        mSubChart.tooltip(new F2Chart.ToolTipConfigBuilder().setOption("xTip", false));
        mSubChart.render();
    }

    @Override
    public void onDestroy() {
        if (mCandleChart != null) {
            mCandleChart.destroy();
        }

        if (mSubChart != null) {
            mSubChart.destroy();
        }
    }

    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        long tm = System.currentTimeMillis();
        if (mCandleChart != null) {
            mCandleChart.postTouchEvent(event);
        }
        if (mSubChart != null) {
            mSubChart.postTouchEvent(event);
        }
        // System.out.println("@@@@ touchDuration: " + (System.currentTimeMillis() - tm) +"ms");
    }

    protected void drawMAIndicators(F2CanvasView canvasView, String param) {
        JSONObject maIndicator = JSON.parseObject(param);
        JSONArray tooltips = maIndicator.getJSONArray("tooltip");
        for (int i = 0; i < tooltips.size(); i++) {
            JSONObject item = tooltips.getJSONObject(i);
            if ("values".equals(item.getString("name"))) {
                continue;
            }
        }
    }
}
