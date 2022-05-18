package com.antgroup.antv.f2.samples.charts;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.samples.Utils;

import java.io.InputStream;


/**
 * 基础折线图-1
 *
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleLineChart_1688 implements F2CanvasView.Adapter, F2CanvasView.OnCanvasTouchListener {
    private F2Chart mChart;

    @Override
    public void onCanvasDraw(final F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleLineChart_1", canvasView.getWidth(), canvasView.getHeight());
            canvasView.setOnCanvasTouchListener(this);
        }

        mChart.clear();
        mChart.setCanvas(canvasView);
        mChart.padding(10, 30, 10, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_1688.json"));
        mChart.interaction("pinch");

        mChart.line().fixedShape("smooth").fixedColor("#FF2900").position("date*value");
        mChart.area().position("date*value").fixedShape("smooth").fixedColor("#FFEDE9");
        mChart.point().position("date*value").fixedColor("#FF2900");

        mChart.setAxis("date",
                new F2Chart.AxisConfigBuilder()
                        .line(new F2Chart.AxisLineConfigBuilder().lineWidth(1).color("#DCDEE3"))
                        .label(new F2Chart.AxisLabelConfigBuilder()
                                .labelOffset(8.0f)
                                .labelMargin(8.0f))
                        .gridHidden());
//        mChart.setAxis("value",
//                new F2Chart.AxisConfigBuilder()
//                        .hidden());

        mChart.setScale("date",
                new F2Chart.ScaleConfigBuilder()
                        .setOption("nice", true)
                        .tickCount(6));
        mChart.setScale("value",
                new F2Chart.ScaleConfigBuilder()
                        .setOption("nice", true)
                        .tickCount(3));
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