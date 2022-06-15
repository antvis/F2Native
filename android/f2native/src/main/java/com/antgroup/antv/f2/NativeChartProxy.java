package com.antgroup.antv.f2;

/**
 * @author qingyuan.yl
 * @date 2020-09-23
 */
public final class NativeChartProxy {
    private volatile long mNativeChartHandler = 0;

    NativeChartProxy(String name, double width, double height, double ratio) {
        try {
            mNativeChartHandler = nCreateNativeChart(name, width, height, ratio);
        } catch (Error error) {
            mNativeChartHandler = 0;
        }
    }

    int setCanvas(long nativeCanvas, String functionId) {
        if (mNativeChartHandler == 0 || nativeCanvas == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        try {
            return nSetCanvasView(mNativeChartHandler, nativeCanvas, functionId);
        } catch (Error error) {
            return 0;
        }
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
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetCoord(mNativeChartHandler, config);
    }

    int setAnimate(String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetAnimate(mNativeChartHandler, config);
    }

    int setInteraction(String type, String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetInteraction(mNativeChartHandler, type, config);
    }

    int setToolTip(String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetToolTip(mNativeChartHandler, config);
    }

    int setLegend(String field, String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetLegend(mNativeChartHandler, field, config);
    }

    int setGuideType(String type, String config) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSetGuideType(mNativeChartHandler, type, config);
    }

    int sendTouchEvent(String event) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nSendTouchEvent(mNativeChartHandler, event);
    }

    boolean render() {
        if (mNativeChartHandler == 0) {
            return false;
        }
        return nRender(mNativeChartHandler);
    }

    double[] getPosition(String itemData) {
        if (mNativeChartHandler == 0) {
            return new double[]{0, 0};
        }
        return nGetPosition(mNativeChartHandler, itemData);
    }

    int clear() {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nClear(mNativeChartHandler);
    }

    F2Geom createGeom(F2Chart chart, String type) {
        long handle;
        if (mNativeChartHandler == 0) {
            handle = 0;
        } else {
            handle = nCreateGeom(mNativeChartHandler, type);
        }
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
            case "point": {
                return new F2Geom.Point(chart, handle, type);
            }
            case "candle": {
                return new F2Geom.Candle(chart, handle, type);
            }
        }
        return null;
    }

    void setGeomPosition(F2Geom geom, String field) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomPosition(geom.getNativeGeomHandler(), geom.getType(), field);
    }

    void setGeomColor(F2Geom geom, String field, String[] colors) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomColor(geom.getNativeGeomHandler(), geom.getType(), field, colors);
    }

    void setGeomColor(F2Geom geom, String color) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomColor(geom.getNativeGeomHandler(), geom.getType(), color);
    }

    void setGeomSize(F2Geom geom, String field, float[] sizes) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomSize(geom.getNativeGeomHandler(), geom.getType(), field, sizes);
    }

    void setGeomSize(F2Geom geom, float size) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomSize(geom.getNativeGeomHandler(), geom.getType(), size);
    }

    void setGeomShape(F2Geom geom, String field, String[] shapes) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomShape(geom.getNativeGeomHandler(), geom.getType(), field, shapes);
    }

    void setGeomShape(F2Geom geom, String shape) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomShape(geom.getNativeGeomHandler(), geom.getType(), shape);
    }

    void setGeomAdjust(F2Geom geom, String type) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomAdjust(geom.getNativeGeomHandler(), geom.getType(), type);
    }

    void setGeomStyle(F2Geom geom, String config) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomStyle(geom.getNativeGeomHandler(), config);
    }

    void setGeomIntervalTag(F2Geom geom, String config) {
        if (!"interval".equals(geom.getType()) || mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomIntervalTag(geom.getNativeGeomHandler(), geom.getType(), config);
    }

    void setGeomAttrs(F2Geom geom, String config) {
        if (mNativeChartHandler == 0 || geom.getNativeGeomHandler() == 0) {
            return;
        }
        nGeomAttrs(geom.getNativeGeomHandler(), config);
    }

    int getRenderDuration() {
        if (mNativeChartHandler == 0) {
            return 0;
        }
        return nGetRenderDurationMM(mNativeChartHandler);
    }

    int getRenderCmdCount() {
        if (mNativeChartHandler == 0) {
            return 0;
        }
        return nGetRenderCmdCount(mNativeChartHandler);
    }

    String getChartId() {
        if (mNativeChartHandler == 0) {
            return null;
        }
        return nGetChartId(mNativeChartHandler);
    }

    String getScaleTicks(String field) {
        if (mNativeChartHandler == 0) {
            return null;
        }
        return nGetScaleTicks(mNativeChartHandler, field);
    }

    static void executeCommand(long command) {
        if (command == 0) {
            return;
        }
        nExecuteCommand(command);
    }

    static void deallocCommand(long command) {
        if (command == 0) {
            return;
        }
        nDeallocCommand(command);
    }

    int destroy() {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_SUCCESS;
        }
        int code = nDestroy(mNativeChartHandler);
        mNativeChartHandler = 0;
        return code;
    }

    long createFunction(long nativeChartHandler, Object thisObj) {
        if (mNativeChartHandler == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nCreateFunctionV(nativeChartHandler, thisObj);
    }

    int destroyFuntion(long nativeFunctionHandle) {
        if (mNativeChartHandler == 0 || nativeFunctionHandle == 0) {
            return F2Constants.CODE_FAIL_UNKNOWN;
        }
        return nDestroyFunction(mNativeChartHandler, nativeFunctionHandle);
    }

    long getNativeChartHandler() {
        return mNativeChartHandler;
    }

    private native static long nCreateNativeChart(String name, double width, double height, double ratio);

    private native static int nSetCanvasView(long nativeChartHandler, long nativeViewHandle, String functionId);

    private native static int nSetSource(long nativeChartHandler, String json);

    private native static int nSetPadding(long nativeChartHandler, double left, double top, double right, double bottom);

    private native static int nSetMargin(long nativeChartHandler, double left, double top, double right, double bottom);

    private native static int nSetScale(long nativeChartHandler, String field, String json);

    private native static int nSetAxis(long nativeChartHandler, String field, String json);

    private native static int nSetCoord(long nativeChartHandler, String json);

    private native static int nSetAnimate(long nativeChartHandler, String json);

    private native static int nSetInteraction(long nativeChartHandler, String type, String config);

    private native static int nSetToolTip(long nativeChartHandler, String config);

    private native static int nSetLegend(long nativeChartHandler, String field, String config);

    private native static int nSetGuideType(long nativeChartHandler, String type, String config);

    private native static int nSendTouchEvent(long nativeChartHandler, String event);

    private native static long nCreateGeom(long nativeChartHandler, String geomType);

    private native static String nGetChartId(long nativeChartHandler);

    private native static int nGetRenderCmdCount(long nativeChartHandler);

    private native static int nGetRenderDurationMM(long nativeChartHandler);

    private native static String nGetScaleTicks(long nativeChartHandler, String field);

    private native static int nDestroy(long nativeChartHandler);

    private native static boolean nRender(long nativeChartHandler);

    private native static double[] nGetPosition(long nativeChartHandler, String itemData);

    private native static int nClear(long nativeChartHandler);

    private native static int nGeomPosition(long geomHandle, String type, String field);

    private native static int nGeomColor(long geomHandle, String type, String field, String[] colors);

    private native static int nGeomColor(long geomHandle, String type, String color);

    private native static int nGeomSize(long geomHandle, String type, String field, float[] sizes);

    private native static int nGeomSize(long geomHandle, String type, float size);

    private native static int nGeomShape(long geomHandle, String type, String field, String[] shapes);

    private native static int nGeomShape(long geomHandle, String type, String shape);

    private native static int nGeomAdjust(long geomHandle, String type, String adjustType);

    private native static int nGeomIntervalTag(long geomHandle, String type, String config);

    private native static int nGeomStyle(long geomHandle, String config);

    private native static int nGeomAttrs(long geomHandle, String config);

    private native static int nExecuteCommand(long commandHandle);

    private native static int nDeallocCommand(long commandHandle);

    private native static long nCreateFunctionV(long nativeChartHandler, Object thisObj);

    private native static int nDestroyFunction(long nativeChartHandler, long nativeFunctionHandle);

    private native static int nConfig(long nativeChartHandler, String config);

}
