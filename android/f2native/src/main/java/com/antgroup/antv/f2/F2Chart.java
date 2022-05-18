package com.antgroup.antv.f2;

import android.content.Context;
import android.text.TextUtils;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * @author qingyuan.yl
 * @date 2020-09-16
 */
public class F2Chart {

    private NativeChartProxy mChartProxy;
    private String mName;
    private F2CanvasView mCanvasView;
    private volatile boolean hasDestroyed = false;
    private boolean mLogCmdCount = true;

    private RequestAnimationFrameHandle mRequestFrameHandle = null;

    public static F2Chart create(Context context, String name, double widthPixel, double heightPixel) {
        double ratio = context.getResources().getDisplayMetrics().density;
        return new F2Chart(name, widthPixel / ratio, heightPixel / ratio, ratio);
    }

    private F2Chart(String name, double width, double height, double ratio) {
        mName = name;
        mChartProxy = new NativeChartProxy(name, width, height, ratio);
    }

    public F2CanvasView getCanvasView() {
        return mCanvasView;
    }

    public void setCanvas(F2CanvasView canvasView) {
        innerLog("#bind canvas: " + canvasView.getNativeCanvas());
        mCanvasView = canvasView;
        if (hasDestroyed) {
            return;
        }
        assertRenderThread();
        if (mRequestFrameHandle != null) {
            mRequestFrameHandle.clear();
        }
        mRequestFrameHandle = new RequestAnimationFrameHandle(this, canvasView);
        mChartProxy.setCanvas(mCanvasView.getNativeCanvas(), mRequestFrameHandle.functionId);
    }

    public F2Chart padding(final double left, final double top, final double right, final double bottom) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setPadding(left, top, right, bottom);
        return this;
    }

    public F2Chart margin(final double left, final double top, final double right, final double bottom) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setMargin(left, top, right, bottom);
        return this;
    }

    public F2Chart source(final String jsonData) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.source(jsonData);
        return this;
    }

    public F2Chart setScale(final String field, final ScaleConfigBuilder builder) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setScale(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart setScale(final String field, final String scaleJsonConfig) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setScale(field, scaleJsonConfig);
        return this;
    }

    public F2Chart setAxis(final String field, final AxisConfigBuilder builder) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setAxis(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart setAxis(final String field, final String axisJsonConfig) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setAxis(field, axisJsonConfig);
        return this;
    }

    public F2Chart setCoord(CoordConfigBuilder builder) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setCoord(builder.build().toJsonString());
        return this;
    }

    public F2Chart setCoord(String coordJsonConfig) {
        if (hasDestroyed) {
            return this;
        }
        assertRenderThread();
        mChartProxy.setCoord(coordJsonConfig);
        return this;
    }

    public boolean postTouchEvent(F2CanvasView.TouchEvent event) {
        if (hasDestroyed) {
            return false;
        }
        assertRenderThread();
        // long ts = System.currentTimeMillis();
        int ret = mChartProxy.sendTouchEvent(event.getData().toJsonString());
        if (ret == 1) {
            // long swTs = System.currentTimeMillis();
            mCanvasView.swapBuffer();
            // long now = System.currentTimeMillis();
            // innerLog("#postTouchEvent duration: " + (now - ts) +"ms(with swap:"+(now - swTs)+"ms)");
        }
        return ret == 1;
    }

    public F2Geom.Line line() {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return (F2Geom.Line) mChartProxy.createGeom(this, "line");
    }

    public F2Geom.Area area() {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return (F2Geom.Area) mChartProxy.createGeom(this, "area");
    }

    public F2Geom.Point point() {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return (F2Geom.Point) mChartProxy.createGeom(this, "point");
    }

    public F2Geom.Interval interval() {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return (F2Geom.Interval) mChartProxy.createGeom(this, "interval");
    }

    public F2Geom.Candle candle() {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return (F2Geom.Candle) mChartProxy.createGeom(this, "candle");
    }

    public F2Chart interaction(String type) {
        interaction(type, (String) null);
        return this;
    }

    public F2Chart interaction(String type, F2Config config) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setInteraction(type, config.toJsonString());
        return this;
    }

    public F2Chart interaction(String type, String interactionConfig) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setInteraction(type, interactionConfig);
        return this;
    }

    public F2Chart tooltip(ToolTipConfigBuilder builder) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setToolTip(builder.build().toJsonString());
        return this;
    }

    public F2Chart tooltip(String tooltipJsonConfig) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setToolTip(tooltipJsonConfig);
        return this;
    }

    public F2Chart legend(String field, LegendConfigBuild builder) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setLegend(field, builder.build().toJsonString());
        return this;
    }

    public F2Chart legend(String field, String legendJsonConfig) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setLegend(field, legendJsonConfig);
        return this;
    }

    public F2Chart animate(boolean enable) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setAnimate("" + enable);
        return this;
    }

    public F2Chart animate(ChartAnimateConfigBuild animateConfigBuild) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setAnimate(animateConfigBuild.build().toJsonString());
        return this;
    }

    public F2Chart animate(String animateJsonConfig) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        mChartProxy.setAnimate(animateJsonConfig);
        return this;
    }

    public F2Guide guide() {
        assertRenderThread();
        return new F2Guide(this);
    }

    public boolean render(boolean swapBuffer) {
        if (hasDestroyed) {
            return false;
        }
        assertRenderThread();
        // 渲染是否成功
        boolean isRenderSuccess = mChartProxy.render();
        // 获取渲染指令的个数
        int renderCount = getRenderCmdCount();
        // 获取渲染指令耗时
        int renderDuration = getRenderDuration();
        // 上屏是否成功
        boolean isDrawSuccess = false;
        if (isRenderSuccess && swapBuffer) {
            isDrawSuccess = mCanvasView.swapBuffer();
        }
        // 上报绘制检测，包含白屏检测
        mCanvasView.sendRenderDetectEvent(renderDuration, isRenderSuccess, renderCount, isDrawSuccess, getChartId());
        return isRenderSuccess;
    }

    private int getRenderDuration() {
        if (hasDestroyed) {
            return 0;
        }
        assertRenderThread();
        return mChartProxy.getRenderDuration();
    }

    private int getRenderCmdCount() {
        if (hasDestroyed) {
            return 0;
        }
        assertRenderThread();
        return mChartProxy.getRenderCmdCount();
    }

    private String getChartId() {
        if (hasDestroyed) {
            return "";
        }
        assertRenderThread();
        return mChartProxy.getChartId();
    }

    public boolean render() {
        return render(true);
    }

    public void setLogCmdCount(boolean logCmdCount) {
        mLogCmdCount = logCmdCount;
    }

    public double[] getPosition(String itemJsonData) {
        if (hasDestroyed) {
            return new double[]{0, 0};
        }
        assertRenderThread();
        return mChartProxy.getPosition(itemJsonData);
    }

    // this method is unsafe, it allows calling from main thread without throwing
    // exception. Caller should make sure that this API is not called during rendering
    public String getTooltipInfos(float touchX, float touchY, int geomIndex) {
        if (hasDestroyed) {
            return null;
        }
        return mChartProxy.getTooltipInfos(touchX, touchY, geomIndex);
    }

    public void clear() {
        if (hasDestroyed) {
            return;
        }
        assertRenderThread();
        mChartProxy.clear();
    }

    public String getRenderDumpInfo() {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return mChartProxy.getRenderDumpInfo();
    }

    public String getScaleTicks(String field) {
        if (hasDestroyed) {
            return null;
        }
        assertRenderThread();
        return mChartProxy.getScaleTicks(field);
    }

    public void destroy() {
        if (hasDestroyed) {
            return;
        }
        innerLog("#destroy");
        if (mRequestFrameHandle != null) {
            mRequestFrameHandle.clear();
        }
        mChartProxy.destroy();
        hasDestroyed = true;
    }

    public boolean isDestroyed() {
        return hasDestroyed;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            innerLog("#finalize..");
            destroy();
        } catch (Exception e) {
            F2Log.e("F2Chart", "#finalize exception " + e.toString());
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

        public T textAlign(JSONArray textAlign) {
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
                F2Log.e("F2Chart", "#range exception " + e.toString());
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
        private static final String KEY_LINE_DASH = "dash";

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
        private static final String KEY_DASH = "dash";

        public AxisLineConfigBuilder type(String type) {
            return setOption(KEY_TYPE, type);
        }

        public AxisLineConfigBuilder lineWidth(float lineWidth) {
            return setOption(KEY_LINE_WIDTH, lineWidth);
        }

        public AxisLineConfigBuilder color(String color) {
            return setOption(KEY_COLOR, color);
        }

        public AxisLineConfigBuilder lineDash(double[] dashParam) {
            return setOption(KEY_DASH, dashParam);
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

    public static class GeomAnimateConfigBuild extends F2Config.Builder<GeomAnimateConfigBuild> {
        private static final String KEY_ANIMATE_TYPE = "animate";
        private static final String KEY_ERASING = "erasing";
        private static final String KEY_DELAY = "delay";
        private static final String KEY_DURATION = "duration";

        public GeomAnimateConfigBuild animateType(String type) {
            return setOption(KEY_ANIMATE_TYPE, type);
        }

        public GeomAnimateConfigBuild erasing(String erasing) {
            return setOption(KEY_ERASING, erasing);
        }

        public GeomAnimateConfigBuild delay(int delay) {
            return setOption(KEY_DELAY, delay);
        }

        public GeomAnimateConfigBuild duration(int duration) {
            return setOption(KEY_DURATION, duration);
        }
    }

    public static class ChartAnimateConfigBuild extends F2Config.Builder<ChartAnimateConfigBuild> {
        public ChartAnimateConfigBuild geom(String type, GeomAnimateConfigBuild build) {
            return setOption(type, build.build().mConfig);
        }
    }


}
