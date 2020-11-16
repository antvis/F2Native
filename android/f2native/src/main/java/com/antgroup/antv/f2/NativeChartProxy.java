package com.antgroup.antv.f2;

/**
 * @author qingyuan.yl
 * @date 2020-09-23
 */
final class NativeChartProxy {
    private volatile long mNativeChartHandler = 0;

    NativeChartProxy(String name, double width, double height, double ratio) {
        mNativeChartHandler = nCreateNativeChart(name, width, height, ratio);
    }

    int setCanvas(NativeCanvasProxy canvasProxy) {
        if (mNativeChartHandler == 0 || canvasProxy.mNativeViewHandle == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetCanvasView(mNativeChartHandler, canvasProxy.mNativeViewHandle);
    }

    int source(String json) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetSource(mNativeChartHandler, json);
    }

    int setPadding(double left, double top, double right, double bottom) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetPadding(mNativeChartHandler, left, top, right, bottom);
    }

    int setMargin(double left, double top, double right, double bottom) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetMargin(mNativeChartHandler, left, top, right, bottom);
    }

    int setScale(String field, String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetScale(mNativeChartHandler, field, config);
    }

    int setAxis(String field, String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetAxis(mNativeChartHandler, field, config);
    }

    int setCoord(String config) {
        if (mNativeChartHandler == 0) return F2Constants.CODE_FAIL_UNKNOWN;
        return nSetCoord(mNativeChartHandler, config);
    }

    int setInteraction(String type) {
        if (mNativeChartHandler == 0) return F2Constants.CODE_FAIL_UNKNOWN;
        return nSetInteraction(mNativeChartHandler, type);
    }

    int setToolTip(String config) {
        if (mNativeChartHandler == 0) return F2Constants.CODE_FAIL_UNKNOWN;
        return nSetToolTip(mNativeChartHandler, config);
    }

    int setLegend(String field, String config) {
        if (mNativeChartHandler == 0) return F2Constants.CODE_FAIL_UNKNOWN;
        return nSetLegend(mNativeChartHandler, field, config);
    }

    int sendTouchEvent(String event) {
        if (mNativeChartHandler == 0) return F2Constants.CODE_FAIL_UNKNOWN;
        return nSendTouchEvent(mNativeChartHandler, event);
    }

    int render() {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nRender(mNativeChartHandler);
    }

    F2Geom createGeom(F2Chart chart, String type) {
        if (mNativeChartHandler == 0) return null;
        long handle = nCreateGeom(mNativeChartHandler, type);
        switch (type) {
            case "line": {
                return new F2Geom.Line(chart, handle, type);
            }
            case "area": {
                return new F2Geom.Area(chart, handle, type);
            }
            case "interval": {
                return new F2Geom.Interval(chart, handle, type);
            }
        }
        return null;
    }

    F2Guide getGuideController() {
        if (mNativeChartHandler == 0) return null;
        return new F2Guide(nGetGuideController(mNativeChartHandler));
    }

    void setGeomPosition(F2Geom geom, String field) {
        if (mNativeChartHandler == 0) return;
        nGeomPosition(geom.getNativeGeomHandler(), geom.getType(), field);
    }

    void setGeomColor(F2Geom geom, String field, String[] colors) {
        if (mNativeChartHandler == 0) return;
        nGeomColor(geom.getNativeGeomHandler(), geom.getType(), field, colors);
    }

    void setGeomColor(F2Geom geom, String color) {
        if (mNativeChartHandler == 0) return;
        nGeomColor(geom.getNativeGeomHandler(), geom.getType(), color);
    }

    void setGeomSize(F2Geom geom, String field, float[] sizes) {
        if (mNativeChartHandler == 0) return;
        nGeomSize(geom.getNativeGeomHandler(), geom.getType(), field, sizes);
    }

    void setGeomSize(F2Geom geom, float size) {
        if (mNativeChartHandler == 0) return;
        nGeomSize(geom.getNativeGeomHandler(), geom.getType(), size);
    }

    void setGeomShape(F2Geom geom, String field, String[] shapes) {
        if (mNativeChartHandler == 0) return;
        nGeomShape(geom.getNativeGeomHandler(), geom.getType(), field, shapes);
    }

    void setGeomShape(F2Geom geom, String shape) {
        if (mNativeChartHandler == 0) return;
        nGeomShape(geom.getNativeGeomHandler(), geom.getType(), shape);
    }

    void setGeomAdjust(F2Geom geom, String type) {
        if (mNativeChartHandler == 0) return;
        nGeomAdjust(geom.getNativeGeomHandler(), geom.getType(), type);
    }

    void setGeomIntervalTag(F2Geom geom, String config) {
        if (!"interval".equals(geom.getType()) && mNativeChartHandler == 0) return;
        nGeomIntervalTag(geom.getNativeGeomHandler(), geom.getType(), config);
    }

    void setGeomIntervalStyle(F2Geom geom, String config) {
        if (!"interval".equals(geom.getType()) && mNativeChartHandler == 0) return;
        nGeomIntervalStyle(geom.getNativeGeomHandler(), geom.getType(), config);
    }

    String getRenderDumpInfo() {
        if (mNativeChartHandler == 0) return null;
        return nGetRenderDumpInfo(mNativeChartHandler);
    }

    int destroy() {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_SUCCESS;
        }
        int code = nDestroy(mNativeChartHandler);
        mNativeChartHandler = 0;
        return code;
    }

    private native static long nCreateNativeChart(String name, double width, double height, double ratio);

    private native static int nSetCanvasView(long nativeChartHandler, long nativeViewHandle);

    private native static int nSetSource(long nativeChartHandler, String json);

    private native static int nSetPadding(long nativeChartHandler, double left, double top, double right, double bottom);

    private native static int nSetMargin(long nativeChartHandler, double left, double top, double right, double bottom);

    private native static int nSetScale(long nativeChartHandler, String field, String json);

    private native static int nSetAxis(long nativeChartHandler, String field, String json);

    private native static int nSetCoord(long nativeChartHandler, String json);
    private native static int nSetInteraction(long nativeChartHandler, String type);
    private native static int nSetToolTip(long nativeChartHandler, String config);

    private native static int nSetLegend(long nativeChartHandler, String field, String config);

    private native static int nSendTouchEvent(long nativeChartHandler, String event);

    private native static long nGetGuideController(long nativeChartHandler);

    private native static long nCreateGeom(long nativeChartHandler, String geomType);

    private native static String nGetRenderDumpInfo(long nativeChartHandler);

    private native static int nDestroy(long nativeChartHandler);

    private native static int nRender(long nativeChartHandler);

    private native static int nGeomPosition(long geomHandle, String type, String field);

    private native static int nGeomColor(long geomHandle, String type, String field, String[] colors);

    private native static int nGeomColor(long geomHandle, String type, String color);

    private native static int nGeomSize(long geomHandle, String type, String field, float[] sizes);

    private native static int nGeomSize(long geomHandle, String type, float size);

    private native static int nGeomShape(long geomHandle, String type, String field, String[] shapes);

    private native static int nGeomShape(long geomHandle, String type, String shape);
    private native static int nGeomAdjust(long geomHandle, String type, String adjustType);

    private native static int nGeomIntervalTag(long geomHandle, String type, String config);
    private native static int nGeomIntervalStyle(long geomHandle, String type, String config);
}
