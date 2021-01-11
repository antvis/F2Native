package com.antgroup.antv.f2;

import org.json.JSONArray;

/**
 * @author qingyuan.yl
 * @date 2020-09-23
 */
public class F2Guide {

    private F2Chart mChart;

    F2Guide(F2Chart chart) {
        mChart = chart;
    }

    public void flag(GuideFlagConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideFlag(config.build().toJsonString());
    }

    public void text(GuideTextConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideText(config.build().toJsonString());
    }

    public static class GuideFlagConfigBuilder extends F2Chart.TextConfigBuilder<GuideFlagConfigBuilder> {
        private static final String KEY_CONTENT = "content";
        private static final String KEY_PADDING = "padding";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_BACK_COLOR = "backgroundColor";
        private static final String KEY_POSITION = "position";

        public GuideFlagConfigBuilder content(String content) {
            return setOption(KEY_CONTENT, content);
        }

        /**
         * @param position ["min", "max"]
         * @return
         */
        public GuideFlagConfigBuilder position(String[] position ) {
            return setOption(KEY_POSITION, position);
        }

        public GuideFlagConfigBuilder position(JSONArray position ) {
            return setOption(KEY_POSITION, position);
        }

        /**
         * @param padding [left, top, right, bottom]
         * @return
         */
        public GuideFlagConfigBuilder padding(double[] padding) {
            return setOption(KEY_PADDING, padding);
        }

        public GuideFlagConfigBuilder lineWidth(double lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public GuideFlagConfigBuilder backgroudColor(String backgroundColor) {
            return setOption(KEY_BACK_COLOR, backgroundColor);
        }
    }

    public static class GuideTextConfigBuilder extends F2Chart.TextConfigBuilder<GuideTextConfigBuilder> {
        private static final String KEY_CONTENT = "content";
        private static final String KEY_MARGIN = "margin";
        private static final String KEY_POSITION = "position";

        public GuideTextConfigBuilder content(String content) {
            return setOption(KEY_CONTENT, content);
        }

        /**
         * @param position ["min", "max"]
         * @return
         */
        public GuideTextConfigBuilder position(String[] position ) {
            return setOption(KEY_POSITION, position);
        }

        public GuideTextConfigBuilder position(JSONArray position ) {
            return setOption(KEY_POSITION, position);
        }

        /**
         * @param margin [top, right]
         * @return
         */
        public GuideTextConfigBuilder margin(double[] margin) {
            return setOption(KEY_MARGIN, margin);
        }
    }
}
