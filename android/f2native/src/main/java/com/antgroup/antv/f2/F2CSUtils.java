package com.antgroup.antv.f2;


/**
 * finanacechart 仓库sync.js脚本自动替换当前类
 */
public class F2CSUtils {

    // kF2NativeCanvasBackendKey true为使用新的canvas。iOS使用CoreGraphics实现的canvas，android使用java实现的canvas，双端共用
    public static boolean isAndroidCanvas() {
        return true;
    }

    public static void showUseNativeCanvasToast() {
    }
}
