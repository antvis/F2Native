package com.antgroup.antv.f2.service;

import android.graphics.Bitmap;
import android.graphics.Typeface;

public interface F2CommonService {

    /**
     * load so
     */
    void loadLibrary(String libraryName, Class<?>[] nativeClasses);

    void sendRenderDetectEvent(final Bitmap snapshotBitmap, final String bizType, final long renderDuration, final boolean renderSuccess,
                               final int renderCmdCount, final boolean drawSuccess,
                               final int width, final int height, final double ratio, final String chartId, final boolean isFirstRender);

    /**
     * 事件上报
     *
     * @param renderDuration c++渲染是时长
     * @param renderSuccess  c++渲染是否成功
     * @param renderCmdCount c++侧渲染的指令
     * @param drawSuccess    native侧上屏是否成功
     * @param bitmapBytes    白屏的二进制图片，非白屏的时候为null
     * @param detectDuration 截屏&分析白屏的时间消耗
     * @param desc           chartId+扩展描述
     * @param isFirstRender  是否首次绘制
     */
    void eventDetectRender(String bizType, long renderDuration, boolean renderSuccess,
                           int renderCmdCount, boolean drawSuccess, byte[] bitmapBytes, long detectDuration,
                           int width, int height, double ratio, String desc, final boolean isFirstRender);

    /**
     * 是否开启白屏检测开关
     */
    boolean isDetectEnable();

    boolean isChartSizeChangeEnable();

    Typeface getAlipayNumber(String familyName);

    void sendLongClickBroadcast();

    void sendLongClickStartBroadcast();

    void sendLongClickFinishBroadcast();

}