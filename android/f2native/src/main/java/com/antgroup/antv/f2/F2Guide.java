package com.antgroup.antv.f2;

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

    public static class GuideFlagConfigBuilder extends F2Chart.TextConfigBuilder {
        private static final String KEY_CONTENT = "content";
        private static final String KEY_PADDING = "padding";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_BACK_COLOR = "backgroundColor";
        @Override
        public GuideFlagConfigBuilder textAlign(String textAlign) {
            super.textAlign(textAlign);
            return this;
        }

        @Override
        public GuideFlagConfigBuilder textSize(float textSize) {
            super.textSize(textSize);
            return this;
        }

        @Override
        public GuideFlagConfigBuilder textColor(String color) {
            super.textColor(color);
            return this;
        }

        @Override
        public GuideFlagConfigBuilder textBaseline(String textBaseline) {
            super.textBaseline(textBaseline);
            return this;
        }

        public GuideFlagConfigBuilder content(String content) {
            setOption(KEY_CONTENT, content);
            return this;
        }

        /**
         * @param padding [left, top, right, bottom]
         * @return
         */
        public GuideFlagConfigBuilder padding(double[] padding) {
            setOption(KEY_PADDING, padding);
            return this;
        }

        public GuideFlagConfigBuilder lineWidth(double lineWidth) {
            setOption(KEY_LINE_WIDTH, lineWidth);
            return this;
        }

        public GuideFlagConfigBuilder backgroudColor(String backgroundColor) {
            setOption(KEY_BACK_COLOR, backgroundColor);
            return this;
        }
    }

    public static class GuideTextConfigBuilder extends F2Chart.TextConfigBuilder {
        private static final String KEY_CONTENT = "content";
        private static final String KEY_MARGIN = "margin";
        private static final String KEY_POSITION = "position";

        @Override
        public GuideTextConfigBuilder textAlign(String textAlign) {
            super.textAlign(textAlign);
            return this;
        }

        @Override
        public GuideTextConfigBuilder textSize(float textSize) {
            super.textSize(textSize);
            return this;
        }

        @Override
        public GuideTextConfigBuilder textColor(String color) {
            super.textColor(color);
            return this;
        }

        @Override
        public GuideTextConfigBuilder textBaseline(String textBaseline) {
            super.textBaseline(textBaseline);
            return this;
        }

        public GuideTextConfigBuilder content(String content) {
            setOption(KEY_CONTENT, content);
            return this;
        }

        /**
         * @param position ["min", "max"]
         * @return
         */
        public GuideTextConfigBuilder position(String[] position ) {
            setOption(KEY_POSITION, position);
            return this;
        }

        /**
         * @param margin [top, right]
         * @return
         */
        public GuideTextConfigBuilder margin(double[] margin) {
            setOption(KEY_MARGIN, margin);
            return this;
        }
    }
}
