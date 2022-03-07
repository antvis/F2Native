package com.antgroup.antv.f2;


/**
 * finanacechart 仓库脚本自动替换当前类
 * @author qingyuan.yl
 * @date 2020/12/4
 */
public class F2Event {

    public static void eventRender(String appId, String bizType, int index, int cmdCount, long duration, int width, int height, double ratio) {
    }

    // isAndroidCanvas 用于区分canvas， isAndroidCanvas ? "android_canvas" : "antg"; key:"xg_render"
    public static void eventRender(String appId, String bizType, int index, int cmdCount, long duration, int width, int height, double ratio, boolean isAndroidCanvas) {
    }

    public static void eventPage(String appId, String bizType, String desc) {
    }

    // isAndroidCanvas 用于区分canvas， isAndroidCanvas ? "android_canvas" : "antg"; key:"xg_render"
    public static void eventPage(String appId, String bizType, String desc, boolean isAndroidCanvas) {
    }
}
