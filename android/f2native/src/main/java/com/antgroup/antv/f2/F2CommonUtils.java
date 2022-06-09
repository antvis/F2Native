package com.antgroup.antv.f2;


import android.text.TextUtils;

/**
 * finanacechart 仓库sync.js脚本自动替换当前类
 * cs开关和通用帮助工具类
 */
public class F2CommonUtils {

    /**
     * load so
     */
    public static void loadLibrary(String libraryName) {
        if (TextUtils.isEmpty(libraryName)) {
            return;
        }
        System.loadLibrary(libraryName);
    }

    /**
     * 事件上报
     *
     * @param renderDuration c++渲染是时长
     * @param renderSuccess c++渲染是否成功
     * @param renderCmdCount c++侧渲染的指令
     * @param drawSuccess native侧上屏是否成功
     * @param bitmapBytes 白屏的二进制图片，非白屏的时候为null
     * @param detectDuration 截屏&分析白屏的时间消耗
     * @param desc chartId+扩展描述
     */
    public static void eventDetectRender(String appId, String bizType, long renderDuration, boolean renderSuccess,
                                         int renderCmdCount, boolean drawSuccess, byte[] bitmapBytes, long detectDuration,
                                         int width, int height, double ratio, String desc) {
    }

    /**
     * 显示测试toast
     */
    public static void showTestToast(String content) {
    }

    /**
     * 是否开启白屏检测开关
     */
    public static boolean isDetectEnable() {
        // kF2NativeReleaseProductDetKey
        return true;
    }

}