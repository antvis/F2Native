package com.antgroup.antv.f2.samples.charts;

import android.os.Handler;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Chart;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Function;
import com.antgroup.antv.f2.F2Guide;
import com.antgroup.antv.f2.samples.Utils;

import java.util.List;

/**
 * @author qingyuan.yl
 * @version 10.1.1
 * @date 2020/12/22
 */
public class MKTrendChart implements F2CanvasView.Adapter {
    private Handler mHandler = new Handler();
    private F2Chart mChart;

    JSONObject mJsonData = null;
    JSONArray mFlagsData = null;
    volatile int rangeIndex = 1;
    boolean stopLoop = false;

    private F2CanvasView mCanvasView = null;

    private void startLoop() {
        if (stopLoop) return;
        rangeIndex ++;
        if (mCanvasView != null) {
            mCanvasView.postCanvasDraw();
        }
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                startLoop();
            }
        }, 1000);
    }


    @Override
    public void onCanvasDraw(F2CanvasView canvasView) {
        mCanvasView = canvasView;
        if (mChart == null) {
            mChart = F2Chart.create(canvasView.getContext(), "MKTrendChart", canvasView.getWidth(), canvasView.getHeight());

            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    startLoop();
                }
            }, 1000);
        }

        mChart.clear();
        mChart.setCanvas(canvasView);
        mChart.padding(0, 0, 0, 0);

        if (mJsonData == null) {
            mJsonData = JSON.parseObject(Utils.loadAssetFile(canvasView.getContext(), "mockData_trendChart.json"));
        }
        JSONArray sourceData = mJsonData.getJSONArray("items");
        List subData = sourceData.subList(0, rangeIndex % sourceData.size());
        mChart.source(JSON.toJSONString(subData));

        try {
            JSONArray timeRange = mJsonData.getJSONArray("timeRange");
            org.json.JSONArray timeRangeCopy = new org.json.JSONArray(timeRange.toJSONString());
            mChart.setScale("date", new F2Chart.ScaleConfigBuilder().type("timeSharing").setOption("timeRange", timeRangeCopy));
        } catch (Exception e) {
            e.printStackTrace();
        }

        F2Chart.ScaleConfigBuilder priceScaleCfg = new F2Chart.ScaleConfigBuilder();
        priceScaleCfg.tickCount(2);
        priceScaleCfg.min(mJsonData.getDouble("min") - 2).max(mJsonData.getDouble("max") + 2);
        mChart.setScale("price", priceScaleCfg);

        mChart.setAxis("date", new F2Chart.AxisConfigBuilder()
                .label(new F2Chart.AxisLabelConfigBuilder().labelMargin(20).labelOffset(4).textColor("#999999").item(mChart, new F2Function() {
                    @Override
                    public F2Config execute(String param) {
                        JSONObject json = JSON.parseObject(param);
                        int index = json.getIntValue("index");
                        if (index == 0) {
                            return new F2Config.Builder().setOption("content", "09:30").build();
                        } else if (index == 1) {
                            return new F2Config.Builder().setOption("content", "11:30/13:00").build();
                        } else if (index == 2){
                            return new F2Config.Builder().setOption("content", "15:00").build();
                        } else {
                            return new F2Config.Builder().setOption("content", "").build();
                        }
                    }
                }))
                .line(new F2Chart.AxisLineConfigBuilder().color("#DDDDDD"))
        );
        mChart.setAxis("price", new F2Chart.AxisConfigBuilder().hidden());

        mChart.line().position("date*price").fixedColor("#528EFF");
        mChart.area().position("date*price").fixedColor("#108EE9");

        JSONArray flagsData = JSONArray.parseArray(Utils.loadAssetFile(canvasView.getContext(), "mockData_trendChart_flags.json"));

        for (int i = 0; i < flagsData.size(); i++) {
            try {
                JSONObject flagCfg = flagsData.getJSONObject(i);
                F2Guide.GuideFlagConfigBuilder cfg = new F2Guide.GuideFlagConfigBuilder();
                JSONArray position = flagCfg.getJSONArray("position");
                cfg.position(new org.json.JSONArray(position.toJSONString()))
                        .content(flagCfg.getString("content"))
                        .padding(new double[]{5, 3, 5, 1})
                        .textSize(10)
                        .lineWidth(0.5);

                if (i == flagsData.size() - 1) {
                    cfg.textColor("#ffffff");
                    cfg.backgroundColor("#108EE9");
                } else {
                    cfg.textColor("#108EE9");
                    cfg.backgroundColor("#C7E7FF");
                }
                mChart.guide().flag(cfg);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        mChart.render();
    }

    @Override
    public void onDestroy() {
        stopLoop = true;
        if (mChart != null) {
            mChart.destroy();
        }
        mCanvasView = null;
    }
}
