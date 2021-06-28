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
import com.antgroup.antv.f2.samples.Utils;

import java.io.InputStream;
import java.text.DateFormat;
import java.util.Date;

//import com.alipay.antgraphic.misc.CanvasImageResource;
//import com.alipay.antgraphic.misc.CanvasIsolateHelper;

/**
 * 基础折线图-1
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleLineChart_KLine implements F2CanvasView.Adapter, F2CanvasView.OnCanvasTouchListener {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(final F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleLineChart_KLine", canvasView.getWidth(), canvasView.getHeight());
            canvasView.setOnCanvasTouchListener(this);
        }

        mChart.clear();
        mChart.setCanvas(canvasView);
        mChart.padding(10, 30, 10, 20);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_line_kline.json"));
        F2Chart.ScaleConfigBuilder scaleConfigBuilder = new F2Chart.ScaleConfigBuilder();
        scaleConfigBuilder
                .tickCount(5)
                .tick(mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        long timestamp = Long.parseLong(param);
                        String content = DateFormat.getDateInstance().format(new Date(timestamp));
                        return new F2Config.Builder().setOption("content", content).build();
                    }
                })
                .type("kline-day")
                .setOption("timeZoneOffset", 28800)
                .setOption("domain", new double[] {1, 20});

        mChart.setScale("date", scaleConfigBuilder);

//        mChart.area().position( "date*price");
        mChart.line().position( "date*price");

//        F2Chart.AxisConfigBuilder builder = new F2Chart.AxisConfigBuilder()
//                .grid(new F2Chart.AxisGridConfigBuilder().lineWidth(mConfig.getWidthRegion1Grid()).stroke(colorInt2Hex(mConfig.getColorRegion1Grid())))
//                .lineHidden();
//
//        mChart.setAxis("date", builder);


        mChart.setScale("price", new F2Chart.ScaleConfigBuilder()
                .tickCount(2).nice(true));

//        F2Chart.AxisConfigBuilder axisConfigBuilder = new F2Chart.AxisConfigBuilder()
//                .grid(new F2Chart.AxisGridConfigBuilder().lineWidth(mConfig.getWidthRegion1Grid()).stroke(colorInt2Hex(mConfig.getColorRegion1Grid())))
//                .labelHidden()
//                .lineHidden();

//        mChart.setAxis("price", axisConfigBuilder);


        mChart.tooltip(new F2Chart.ToolTipConfigBuilder()
                .setOption("yTip", new F2Config.Builder().setOption("inner", true))
        );

        mChart.render();
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
//
//        CanvasIsolateHelper.putImage(canvasView.getCanvas().getCanvasSessionId(), imageResource);
//        canvasView.getCanvasHandle().drawImage("10001", width, height, 0, 0, width, height, x, y, width * 2, height * 2);
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