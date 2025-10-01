package com.antgroup.antv.f2.samples.charts;

import android.text.TextUtils;
import android.view.MotionEvent;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.F2Log;
import com.antgroup.antv.f2.gesture.F2GestureListener;
import com.antgroup.antv.f2.samples.Utils;

import java.util.HashMap;
import java.util.List;

public class KLineWeekCandleChart implements F2CanvasView.Adapter, F2CanvasView.OnCanvasTouchListener,
        F2CanvasView.OnCanvasGestureListener {
    private static final String TAG = KLineWeekCandleChart.class.getSimpleName();
    private F2Chart mCandleChart;

    @Override
    public void onCanvasDraw(final F2CanvasView canvasView) {
        if (mCandleChart == null) {
            canvasView.setOnCanvasTouchListener(this);
            canvasView.setOnCanvasGestureListener(this);

            mCandleChart = F2Chart.create(canvasView.getContext(), "KlineWeek", canvasView.getWidth(), canvasView.getHeight() * 3 / 4);
        }
        mCandleChart.setCanvas(canvasView);
        mCandleChart.padding(15, 10, 0, 0);
        String s = Utils.loadAssetFile(canvasView.getContext(), "mockData_klineweek_1.json");
        List<JSONObject> objects = JSONArray.parseArray(s, JSONObject.class);
        int size = objects.size();
        F2Log.get().i("KLineWeekCandleChart", "onCanvasDraw size: " + size);

        mCandleChart.source(s);
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
                        .type("kline-week")
                        .setOption("domain", new double[]{size - 50, size - 1})
        );
        mCandleChart.setScale("value", new F2Chart.ScaleConfigBuilder().nice(true));

        mCandleChart.interaction("pan");
        mCandleChart.interaction("pinch");
        mCandleChart.interaction("tap");

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
        if (mCandleChart != null) {
            mCandleChart.postTouchEvent(event);
        }
    }

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        if (mCandleChart != null) {
            String jsonString = F2GestureListener.createSinglePointEvent(event.getX(), event.getY(), "tap");
            F2Log.get().i(TAG, "onSingleTapUp  " + jsonString);
            mCandleChart.postTouchEvent(jsonString);
            return true;
        }
        return false;
    }

    private String mLongPressEventType;
    private String mScrollEventType;
    private String mFlingEventType;
    private String mScaleEventType;

    @Override
    public void onLongPress(MotionEvent e, boolean isPress, String eventType) {
        if (TextUtils.isEmpty(mLongPressEventType)) {
            String jsonString = F2GestureListener.createSinglePointEvent(e.getX(), e.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_START_TYPE);
            mCandleChart.postTouchEvent(jsonString);
        }
        mCandleChart.postTouchEvent(F2GestureListener.createSinglePointEvent(e.getX(), e.getY(), eventType));
        mLongPressEventType = isPress ? eventType : "";
    }

    @Override
    public void onTouchChange(MotionEvent e, boolean isDown) {
        HashMap<String, Object> map = new HashMap<>();
        map.put("touchEvent", isDown ? "touchstart" : "touchend");
//        sendTouchEvent(map);
        F2Log.get().i(TAG, "onTouchChange  " + isDown);
        if (!isDown) {
            mCandleChart.postTouchEvent(F2GestureListener.createSinglePointEvent(e.getX(), e.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_END_TYPE));
            mScrollEventType = "";
            mFlingEventType = "";
            mLongPressEventType = "";
            mScaleEventType = "";
        }
    }

    @Override
    public boolean onScale(MotionEvent e, boolean isScale, String eventType) {
        if (mCandleChart != null) {
            if (TextUtils.isEmpty(mScaleEventType)) {
                String jsonString = F2GestureListener.createMorePointEvent(e, F2CanvasView.OnCanvasGestureListener.TOUCH_START_TYPE);
                mCandleChart.postTouchEvent(jsonString);
            }

            String jsonString = F2GestureListener.createMorePointEvent(e, eventType);
            F2Log.get().i(TAG, "onScale  " + jsonString);
            mCandleChart.postTouchEvent(jsonString);
            return true;
        }
        mScaleEventType = isScale ? eventType : "";

        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        if (TextUtils.isEmpty(mScrollEventType)) {
            String jsonString = F2GestureListener.createSinglePointEvent(e1.getX(), e1.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_START_TYPE);
            F2Log.get().i(TAG, "onScroll start " + jsonString);
            mCandleChart.postTouchEvent(jsonString);
        }
        String jsonString = F2GestureListener.createSinglePointEvent(e2.getX(), e2.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_MOVE_TYPE);
        F2Log.get().i(TAG, "onScroll " + jsonString);
        mCandleChart.postTouchEvent(jsonString);
        return true;
    }
}