package com.antgroup.antv.f2.kotlinsamples.charts;

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
import com.antgroup.antv.f2.kotlinsamples.Utils;

import java.io.InputStream;
import java.text.DateFormat;
import java.util.Date;


/**
 * 股票日K趋势图
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
        mChart.line().position("date*price");

        F2Chart.ScaleConfigBuilder scaleConfigBuilder = new F2Chart.ScaleConfigBuilder();
        scaleConfigBuilder
                .tickCount(5)
                .type("kline-day")
                .setOption("timeZoneOffset", 28800)
                .setOption("domain", new double[]{1, 20});


        mChart.setScale("date", scaleConfigBuilder);

        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().item(mChart, new F2Function() {
                    @Override
                    public F2Config execute(String paramStr) {
                        JSONObject jsonObject = JSON.parseObject(paramStr);
                        String param = jsonObject.getString("content");
                        long timestamp = Long.parseLong(param);
                        String content = DateFormat.getDateInstance().format(new Date(timestamp));
                        return new F2Config.Builder().setOption("content", content).build();
                    }
                })));

        mChart.setScale("price", new F2Chart.ScaleConfigBuilder()
                .tickCount(2).nice(true));
        mChart.tooltip(new F2Chart.ToolTipConfigBuilder()
                .setOption("yTip", new F2Config.Builder().setOption("inner", true)));

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