package com.antgroup.antv.f2;


/**
 * finanacechart 仓库脚本自动替换当前类
 *
 * @author qingyuan.yl
 */
public class F2Event {

    // native canvas init调用
    public static void eventPage(String appId, String bizType, String desc) {
    }

    /**
     * native canvas绘制调用
     *
     * @param renderDuration c++渲染是时长
     * @param renderSuccess  c++渲染是否成功
     * @param renderCmdCount c++侧渲染的指令
     * @param drawSuccess    native侧上屏是否成功
     * @param bitmapBytes    白屏的二进制图片，非白屏的时候为null
     * @param detectDuration 截屏&分析白屏的时间消耗
     * @param desc           chartId+扩展描述
     */
    public static void eventDetectRender(String appId, String bizType, long renderDuration, boolean renderSuccess,
                                         int renderCmdCount, boolean drawSuccess, byte[] bitmapBytes, long detectDuration,
                                         int width, int height, double ratio, String desc) {
    }

}
