package com.antgroup.antv.f2.samples.charts;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.samples.Utils;

/**
 * 基础折线图-1
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleLineChart_1 implements F2CanvasView.Adapter, F2CanvasView.OnCanvasTouchListener {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(final F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleLineChart_1", canvasView.getWidth(), canvasView.getHeight());
            canvasView.setOnCanvasTouchListener(this);
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleLineChart.json"));
        mChart.interaction("pinch");
        mChart.interaction("pan");
        mChart.tooltip(new F2Chart.ToolTipConfigBuilder().setOption("onPress", mChart, new F2Function() {
            @Override
            public F2Config execute(String param) {
                drawToolTipRect(canvasView, param);
                return new F2Config.Builder().build();
            }
        }));

        mChart.line().position("date*value");
        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f).item(mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        JSONObject params = JSON.parseObject(param);
                        int index = params.getIntValue("index");
                        return new F2Config.Builder().setOption("textColor", (index % 2 == 0 ? "#000000" : "#DC143C")).setOption("xOffset", (index % 2 == 0)? 0 : -30).build();
                    }
                }))
                .gridHidden());

        mChart.setAxis("value", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));
        mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tick(mChart, new F2Function() {
            @Override
            public F2Config execute(String param) {
                String[] arr = param.split("-");
                String rst;
                if (arr.length == 3) {
                    rst = arr[1] +"-"+arr[2];
                } else {
                    rst = param;
                }
                return new F2Config.Builder().setOption("content", rst).build();
            }
        }).tickCount(5));
        mChart.setScale("value", new F2Chart.ScaleConfigBuilder().setOption("nice", true).min(10).max(340));
        mChart.render();

        drawTag(canvasView);
    }

    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        if (mChart != null) {
            mChart.postTouchEvent(event);
        }
        drawTag(canvasView);
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }

    private void drawTag(F2CanvasView canvasView) {
        if (mChart == null) return;
        double[] position = mChart.getPosition("{\"date\":\"2017-06-10\",\"value\":85}");
        canvasView.getCanvasHandle().save();
        canvasView.getCanvasHandle().setFillStyle("#DC143C");
        canvasView.getCanvasHandle().beginPath();
        canvasView.getCanvasHandle().arc((float) position[0], (float) position[1], 8.f, 0.0f, (float) Math.toRadians(360.f), false);
        canvasView.getCanvasHandle().fill();
        canvasView.getCanvasHandle().restore();

        canvasView.swapBuffer();
    }

    private void drawToolTipRect(F2CanvasView canvasView, String param) {
        JSONObject itemData = JSON.parseArray(param).getJSONObject(0);
        float x = itemData.getFloatValue("x");
        float y = itemData.getFloatValue("y");
        String color = itemData.getString("color");
        canvasView.getCanvasHandle().save();
        canvasView.getCanvasHandle().setFillStyle(color);
        canvasView.getCanvasHandle().fillRect((int)x, (int)y, 100, 50);
        canvasView.getCanvasHandle().restore();

        canvasView.getCanvasHandle().save();
        canvasView.getCanvasHandle().setFillStyle("#ffffff");
        String text = itemData.getString("value");
        float textWidth = canvasView.getCanvasHandle().measureText(text);
        canvasView.getCanvasHandle().fillText(text, x + (100 - textWidth)/2, y + 40);
        canvasView.getCanvasHandle().restore();
    }
}