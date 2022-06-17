package com.antgroup.antv.f2;

import java.util.LinkedHashSet;
import java.util.Set;

import org.json.JSONObject;

import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;

/**
 * c++反射调用的类
 * author by luna
 * date on 2022/5/28
 */

public class F2BridgeRailing {

    private final long mRailingHandle;
    private Set<Long> mCommands = new LinkedHashSet<>();
    private Handler mHandler;
    private F2CanvasView mF2CanvasView;

    public F2BridgeRailing(F2CanvasView f2CanvasView) {
        mF2CanvasView = f2CanvasView;
        mRailingHandle = F2ChartBridge.nCreateNativeRailHandle(this);
    }

    public long getRailingHandle() {
        return mRailingHandle;
    }

    public void destroy() {
        F2ChartBridge.nDestroyNativeRailHandle(mRailingHandle);
    }

    /**
     * c++反射调用，播放动画
     */
    public final void playAnimation(String param) {
        if (TextUtils.isEmpty(param)) {
            return;
        }
        long pointer = 0;
        long delay = 16;
        try {
            JSONObject json = new JSONObject(param);
            if (!json.has("command")) {
                return;
            }
            pointer = json.getLong("command");
            if (json.has("delay")) {
                delay = json.getLong("delay");
            }
        } catch (Exception e) {
            F2Log.e("F2BridgeRailing", " playAnimation exception: " + e.getMessage());
        }

        final long command = pointer;
        mCommands.add(command);
        removeHandlerCallbacks();
        mHandler = new Handler(Looper.getMainLooper());
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!mCommands.contains(command)) {
                    return;
                }
                NativeChartProxy.executeCommand(command);
                swap();
                mCommands.remove(command);
            }
        }, delay);
        return;
    }

    /**
     * c++反射调用，触发刷新
     */
    public void swap() {
        if (mF2CanvasView != null) {
            mF2CanvasView.swapBuffer();
        }
    }

    /**
     * c++反射调用，以格林威治为标准时间，计算时间差
     */
    public long getTimezoneOffset(String timezone) {
        return F2Util.getTimeZoneOffset(System.currentTimeMillis(), timezone);
    }

    /**
     * c++反射调用，获取format的时间
     */
    public String formatTime(String dateString, String timeZoneStr, String timestampFormatter) {
        return F2Util.getFormatTime(dateString, timeZoneStr, timestampFormatter);
    }

    private void removeHandlerCallbacks() {
        if (mHandler != null) {
            mHandler.removeCallbacks(null);
            mHandler = null;
        }
    }

}
