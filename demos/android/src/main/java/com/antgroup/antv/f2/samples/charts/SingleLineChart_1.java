package com.antgroup.antv.f2.samples.charts;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.F2Log;
import com.antgroup.antv.f2.samples.Utils;

import java.io.InputStream;

//import com.alipay.antgraphic.misc.CanvasImageResource;
//import com.alipay.antgraphic.misc.CanvasIsolateHelper;

/**
 * 基础折线图-1
 *
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
        mChart.tooltip(new F2Chart.ToolTipConfigBuilder()
                .setOption("onPressStart", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        F2Log.i("tooltip", "onPressStart: " + param);
                        return new F2Config.Builder().build();
                    }
                })
                .setOption("onPressEnd", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        F2Log.i("tooltip", "onPressEnd: " + param);
                        return new F2Config.Builder().build();
                    }
                })
                .setOption("onPress", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        drawToolTipRect(canvasView, param);
                        return new F2Config.Builder().build();
                    }
                }));

        mChart.animate(true);

        mChart.line().position("date*value");
        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f).item(mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        JSONObject params = JSON.parseObject(param);
                        int index = params.getIntValue("index");
                        return new F2Config.Builder().setOption("textColor", (index % 2 == 0 ? "#000000" : "#DC143C")).setOption("xOffset", (index % 2 == 0) ? 0 : -30).build();
                    }
                }))
                .gridHidden());

        mChart.setAxis("value", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder()
                        .type("dash")
                        .lineDash(new double[]{8, 3, 3, 8})
                )
        );
        mChart.setScale("date", new F2Chart.ScaleConfigBuilder().tick(mChart, new F2Function() {
            @Override
            public F2Config execute(String param) {
                String[] arr = param.split("-");
                String rst;
                if (arr.length == 3) {
                    rst = arr[1] + "-" + arr[2];
                } else {
                    rst = param;
                }
                return new F2Config.Builder().setOption("content", rst).build();
            }
        }).tickCount(5));
        mChart.setScale("value", new F2Chart.ScaleConfigBuilder().setOption("nice", true));

        mChart.render();

        drawTag(canvasView);
    }

    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        if (mChart != null && mChart.postTouchEvent(event)) {
            drawTag(canvasView);
        }
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


//        CanvasImageResource imageResource = new CanvasImageResource("10001", getImageFromAssets(canvasView.getContext(), "wallet.png"));
//        float width = imageResource.getBitmap().getWidth();
//        float height = imageResource.getBitmap().getHeight();
//
//        float x = (float) position[0] - width / 2;
//        float y = (float) position[1] + 10;

        canvasView.swapBuffer();
    }

    private void drawToolTipRect(F2CanvasView canvasView, String param) {
        JSONObject itemData = JSON.parseArray(param).getJSONObject(0);
        float x = itemData.getFloatValue("x");
        float y = itemData.getFloatValue("y");
        String color = itemData.getString("color");
        canvasView.getCanvasHandle().save();
        canvasView.getCanvasHandle().setFillStyle(color);
        canvasView.getCanvasHandle().fillRect((int) x, (int) y, 100, 50);
        canvasView.getCanvasHandle().restore();

        canvasView.getCanvasHandle().save();
        canvasView.getCanvasHandle().setFillStyle("#ffffff");
        String text = itemData.getString("value");
        canvasView.getCanvasHandle().setTextAlign("center");
        canvasView.getCanvasHandle().fillText(text, x + 50, y + 40);
        canvasView.getCanvasHandle().restore();
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
            Log.e("SingleLineChart1", "getImageFromAssets error:" + t);
        } finally {
            if (ins != null) {
                try {
                    ins.close();
                } catch (Throwable e) {
                    Log.e("SingleLineChart1", "getImageFromAssets close error:" + e);
                }
            }
        }
        return bitmap;
    }
}