package com.antgroup.antv.f2;

/**
 * bridge jni proxy
 * author by luna
 * date on 2022/5/27
 */

public class F2ChartBridge {

    private F2ChartBridgeListener mF2ChartBridgeListener;
    private long chartBridgeHandle;


    public F2ChartBridge(long railHandle, long nativeCanvas, double width, double height, double ratio, double rpxRatio) {
        chartBridgeHandle = nCreate(railHandle, nativeCanvas, width, height, ratio, rpxRatio);
    }

    /**
     * cubeBridge invokeMethod 调用
     */
    public void invokeMethod(String methodName, String params, F2ChartBridgeListener callback) {
        this.mF2ChartBridgeListener = callback;
        if (chartBridgeHandle != 0) {
            nInvokeMethod(chartBridgeHandle, this, methodName, params);
        }
    }

    /**
     * c++ jni调用，c++的callbakc收到回调之后会反射调用此处
     */
    public void invokeCallback(String result) {
        if (this.mF2ChartBridgeListener != null) {
            this.mF2ChartBridgeListener.onResult(
                    mF2ChartBridgeListener.getMethodName(), mF2ChartBridgeListener.getBizId(), result);
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
     * 设置是否需要tooltip
     */
    public void setNeedTooltip(boolean tooltip) {
        if (chartBridgeHandle != 0) {
            nSetNeedTooltip(chartBridgeHandle, tooltip);
        }
    }

    public void destroy() {
        if (chartBridgeHandle != 0) {
            nDestroy(chartBridgeHandle);
            chartBridgeHandle = 0;
            mF2ChartBridgeListener = null;
        }
    }


    private native static long nCreate(long railHandle, long nativeCanvas, double width, double height, double ratio, double rpxRatio);

    private native static void nDestroy(long chartBridgeHandle);

    private native static void nInvokeMethod(long chartBridgeHandle, Object o, String methodName, String params);

    private native static boolean nSendTouchEvent(long chartBridgeHandle, String toJsonString);

    public native static void nSetNeedTooltip(long chartBridgeHandle, boolean tooltip);

    public native static long nCreateNativeRailHandle(Object o);

    public native static void nDestroyNativeRailHandle(long canvasHolder);


}
