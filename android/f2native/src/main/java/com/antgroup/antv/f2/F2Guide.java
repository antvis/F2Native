package com.antgroup.antv.f2;

import org.json.JSONArray;

/**
 * https://yuque.antfin.com/financeteam/rtdmco/xeabm3lzgr1thvgx
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
        mChart.getChartProxy().setGuideType("flag", config.build().toJsonString());
    }

    public void text(GuideTextConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideType("text", config.build().toJsonString());
    }

    public void line(GuideLineConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideType("line", config.build().toJsonString());
    }

    public void background(GuideBackgroundConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideType("background", config.build().toJsonString());
    }

    public void point(GuidePointConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideType("point", config.build().toJsonString());
    }

    public void tag(GuideTagConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideType("tag", config.build().toJsonString());
    }

    public void image(GuideImageConfigBuilder config) {
        mChart.assertRenderThread();
        mChart.getChartProxy().setGuideType("image", config.build().toJsonString());
    }

    public static class GuideFlagConfigBuilder extends F2Chart.TextConfigBuilder<GuideFlagConfigBuilder> {
        private static final String KEY_CONTENT = "content";
        private static final String KEY_PADDING = "padding";
        private static final String KEY_COLOR = "color";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_BACK_COLOR = "backgroundColor";
        private static final String KEY_POSITION = "position";

        public GuideFlagConfigBuilder content(String content) {
            return setOption(KEY_CONTENT, content);
        }

        /**
         * @param position ["min", "max"]
         */
        public GuideFlagConfigBuilder position(String[] position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideFlagConfigBuilder position(JSONArray position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideFlagConfigBuilder color(String color) {
            return setOption(KEY_COLOR, color);
        }

        /**
         * @param padding [left, top, right, bottom]
         */
        public GuideFlagConfigBuilder padding(double[] padding) {
            return setOption(KEY_PADDING, padding);
        }

        public GuideFlagConfigBuilder lineWidth(double lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public GuideFlagConfigBuilder backgroundColor(String backgroundColor) {
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
         */
        public GuideTextConfigBuilder position(String[] position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideTextConfigBuilder position(JSONArray position) {
            return setOption(KEY_POSITION, position);
        }

        /**
         * @param margin [top, right]
         */
        public GuideTextConfigBuilder margin(double[] margin) {
            return setOption(KEY_MARGIN, margin);
        }
    }

    public static class GuideLineConfigBuilder extends F2Config.Builder<GuideLineConfigBuilder> {
        private static final String KEY_POSITION = "position";
        private static final String KEY_LINE_WIDTH = "lineWith";
        private static final String KEY_COLOR = "color";
        private static final String KEY_ORIENTATION = "orientation";
        private static final String KEY_DASH = "dash";
        private static final String KEY_TOP = "top";

        public GuideLineConfigBuilder position(String[] position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideLineConfigBuilder position(JSONArray position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideLineConfigBuilder color(String color) {
            return setOption(KEY_COLOR, color);
        }

        public GuideLineConfigBuilder color(F2Util.ColorGradient color) {
            return setOption(KEY_COLOR, color);
        }

        public GuideLineConfigBuilder lineWidth(float lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public GuideLineConfigBuilder orientation(String orientation) {
            return setOption(KEY_ORIENTATION, orientation);
        }

        public GuideLineConfigBuilder dash(double[] dash) {
            return setOption(KEY_DASH, dash);
        }

        public GuideLineConfigBuilder top(boolean top) {
            return setOption(KEY_TOP, top);
        }
    }

    public static class GuideBackgroundConfigBuilder extends F2Config.Builder<GuideBackgroundConfigBuilder> {
        private static final String KEY_LEFT_BOTTOM = "leftBottom";
        private static final String KEY_RIGHT_TOP = "rightTop";
        private static final String KEY_COLOR = "color";

        public GuideBackgroundConfigBuilder leftBottom(String[] position) {
            return setOption(KEY_LEFT_BOTTOM, position);
        }

        public GuideBackgroundConfigBuilder leftBottom(JSONArray position) {
            return setOption(KEY_LEFT_BOTTOM, position);
        }

        public GuideBackgroundConfigBuilder rightTop(String[] position) {
            return setOption(KEY_RIGHT_TOP, position);
        }

        public GuideBackgroundConfigBuilder rightTop(JSONArray position) {
            return setOption(KEY_RIGHT_TOP, position);
        }

        public GuideBackgroundConfigBuilder color(String color) {
            return setOption(KEY_COLOR, color);
        }

        public GuideBackgroundConfigBuilder color(F2Util.ColorGradient color) {
            return setOption(KEY_COLOR, color);
        }

    }

    public static class GuidePointConfigBuilder extends F2Config.Builder<GuidePointConfigBuilder> {
        private static final String KEY_POSITION = "position";
        private static final String KEY_LINE_WIDTH = "lineWith";
        private static final String KEY_FILL = "fill";
        private static final String KEY_STROKE = "stroke";
        private static final String KEY_SHAPE = "shape";
        private static final String KEY_SIZE = "size";
        private static final String KEY_MARGIN = "margin";

        public GuidePointConfigBuilder position(String[] position) {
            return setOption(KEY_POSITION, position);
        }

        public GuidePointConfigBuilder position(JSONArray position) {
            return setOption(KEY_POSITION, position);
        }

        public GuidePointConfigBuilder fill(String color) {
            return setOption(KEY_FILL, color);
        }

        public GuidePointConfigBuilder stroke(String color) {
            return setOption(KEY_STROKE, color);
        }

        public GuidePointConfigBuilder lineWidth(float lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public GuidePointConfigBuilder shape(String type) {
            return setOption(KEY_SHAPE, type);
        }

        public GuidePointConfigBuilder size(float size) {
            return setOption(KEY_SIZE, size);
        }

        public GuidePointConfigBuilder margin(double[] margin) {
            return setOption(KEY_MARGIN, margin);
        }
    }

    public static class GuideTagConfigBuilder extends F2Chart.TextConfigBuilder<GuideTagConfigBuilder> {
        private static final String KEY_POSITION = "position";
        private static final String KEY_LINE_WIDTH = "lineWith";
        private static final String KEY_FILL = "fill";
        private static final String KEY_STROKE = "stroke";

        private static final String KEY_DIRECT = "direct";
        private static final String KEY_PADDING = "padding";
        private static final String KEY_RADIUS = "radius";
        private static final String KEY_CONTENT = "content";
        private static final String KEY_SIDE = "sideSize";


        public GuideTagConfigBuilder position(String[] position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideTagConfigBuilder position(JSONArray position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideTagConfigBuilder fill(String color) {
            return setOption(KEY_FILL, color);
        }

        public GuideTagConfigBuilder stroke(String color) {
            return setOption(KEY_STROKE, color);
        }


        public GuideTagConfigBuilder lineWidth(float lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public GuideTagConfigBuilder direct(String direct) {
            return setOption(KEY_DIRECT, direct);
        }

        public GuideTagConfigBuilder padding(double[] padding) {
            return setOption(KEY_PADDING, padding);
        }

        public GuideTagConfigBuilder content(String content) {
            return setOption(KEY_CONTENT, content);
        }

        public GuideTagConfigBuilder radius(float radius) {
            return setOption(KEY_RADIUS, radius);
        }

        public GuideTagConfigBuilder sideSize(float sideSize) {
            return setOption(KEY_SIDE, sideSize);
        }
    }

    public static class GuideImageConfigBuilder extends F2Config.Builder<GuideImageConfigBuilder> {
        private static final String KEY_SRC = "src";
        private static final String KEY_POSITION = "position";
        private static final String KEY_MARGIN = "margin";
        private static final String KEY_IMAGE_WIDTH = "width";
        private static final String KEY_IMAGE_HEIGHT = "height";

        public GuideImageConfigBuilder position(String[] position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideImageConfigBuilder position(JSONArray position) {
            return setOption(KEY_POSITION, position);
        }

        public GuideImageConfigBuilder src(String src) {
            return setOption(KEY_SRC, src);
        }

        public GuideImageConfigBuilder margin(double[] margin) {
            return setOption(KEY_MARGIN, margin);
        }

        public GuideImageConfigBuilder width(int imageWith) {
            return setOption(KEY_IMAGE_WIDTH, imageWith);
        }

        public GuideImageConfigBuilder height(int imageHeight) {
            return setOption(KEY_IMAGE_HEIGHT, imageHeight);
        }
    }
}
