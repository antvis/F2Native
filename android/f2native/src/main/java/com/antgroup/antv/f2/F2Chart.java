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
    private volatile boolean hasDestroyed = false;

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
        mChartProxy.setCanvas(mCanvasView.getNativeCanvas());
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

    public F2Chart setScale(final String field, final String scaleJsonConfig) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setScale(field, scaleJsonConfig);
        return this;
    }

    public F2Chart setAxis(final String field, final AxisConfigBuilder builder) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setAxis(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart setAxis(final String field, final String axisJsonConfig) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setAxis(field, axisJsonConfig);
        return this;
    }

    public F2Chart setCoord(CoordConfigBuilder builder) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setCoord(builder.build().toJsonString());
        return this;
    }

    public F2Chart setCoord(String coordJsonConfig) {
        if (hasDestroyed) return this;
        assertRenderThread();
        mChartProxy.setCoord(coordJsonConfig);
        return this;
    }

    public F2Chart postTouchEvent(F2CanvasView.TouchEvent event) {
        if (hasDestroyed) return null;
        assertRenderThread();
        // long ts = System.currentTimeMillis();
        int ret = mChartProxy.sendTouchEvent(event.getData().toJsonString());
        if (ret == 1) {
            // long swTs = System.currentTimeMillis();
            mCanvasView.swapBuffer();
            // long now = System.currentTimeMillis();
            // innerLog("#postTouchEvent duration: " + (now - ts) +"ms(with swap:"+(now - swTs)+"ms)");
        }
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

    public F2Geom.Point point() {
        if (hasDestroyed) return null;
        assertRenderThread();
        return (F2Geom.Point) mChartProxy.createGeom(this, "point");
    }

    public F2Geom.Interval interval() {
        if (hasDestroyed) return null;
        assertRenderThread();
        return (F2Geom.Interval) mChartProxy.createGeom(this, "interval");
    }

    public F2Chart interaction(String type) {
        interaction(type, (String) null);
        return this;
    }

    public F2Chart interaction(String type, F2Config config) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setInteraction(type, config.toJsonString());
        return this;
    }

    public F2Chart interaction(String type, String interactionConfig) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setInteraction(type, interactionConfig);
        return this;
    }

    public F2Chart tooltip(ToolTipConfigBuilder builder) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setToolTip(builder.build().toJsonString());
        return this;
    }

    public F2Chart tooltip(String tooltipJsonConfig) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setToolTip(tooltipJsonConfig);
        return this;
    }

    public F2Chart legend(String field, LegendConfigBuild builder) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setLegend(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart legend(String field, String legendJsonConfig) {
        if (hasDestroyed) return null;
        assertRenderThread();
        mChartProxy.setLegend(field, legendJsonConfig);
        return this;
    }

    public F2Guide guide() {
        assertRenderThread();
        return new F2Guide(this);
    }

    public void render() {
        if (hasDestroyed) return;
        assertRenderThread();
        int code = mChartProxy.render();
        if (F2Constants.isSuccessCode(code)) {
            mCanvasView.swapBuffer();
        }
        String dumpInfo = getRenderDumpInfo();
        innerLog("#render ret: " + code + ", dumpInfo: " + dumpInfo);
    }

    public double[] getPosition(String itemJsonData) {
        if (hasDestroyed) return new double[]{0, 0};
        assertRenderThread();
        return mChartProxy.getPosition(itemJsonData);
    }

    public void clear() {
        if (hasDestroyed) return;
        assertRenderThread();
        mChartProxy.clear();
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

    public String getName() {
        return mName;
    }

    final NativeChartProxy getChartProxy() {
        return mChartProxy;
    }

    private void innerLog(String content) {
        F2Log.i("F2Chart-" + mName, content);
    }

    final void assertRenderThread() {
        if (hasDestroyed || mCanvasView == null) {
            F2Log.e("F2Chart-" + mName, "#runOnRenderThread chartView is null.");
            return;
        }
        if (!mCanvasView.isOnCanvasThread()) {
            throw new RuntimeException("F2Chart operations must on render thread.");
        }
    }

    public static class TextConfigBuilder<T extends TextConfigBuilder> extends F2Config.Builder<T> {
        private static final String KEY_TEXT_COLOR = "textColor";
        private static final String KEY_TEXT_SIZE = "textSize";
        private static final String KEY_TEXT_ALIGN = "textAlign";
        private static final String KEY_BASE_LINE = "textBaseline";

        public T textColor(String color) {
            return setOption(KEY_TEXT_COLOR, color);
        }

        public T textSize(float textSize) {
            return setOption(KEY_TEXT_SIZE, textSize);
        }

        public T textAlign(String textAlign) {
            return setOption(KEY_TEXT_ALIGN, textAlign);
        }

        public T textBaseline(String textBaseline) {
            return setOption(KEY_BASE_LINE, textBaseline);
        }
    }

    public static class ScaleConfigBuilder extends F2Config.Builder<ScaleConfigBuilder> {
        private static final String KEY_TYPE = "type";
        private static final String KEY_PRECISION = "precision";
        private static final String KEY_RANGE = "range";
//        private static final String KEY_SCALE_RANGE = "scaleRange";
        private static final String KEY_TICK_COUNT = "tickCount";
        private static final String KEY_MAX = "max";
        private static final String KEY_MIN = "min";
        private static final String KEY_TICK = "tick";
        private static final String KEY_NICE = "nice";

        public ScaleConfigBuilder type(String type) {
            return setOption(KEY_TYPE, type);
        }

        public ScaleConfigBuilder precision(int precision) {
            return setOption(KEY_PRECISION, precision);
        }

        public ScaleConfigBuilder range(double range[]) {
            JSONArray array = new JSONArray();
            try {
                for (int i = 0; i < range.length; i++) {
                    array.put(range[i]);
                }
            } catch (Exception e) {
            }
            return setOption(KEY_RANGE, array);
        }

        public ScaleConfigBuilder range(JSONArray jsonArray) {
            return setOption(KEY_RANGE, jsonArray);
        }

        public ScaleConfigBuilder tickCount(int tickCount) {
            return setOption(KEY_TICK_COUNT, tickCount);
        }

        public ScaleConfigBuilder max(double max) {
            return setOption(KEY_MAX, max);
        }

        public ScaleConfigBuilder min(double min) {
            return setOption(KEY_MIN, min);
        }

        public ScaleConfigBuilder tick(F2Chart chart, F2Function function) {
            return setOption(KEY_TICK, chart, function);
        }

        public ScaleConfigBuilder nice(boolean nice) {
            return setOption(KEY_NICE, nice);
        }
    }

    public static class AxisGridConfigBuilder extends F2Config.Builder<AxisGridConfigBuilder> {
        private static final String KEY_TYPE = "type";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_STROKE = "stroke";
        private static final String KEY_LINE_DASH = "lineDash";

        public AxisGridConfigBuilder type(String type) {
            return setOption(KEY_TYPE, type);
        }

        public AxisGridConfigBuilder lineWidth(float lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public AxisGridConfigBuilder stroke(String color) {
            return setOption(KEY_STROKE, color);
        }

        public AxisGridConfigBuilder lineDash(double[] dashParam) {
            return setOption(KEY_LINE_DASH, dashParam);
        }
    }

    public static class AxisLineConfigBuilder extends F2Config.Builder<AxisLineConfigBuilder> {
        private static final String KEY_TYPE = "type";
        private static final String KEY_LINE_WIDTH = "lineWidth";
        private static final String KEY_COLOR = "color";

        public AxisLineConfigBuilder type(String type) {
            return setOption(KEY_TYPE, type);
        }

        public AxisLineConfigBuilder lineWidth(float lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public AxisLineConfigBuilder color(String color) {
            return setOption(KEY_COLOR, color);
        }
    }

    public static class AxisLabelConfigBuilder extends TextConfigBuilder<AxisLabelConfigBuilder> {

        private static final String KEY_LABEL_MARGIN = "labelMargin";
        private static final String KEY_LABEL_OFFSET = "labelOffset";
        private static final String KEY_LABEL_ITEM = "item";

        public AxisLabelConfigBuilder labelMargin(float labelMargin) {
            return setOption(KEY_LABEL_MARGIN, labelMargin);
        }

        public AxisLabelConfigBuilder labelOffset(float labelOffset) {
            return setOption(KEY_LABEL_OFFSET, labelOffset);
        }

        public AxisLabelConfigBuilder item(F2Chart chart, F2Function function) {
            return setOption(KEY_LABEL_ITEM, chart, function);
        }
    }

    public static class AxisConfigBuilder extends F2Config.Builder<AxisConfigBuilder> {
        private static final String KEY_LABEL = "label";
        private static final String KEY_GRID = "grid";
        private static final String KEY_LINE = "line";
        private static final String KEY_HIDDEN = "hidden";

        public AxisConfigBuilder label(AxisLabelConfigBuilder builder) {
            return setOption(KEY_LABEL, builder.options);
        }

        public AxisConfigBuilder labelHidden() {
            return setOption(KEY_LABEL, false);
        }

        public AxisConfigBuilder line(AxisLineConfigBuilder builder) {
            return setOption(KEY_LINE, builder.options);
        }

        public AxisConfigBuilder lineHidden() {
            return setOption(KEY_LINE, false);
        }

        public AxisConfigBuilder grid(AxisGridConfigBuilder builder) {
            return setOption(KEY_GRID, builder.options);
        }

        public AxisConfigBuilder gridHidden() {
            return setOption(KEY_GRID, false);
        }

        public AxisConfigBuilder hidden() {
            return setOption(KEY_HIDDEN, true);
        }
    }

    public static class CoordConfigBuilder extends F2Config.Builder<CoordConfigBuilder> {
        private static final String KEY_TRANSPOSED = "transposed";
        private static final String KEY_TYPE = "type";

        public CoordConfigBuilder transposed(boolean transposed) {
            return setOption(KEY_TRANSPOSED, transposed);
        }

        public CoordConfigBuilder type(String type) {
            return setOption(KEY_TYPE, type);
        }
    }

    public static class ToolTipConfigBuilder extends F2Config.Builder<ToolTipConfigBuilder> {

    }

    public static class LegendConfigBuild extends F2Config.Builder<LegendConfigBuild> {
        private static final String KEY_SYMBOL = "symbol";
        private static final String KEY_ENABLE = "enable";
        private static final String KEY_POSITION = "position";
        private static final String KEY_LAYOUT = "layout";
        private static final String KEY_LINE_BOTTOM = "lineBottom";
        private static final String KEY_ITEM_MARGIN_BOTTOM = "itemMarginBottom";
        private static final String KEY_WORD_SPACE = "wordSpace";

        public LegendConfigBuild symbol(String symbol) {
            return setOption(KEY_SYMBOL, symbol);
        }

        public LegendConfigBuild enable(boolean enable) {
            return setOption(KEY_ENABLE, enable);
        }

        public LegendConfigBuild position(String position) {
            return setOption(KEY_POSITION, position);
        }

        public LegendConfigBuild layout(String layout) {
            return setOption(KEY_LAYOUT, layout);
        }

        public LegendConfigBuild lineBottom(double lineBottom) {
            return setOption(KEY_LINE_BOTTOM, lineBottom);
        }

        public LegendConfigBuild wordSpace(double wordSpace) {
            return setOption(KEY_WORD_SPACE, wordSpace);
        }

        public LegendConfigBuild itemMarginBottom(double itemMarginBottom) {
            return setOption(KEY_ITEM_MARGIN_BOTTOM, itemMarginBottom);
        }
    }

}
