package com.antgroup.antv.f2.service;

import android.graphics.Bitmap;
import android.graphics.Typeface;
import android.text.TextUtils;

public class F2CommonDefault implements F2CommonService {

    @Override
    public void loadLibrary(String libraryName, Class<?>[] nativeClasses) {
        if (TextUtils.isEmpty(libraryName)) {
            return;
        }
        System.loadLibrary(libraryName);
    }

    @Override
    public void sendRenderDetectEvent(Bitmap snapshotBitmap, String bizType, long renderDuration, boolean renderSuccess, int renderCmdCount, boolean drawSuccess, int width, int height, double ratio, String chartId, boolean isFirstRender) {

    }

    @Override
    public void eventDetectRender(String bizType, long renderDuration, boolean renderSuccess,
                                  int renderCmdCount, boolean drawSuccess, byte[] bitmapBytes, long detectDuration,
                                  int width, int height, double ratio, String desc, final boolean isFirstRender) {
    }

    @Override
    public boolean isDetectEnable() {
        // kF2NativeReleaseProductDetKey
        return true;
    }

    @Override
    public boolean isChartSizeChangeEnable() {
        return true;
    }

    @Override
    public Typeface getAlipayNumber(String familyName) {
        return null;
    }

    @Override
    public void sendLongClickBroadcast() {
    }

    @Override
    public void sendLongClickStartBroadcast() {

    }

    @Override
    public void sendLongClickFinishBroadcast() {

    }
}
