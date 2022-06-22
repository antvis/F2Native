package com.antgroup.antv.f2.kotlinsamples.charts;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.F2Util;
import com.antgroup.antv.f2.kotlinsamples.Utils;

/**
 * @author qingyuan.yl
 * @date 2020-09-27
 */
public class SingleAreaChart_1 implements F2CanvasView.Adapter {

    private F2Chart mChart;

    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "SingleAreaChart_1", canvasView.getWidth(), canvasView.getHeight());
        }

        mChart.setCanvas(canvasView);
        mChart.padding(10, 0, 0, 0);
        mChart.source(Utils.loadAssetFile(canvasView.getContext(), "mockData_singleAreaChart.json"));

        mChart.setAxis("time", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelOffset(5.f))
                .gridHidden());

        mChart.setAxis("tem", new F2Chart.AxisConfigBuilder()
                .grid(new F2Chart.AxisGridConfigBuilder().type("dash")));
        mChart.setScale("tem", new F2Chart.ScaleConfigBuilder().min(800).max(3000));

        mChart.line().position("time*tem");
        mChart.area().position("time*tem").fixedColor(
                new F2Util.ColorLinearGradient()
                        .addColorStop(0.f, "red")
                        .addColorStop(1.f, "white")
                        .setPosition(0, 0, 0, canvasView.getHeight())
        );
        mChart.point().position("time*tem").fixedColor("#FFB6C1").style(new F2Config.Builder()
                .setOption("custom", mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        JSONObject jsonObject = JSON.parseObject(param);
                        if (jsonObject.getIntValue("_index") % 2 == 0) {
                            return new F2Config.Builder().setOption("size", 0).build();
                        }
                        return null;
                    }
                })
                .build());

        mChart.render();
    }

    @Override
    public void onDestroy() {
        if (mChart != null) {
            mChart.destroy();
        }
    }
}
