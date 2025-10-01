package com.antgroup.antv.f2.samples.charts;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.F2Guide;
import com.antgroup.antv.f2.F2Log;
import com.antgroup.antv.f2.samples.Utils;

import java.io.InputStream;

/**
 * 基础折线图
 *
 * @author qingyuan.yl
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
        mChart.tooltip(new F2Chart.ToolTipConfigBuilder()
                .setOption("onPressStart", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        F2Log.get().i("tooltip", "onPressStart: " + param);
                        return new F2Config.Builder().build();
                    }
                })
                .setOption("onPressEnd", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        F2Log.get().i("tooltip", "onPressEnd: " + param);
                        return new F2Config.Builder().build();
                    }
                })
                .setOption("onPress", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        F2Log.get().i("tooltip", "onPress: " + param);
                        return new F2Config.Builder().build();
                    }
                }));

        mChart.animate(true);

        mChart.line().position("date*value").attrs(new F2Config.Builder().setOption("connectNulls", true).build());
        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f).item(mChart, new F2Function() {
                    @Override
                    public F2Config execute(String paramStr) {
                        JSONObject jsonObject = JSON.parseObject(paramStr);
                        String param = jsonObject.getString("content");
                        String[] arr = param.split("-");
                        String rst;
                        if (arr.length == 3) {
                            rst = arr[1] + "-" + arr[2];
                        } else {
                            rst = param;
                        }

                        JSONObject params = JSON.parseObject(param);
                        int index = params.getIntValue("index");
                        return new F2Config.Builder()
                                .setOption("textColor", (index % 2 == 0 ? "#000000" : "#DC143C"))
                                .setOption("xOffset", (index % 2 == 0) ? 0 : -30)
                                .setOption("content", rst)
                                .build();
                    }
                }))
                .gridHidden());

        mChart.setAxis("value", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder()
                        .type("dash")
                        .lineDash(new double[]{8, 3, 3, 8})
                )
        );
        mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tickCount(5));
        mChart.setScale("value", new F2Chart.ScaleConfigBuilder().setOption("nice", true));
        String guideJson = "[\n" +
                "    {\n" +
                "        \"position\": [\"2017-06-05\",116]\n" +
                "    },\n" +
                "    {\n" +
                "        \"position\": [\"2017-06-10\",85]\n" +
                "    }\n" +
                "]";
        JSONArray flagsData = JSONArray.parseArray(guideJson);

        for (int i = 0; i < flagsData.size(); i++) {
            try {
                JSONObject flagCfg = flagsData.getJSONObject(i);
                F2Guide.GuidePointConfigBuilder cfg = new F2Guide.GuidePointConfigBuilder();
                JSONArray position = flagCfg.getJSONArray("position");
                cfg.position(new org.json.JSONArray(position.toJSONString()))
                        .size(4)
                        .shape("circle")
                        .fill("#108EE9")
                        .lineWidth(0.1f);

                mChart.guide().point(cfg);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        mChart.render();
    }

    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        if (mChart != null && mChart.postTouchEvent(event)) {
        }
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }

    private static Bitmap getImageFromAssets(Context context, String filePath) {
        Bitmap bitmap = null;
        InputStream ins = null;
        try {
            AssetManager assetManager = context.getAssets();
            ins = assetManager.open(filePath);
            bitmap = BitmapFactory.decodeStream(ins);
            ins.close();
        } catch (Throwable t) {
            F2Log.get().e("SingleLineChart1", "getImageFromAssets error:", t);
        } finally {
            if (ins != null) {
                try {
                    ins.close();
                } catch (Throwable e) {
                    F2Log.get().e("SingleLineChart1", "getImageFromAssets close error:", e);
                }
            }
        }
        return bitmap;
    }
}