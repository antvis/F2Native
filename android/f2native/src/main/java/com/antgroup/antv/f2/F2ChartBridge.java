package com.antgroup.antv.f2;

import java.util.Map;

import android.text.TextUtils;

/**
 * bridge jni proxy
 * author by luna
 * date on 2022/5/27
 */

public class F2ChartBridge {

    private IF2ChartInvokeCallback mF2ChartBridgeListener;
    private long chartBridgeHandle;


    public F2ChartBridge(long railHandle, long nativeCanvas, double width, double height, double ratio, double rpxRatio) {
        try {
            if (railHandle != 0 && nativeCanvas != 0) {
                chartBridgeHandle = nCreate(railHandle, nativeCanvas, width, height, ratio, rpxRatio);
            } else {
                F2Log.get().e("F2ChartBridge", " nCreate error 0");
            }
        } catch (Throwable t) {
            F2Log.get().e("F2ChartBridge", " nCreate error " + t.getMessage());
            chartBridgeHandle = 0;
        }
    }

    public long getChartBridgeHandle() {
        return chartBridgeHandle;
    }

    public void setF2ChartInvokeCallback(IF2ChartInvokeCallback callback) {
        this.mF2ChartBridgeListener = callback;
    }

    /**
     * cubeBridge invokeMethod 调用
     */
    public void invokeMethod(String methodName, String params) {
        if (chartBridgeHandle != 0) {
            nInvokeMethod(chartBridgeHandle, this, methodName, params);
        }
    }

    /**
     * c++ jni调用，c++的callbakc收到回调之后会反射调用此处
     */
    public void nInvokeCallback(String result) {
        if (this.mF2ChartBridgeListener != null) {
            this.mF2ChartBridgeListener.onResult(result);
        }
    }

    /**
     * touch事件
     */
    public boolean sendTouchEvent(F2CanvasView.TouchEvent event) {
        if (event == null) {
            return false;
        }
        if (chartBridgeHandle != 0) {
            return nSendTouchEvent(chartBridgeHandle, event.getData().toJsonString());
        }
        return false;
    }


    /**
     * touch事件
     */
    public boolean sendTouchEvent(String eventJsonString) {
        if (TextUtils.isEmpty(eventJsonString)) {
            return false;
        }
        if (chartBridgeHandle != 0) {
            return nSendTouchEvent(chartBridgeHandle, eventJsonString);
        }
        return false;
    }

    /**
     * 设置是否需要tooltip
     */
    public void setNeedTooltip(boolean tooltip) {
        if (chartBridgeHandle != 0) {
            nSetNeedTooltip(chartBridgeHandle, tooltip);
        }
    }

    public void setEnableListConfig(String enableConfig) {
        if (chartBridgeHandle != 0) {
            nSetEnableListConfig(chartBridgeHandle, enableConfig);
        }
    }

    public void destroy() {
        if (chartBridgeHandle != 0) {
            nDestroy(chartBridgeHandle);
            chartBridgeHandle = 0;
            mF2ChartBridgeListener = null;
        }
    }

    public void updateThemeToken(Map<Integer, Integer> replaceColorTokens, boolean isDark) {
        if (chartBridgeHandle != 0) {
            nUpdateThemeToken(chartBridgeHandle, replaceColorTokens, isDark);
        }
    }

    /**
     * 点击事件json
     */
    public String onTapEvent(String tapJson) {
        if (chartBridgeHandle != 0) {
            return nOnTapEvent(chartBridgeHandle, tapJson);
        }
        return "";
    }

    private native static long nCreate(long railHandle, long nativeCanvas, double width, double height, double ratio, double rpxRatio);

    private native static void nDestroy(long chartBridgeHandle);

    private native static void nInvokeMethod(long chartBridgeHandle, Object o, String methodName, String params);

    private native static boolean nSendTouchEvent(long chartBridgeHandle, String toJsonString);

    public native static void nSetNeedTooltip(long chartBridgeHandle, boolean tooltip);

    public native static long nCreateNativeRailHandle(Object o);

    public native static void nDestroyNativeRailHandle(long canvasHolder);

    public native static void nSetEnableListConfig(long chartBridgeHandle, String enableConfig);

    public native static void nUpdateThemeToken(long chartBridgeHandle,
                                                Map<Integer, Integer> replaceColorTokens, boolean isDarkMode);

    private native static String nOnTapEvent(long chartBridgeHandle, String toJsonString);

}
