package com.antgroup.antv.f2;

import com.alipay.antgraphic.AntGfxLauncher;

import java.util.Map;

/**
 *  AntG 初始化。
 *  Alipay 和 标准模式下 api 不一样。
 */
class AntGLauncher {

    private static volatile boolean sAntGLaunched = false;
    public static void init() {
        if (!sAntGLaunched) {
            AntGfxLauncher.InitConfig initConfig = new AntGfxLauncher.InitConfig();
            initConfig.loadSkiaSo = true;
            AntGfxLauncher.initAsync(initConfig, new AntGfxLauncher.InitCallback() {

                @Override
                public void onInitFinish(int i, Map<String, Object> map) {
                    F2Log.i("F2CanvasView", "onInitFinish, " + map);
                }
            });
            sAntGLaunched = true;
        }
    }
}
