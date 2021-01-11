package com.antgroup.antv.f2;

/**
 * @author qingyuan.yl
 * @date 2020-09-23
 */
public class F2Geom {

    protected F2Chart mF2Chart;
    protected long mNativeGeomHandler = 0;
    protected String mType;

    F2Geom(F2Chart chart, long nativeGeomHandler, String type) {
        mF2Chart = chart;
        this.mNativeGeomHandler = nativeGeomHandler;
        mType = type;
    }

    long getNativeGeomHandler() {
        return mNativeGeomHandler;
    }

    public String getType() {
        return mType;
    }

    public F2Geom position(final String field) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomPosition(F2Geom.this, field);
        return this;
    }

    public F2Geom color(final String field) {
        return color(field, new String[0]);
    }

    public F2Geom color(final String field, final String[] colors) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomColor(F2Geom.this, field, colors);
        return this;
    }

    public F2Geom fixedColor(final String color) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomColor(F2Geom.this, color);
        return this;
    }

    public F2Geom size(final String field, final float[] sizes) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomSize(F2Geom.this, field, sizes);
        return this;
    }

    public F2Geom fixedSize(final float size) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomSize(F2Geom.this, size);
        return this;
    }

    public F2Geom shape(final String field, final String[] shapes) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomShape(F2Geom.this, field, shapes);
        return this;
    }

    public F2Geom fixedShape(final String shape) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomShape(F2Geom.this, shape);

        return this;
    }

    public F2Geom adjust(final String type) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomAdjust(F2Geom.this, type);
        return this;
    }

    public F2Geom style(final F2Config config) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomStyle(this, config.toJsonString());
        return this;
    }

    public F2Geom style(final String jsonConfig) {
        mF2Chart.assertRenderThread();
        mF2Chart.getChartProxy().setGeomStyle(this, jsonConfig);
        return this;
    }

    public static class Line extends F2Geom {

        Line(F2Chart chart, long nativeGeomHandler, String type) {
            super(chart, nativeGeomHandler, type);
        }
    }

    public static class Area extends F2Geom {
        Area(F2Chart chart, long nativeGeomHandler, String type) {
            super(chart, nativeGeomHandler, type);
        }
    }

    public static class Point extends F2Geom {
        Point(F2Chart chart, long nativeGeomHandler, String type) {
            super(chart, nativeGeomHandler, type);
        }
    }

    public static class Interval extends F2Geom {
        Interval(F2Chart chart, long nativeGeomHandler, String type) {
            super(chart, nativeGeomHandler, type);
        }

        @Override
        public Interval position(String field) {
            return (Interval) super.position(field);
        }

        @Override
        public Interval fixedColor(String color) {
            return (Interval) super.fixedColor(color);
        }

        @Override
        public Interval color(String field) {
            return (Interval) super.color(field);
        }

        @Override
        public Interval size(String field, float[] sizes) {
            return (Interval) super.size(field, sizes);
        }

        @Override
        public Interval fixedSize(float size) {
            return (Interval) super.fixedSize(size);
        }

        @Override
        public Interval shape(String field, String[] shapes) {
            return (Interval) super.shape(field, shapes);
        }

        @Override
        public Interval fixedShape(String shape) {
            return (Interval) super.fixedShape(shape);
        }

        public Interval tag(final F2Config config) {
            mF2Chart.assertRenderThread();
            mF2Chart.getChartProxy().setGeomIntervalTag(Interval.this, config.toJsonString());
            return this;
        }

        public Interval tag(final String jsonConfig) {
            mF2Chart.assertRenderThread();
            mF2Chart.getChartProxy().setGeomIntervalTag(Interval.this, jsonConfig);
            return this;
        }
    }
}
