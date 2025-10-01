package com.antgroup.antv.f2;

import android.os.Handler;
import android.os.Looper;

import org.json.JSONObject;

import java.util.LinkedHashSet;
import java.util.Set;

/**
 * @author qingyuan.yl
 * @date 2021/2/23
 */
final class RequestAnimationFrameHandle extends F2Function {
    private F2Chart mChart;
    private F2CanvasView mCanvasView;
    private String mChartName;
    private Handler mHandler;

    private Set<Long> mCommands = new LinkedHashSet<>();

    RequestAnimationFrameHandle(F2Chart chart, F2CanvasView canvasView) {
        super();
        mChart = chart;
        mChartName = mChart.getName();
        mCanvasView = canvasView;
        bindChart(chart);
    }

    public void clear() {
        for (Long command : mCommands) {
            NativeChartProxy.deallocCommand(command);
        }
        mCommands.clear();
        removeHandlerCallbacks();
    }

    @Override
    protected void finalize() throws Throwable {
        F2Log.get().i(mChartName + "", "RequestAnimationFrameHandle finalize .. " + functionId);
        super.finalize();
    }

    @Override
    public F2Config execute(String param) {
        if (mChart == null || mChart.isDestroyed()) {
            return null;
        }
        long pointer = 0;
        long delay = 16;
        try {
            JSONObject json = new JSONObject(param);
            if (!json.has("command")) {
                return null;
            }
            pointer = json.getLong("command");
            if (json.has("delay")) {
                delay = json.getLong("delay");
            }
        } catch (Exception e) {
        }

        final long command = pointer;
        mCommands.add(command);
        removeHandlerCallbacks();
        mHandler = new Handler(Looper.getMainLooper());
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (mChart == null || mChart.isDestroyed() || !mCommands.contains(command)) {
                    return;
                }
                NativeChartProxy.executeCommand(command);
                mCanvasView.swapBuffer();
                mCommands.remove(command);
            }
        }, delay);
        return null;
    }

    private void removeHandlerCallbacks() {
        if (mHandler != null) {
            mHandler.removeCallbacks(null);
            mHandler = null;
        }
    }
}
