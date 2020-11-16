package com.antgroup.antv.f2;

import android.content.Context;

import org.json.JSONArray;

/**
 * @author qingyuan.yl
 * @date 2020-09-16
 */
public class F2Chart {

    private NativeChartProxy mChartProxy;
    private String mName;
    private F2CanvasView mCanvasView;
    private boolean hasDestroyed = false;

    public static F2Chart create(Context context, String name, double widthPixel, double heightPixel) {
        double ratio = context.getResources().getDisplayMetrics().density;
        return new F2Chart(name, widthPixel / ratio, heightPixel / ratio, ratio);
    }

    private F2Chart(String name, double width, double height, double ratio) {
        mName = name;

        mChartProxy = new NativeChartProxy(name, width, height, ratio);
    }

    public void setCanvas(F2CanvasView canvasView) {
        innerLog("#bind canvas: " + canvasView);
        mCanvasView = canvasView;
        if (hasDestroyed) return;
        assertRenderThread();
        mChartProxy.setCanvas(mCanvasView.getCanvasHolder().getNativeCanvasProxy());
    }

    public F2Chart padding(final double left, final double top, final double right, final double bottom) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setPadding(left, top, right, bottom);
        innerLog("#padding");
        return this;
    }

    public F2Chart margin(final double left, final double top, final double right, final double bottom) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setMargin(left, top, right, bottom);
        return this;
    }

    public F2Chart source(final String jsonData) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.source(jsonData);
        return this;
    }

    public F2Chart setScale(final String field, final ScaleConfigBuilder builder) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setScale(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart setAxis(final String field, final AxisConfigBuilder builder) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setAxis(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart setCoord(CoordConfigBuilder builder) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setCoord(builder.build().toJsonString());
        return this;
    }

    public F2Chart postTouchEvent(F2CanvasView.TouchEvent event) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.sendTouchEvent(event.getData().toJsonString());
        mCanvasView.swapBuffer();
        return this;
    }

    public F2Geom.Line line() {
        if (hasDestroyed) return null;
        assertRenderThread();
        return (F2Geom.Line) mChartProxy.createGeom(this, "line");
    }

    public F2Geom.Area area() {
        if (hasDestroyed) return null;
        assertRenderThread();
        return (F2Geom.Area) mChartProxy.createGeom(this, "area");
    }

    public F2Geom.Interval interval() {
        if (hasDestroyed) return null;
        assertRenderThread();
        return (F2Geom.Interval) mChartProxy.createGeom(this, "interval");
    }

    public F2Chart interaction(String type) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setInteraction(type);
        return this;
    }

    public F2Chart tooltip(ToolTipConfigBuilder builder) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setToolTip(builder.build().toJsonString());
        return this;
    }

    public F2Chart legend(String field, LegendConfigBuild builder) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setLegend(field, builder.build().toJsonString());
        return this;
    }

    public F2Guide guide() {
        return null;
    }

    public void render() {
        if (hasDestroyed) return;
        assertRenderThread();
        int code = mChartProxy.render();
        if (F2Constants.isSuccessCode(code)) {
            mCanvasView.swapBuffer();
        }
        String dumpInfo = getRenderDumpInfo();
        innerLog("#render dumpInfo: " + dumpInfo);

    }

    public String getRenderDumpInfo() {
        if (hasDestroyed) return null;
        assertRenderThread();
        return mChartProxy.getRenderDumpInfo();
    }

    public void destroy() {
        if (hasDestroyed) return;
        innerLog("#destroy");
        mChartProxy.destroy();
        hasDestroyed = true;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            innerLog("#finalize..");
            destroy();
        } catch (Exception e) {
        } finally {
            super.finalize();
        }
    }

    NativeChartProxy getChartProxy() {
        return mChartProxy;
    }

    private void innerLog(String content) {
        F2Log.i("F2Chart-" + mName, content);
    }

    void assertRenderThread() {
        if (hasDestroyed || mCanvasView == null || mCanvasView.getCanvasHolder() == null) {
            F2Log.e("F2Chart-" + mName, "#runOnRenderThread chartView is null.");
            return;
        }
        if (!mCanvasView.getCanvasHolder().isOnRenderThread()) {
            throw new RuntimeException("F2Chart operations must on render thread.");
        }
    }

    public static class ScaleConfigBuilder extends F2Config.Builder {
        private static final String KEY_TYPE = "type";
        private static final String KEY_PRECISION = "precision";
        private static final String KEY_RANGE = "range";
        private static final String KEY_SCALE_RANGE = "scaleRange";
        private static final String KEY_TICK_COUNT = "tickCount";
        private static final String KEY_MAX = "max";
        private static final String KEY_min = "min";

        public ScaleConfigBuilder type(String type) {
            setOption(KEY_TYPE, type);
            return this;
        }

        public ScaleConfigBuilder precision(int precision) {
            setOption(KEY_PRECISION, precision);
            return this;
        }

        public ScaleConfigBuilder range(double range[]) {
            JSONArray array = new JSONArray();
            try {
                for (int i = 0; i < range.length; i++) {
                    array.put(range[i]);
                }
            } catch (Exception e) {
            }
            setOption(KEY_RANGE, array);
            return this;
        }

        public ScaleConfigBuilder tickCount(int tickCount) {
            setOption(KEY_TICK_COUNT, tickCount);
            return this;
        }

        public ScaleConfigBuilder max(double max) {
            setOption(KEY_MAX, max);
            return this;
        }

        public ScaleConfigBuilder min(double min) {
            setOption(KEY_min, min);
            return this;
        }
    }

    public static class AxisGridConfigBuilder extends F2Config.Builder {
        private static final String KEY_TYPE = "type";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_STROKE = "stroke";
        private static final String KEY_LINE_DASH = "lineDash";

        public AxisGridConfigBuilder type(String type) {
            setOption(KEY_TYPE, type);
            return this;
        }

        public AxisGridConfigBuilder lineWidth(float lineWidth) {
            setOption(KEY_LINE_WIDTH, lineWidth);
            return this;
        }

        public AxisGridConfigBuilder stroke(String color) {
            setOption(KEY_STROKE, color);
            return this;
        }

        public AxisGridConfigBuilder lineDash(double[] dashParam) {
            setOption(KEY_LINE_DASH, dashParam);
            return this;
        }
    }

    public static class AxisLineConfigBuilder extends F2Config.Builder {
        private static final String KEY_TYPE = "type";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_COLOR = "color";

        public AxisLineConfigBuilder type(String type) {
            setOption(KEY_TYPE, type);
            return this;
        }

        public AxisLineConfigBuilder lineWidth(float lineWidth) {
            setOption(KEY_LINE_WIDTH, lineWidth);
            return this;
        }

        public AxisLineConfigBuilder color(String color) {
            setOption(KEY_COLOR, color);
            return this;
        }
    }

    public static class AxisLabelConfigBuilder extends F2Config.Builder {
        private static final String KEY_TEXT_COLOR = "textColor";
        private static final String KEY_TEXT_SIZE = "textSize";
        private static final String KEY_LABEL_MARGIN = "labelMargin";
        private static final String KEY_LABEL_OFFSET = "labelOffset";
        private static final String KEY_TEXT_ALIGN = "textAlign";
        private static final String KEY_BASE_LINE = "textBaseline";

        public AxisLabelConfigBuilder textColor(String color) {
            setOption(KEY_TEXT_COLOR, color);
            return this;
        }

        public AxisLabelConfigBuilder textSize(float textSize) {
            setOption(KEY_TEXT_SIZE, textSize);
            return this;
        }

        public AxisLabelConfigBuilder labelMargin(float labelMargin) {
            setOption(KEY_LABEL_MARGIN, labelMargin);
            return this;
        }

        public AxisLabelConfigBuilder labelOffset(float labelOffset) {
            setOption(KEY_LABEL_OFFSET, labelOffset);
            return this;
        }

        public AxisLabelConfigBuilder textAlign(String textAlign) {
            setOption(KEY_TEXT_ALIGN, textAlign);
            return this;
        }

        public AxisLabelConfigBuilder textBaseline(String textBaseline) {
            setOption(KEY_BASE_LINE, textBaseline);
            return this;
        }
    }

    public static class AxisConfigBuilder extends F2Config.Builder {
        private static final String KEY_LABEL = "label";
        private static final String KEY_GRID = "grid";
        private static final String KEY_LINE = "line";
        private static final String KEY_HIDDEN = "hidden";

        public AxisConfigBuilder label(AxisLabelConfigBuilder builder) {
            setOption(KEY_LABEL, builder.options);
            return this;
        }

        public AxisConfigBuilder labelHidden() {
            setOption(KEY_LABEL, false);
            return this;
        }

        public AxisConfigBuilder line(AxisLineConfigBuilder builder) {
            setOption(KEY_LINE, builder.options);
            return this;
        }

        public AxisConfigBuilder lineHidden() {
            setOption(KEY_LINE, false);
            return this;
        }

        public AxisConfigBuilder grid(AxisGridConfigBuilder builder) {
            setOption(KEY_GRID, builder.options);
            return this;
        }

        public AxisConfigBuilder gridHidden() {
            setOption(KEY_GRID, false);
            return this;
        }

        public AxisConfigBuilder hidden(boolean hidden) {
            setOption(KEY_HIDDEN, hidden);
            return this;
        }
    }

    public static class CoordConfigBuilder extends F2Config.Builder {
        private static final String KEY_TRANSPOSED = "transposed";
        private static final String KEY_TYPE= "type";

        public CoordConfigBuilder transposed(boolean transposed) {
            setOption(KEY_TRANSPOSED, transposed);
            return this;
        }

        public CoordConfigBuilder type(String type) {
            setOption(KEY_TYPE, type);
            return this;
        }
    }

    public static class ToolTipConfigBuilder extends F2Config.Builder {

    }

    public static class LegendConfigBuild extends F2Config.Builder {
        private static final String KEY_SYMBOL = "symbol";
        private static final String KEY_ENABLE = "enable";
        private static final String KEY_POSITION = "position";
        private static final String KEY_LAYOUT = "layout";
        private static final String KEY_LINE_BOTTOM = "lineBottom";
        private static final String KEY_ITEM_MARGIN_BOTTOM = "itemMarginBottom";
        private static final String KEY_WORD_SPACE = "wordSpace";

        @Override
        public LegendConfigBuild setOption(String key, int value) {
            super.setOption(key, value);
            return this;
        }

        public LegendConfigBuild symbol(String symbol) {
            setOption(KEY_SYMBOL, symbol);
            return this;
        }

        public LegendConfigBuild enable(boolean enable) {
            setOption(KEY_ENABLE, enable);
            return this;
        }

        public LegendConfigBuild position(String position) {
            setOption(KEY_POSITION, position);
            return this;
        }

        public LegendConfigBuild layout(String layout) {
            setOption(KEY_LAYOUT, layout);
            return this;
        }

        public LegendConfigBuild lineBottom(double lineBottom) {
            setOption(KEY_LINE_BOTTOM, lineBottom);
            return this;
        }

        public LegendConfigBuild wordSpace(double wordSpace) {
            setOption(KEY_WORD_SPACE, wordSpace);
            return this;
        }

        public LegendConfigBuild itemMarginBottom(double itemMarginBottom) {
            setOption(KEY_ITEM_MARGIN_BOTTOM, itemMarginBottom);
            return this;
        }
    }

}
