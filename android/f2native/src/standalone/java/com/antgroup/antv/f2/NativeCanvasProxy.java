package com.antgroup.antv.f2;

import android.view.Surface;

/**
 * @author qingyuan.yl
 * @date 2020-09-17
 */
final class NativeCanvasProxy {

    long mNativeViewHandle = 0;

    private static volatile boolean sLibLoaded = false;
    static void load() {
        if (!sLibLoaded) {
            System.loadLibrary("f2");
        }
        sLibLoaded = true;
    }

    NativeCanvasProxy(F2Config options) {
        try {
            mNativeViewHandle = nCreateNativeChartView(options.toJsonString());
        } catch (Error error) {
            mNativeViewHandle = 0;
        }
    }

    int onSurfaceTextureAvailable(Surface surface) {
        if (mNativeViewHandle == 0) return 0;

        return nSurfaceAvailable(mNativeViewHandle, surface);
    }

    int onSurfaceTextureSizeChanged(int width, int height) {
        if (mNativeViewHandle == 0) return 0;
        return nSurfaceSizeChanged(mNativeViewHandle, width, height);
    }

    int onSurfaceTextureDestroyed() {
        if (mNativeViewHandle == 0) return 0;
        return nSurfaceDestroyed(mNativeViewHandle);
    }

    boolean isCanvasContextInited() {
        if (mNativeViewHandle == 0) return false;
        return nIsCanvasContextInitted(mNativeViewHandle);
    }

    public int initCanvasContext() {
        if (mNativeViewHandle == 0) return 0;
        return nInitCanvasContext(mNativeViewHandle);
    }

    int destroy() {
        try {
            if (mNativeViewHandle == 0) return 0;

            int retCode = nDestroyNativeChartView(mNativeViewHandle);
            mNativeViewHandle = 0;
            return retCode;
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }

    int performSwapFrame() {
        if (mNativeViewHandle == 0) return 0;
        return nPerformSwapFrame(mNativeViewHandle);
    }

    private static native long nCreateNativeChartView(String options);

    private static native int nSurfaceAvailable(long nativeViewHandle, Surface surface);

    private static native int nInitCanvasContext(long nativeViewHandle);

    private static native boolean nIsCanvasContextInitted(long nativeViewHandle);

    private static native int nSurfaceSizeChanged(long nativeViewHandle, int width, int height);

    private static native int nSurfaceDestroyed(long nativeViewHandle);

    private static native int nDestroyNativeChartView(long nativeViewHandle);

    private static native int nPerformSwapFrame(long nativeViewHandle);


    //////////////////////////// graphic 2d apis ////////////////////////////////////////////////////

    static native void nSetFillStyle(long nativeViewHandle, String style);

    static native void nSetStrokeStyle(long nativeViewHandle, String style);

    static native void nRect(long nativeViewHandle, int x, int y, int width, int height);

    static native void nFillRect(long nativeViewHandle, int x, int y, int width, int height);

    static native void nStrokeRect(long nativeViewHandle, int x, int y, int width, int height);

    static native void nClearRect(long nativeViewHandle, int x, int y, int width, int height);

    static native void nFill(long nativeViewHandle);
    
    static native void nStroke(long nativeViewHandle);

    static native void nBeginPath(long nativeViewHandle);

    static native void nMoveTo(long nativeViewHandle, float x, float y);

    static native void nClosePath(long nativeViewHandle);

    static native void nLineTo(long nativeViewHandle, float x, float y);

    static native void nClip(long nativeViewHandle);

    static native void nQuadraticCurveTo(long nativeViewHandle, float cpx, float cpy, float x, float y);

    static native void nBezierCurveTo(long nativeViewHandle, float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

    static native void nArc(long nativeViewHandle, float x, float y, float r, float sAngle, float eAngle, boolean antiClockwise);

    static native void nArcTo(long nativeViewHandle, float x1, float y1, float x2, float y2, float r);

    static native void nScale(long nativeViewHandle, float scaleWidth, float scaleHeight);

    static native void nRotate(long nativeViewHandle, float angle);

    static native void nTranslate(long nativeViewHandle, float x, float y);

    static native void nTransform(long nativeViewHandle, float a, float b, float c, float d, float e, float f);
    static native void nSetTransform(long nativeViewHandle, float a, float b, float c, float d, float e, float f);

    static native void nSetLineCap(long nativeViewHandle, String lineCap);
    static native void nSetLineJoin(long nativeViewHandle, String lineJoin);

    static native void nSetLineWidth(long nativeViewHandle, float lineWidth);
    static native void nSetLineDashOffset(long nativeViewHandle, float offset);
    static native void nSetLineDash(long nativeViewHandle, float[] params);
    static native float[] nGetLineDash(long nativeViewHandle);


    static native void nSetMiterLimit(long nativeViewHandle, float limit);

    static native void nSetFont(long nativeViewHandle, String font);
    static native void nSetTextAlign(long nativeViewHandle, String textAlign);
    static native String nGetTextAlign(long nativeViewHandle);
    static native void nSetTextBaseline(long nativeViewHandle, String textBaseline);
    static native String nGetTextBaseline(long nativeViewHandle);

    static native void nFillText(long nativeViewHandle, String text, float x, float y);
    static native void nFillText(long nativeViewHandle, String text, float x, float y, float maxWidth);

    static native void nStrokeText(long nativeViewHandle, String text, float x, float y);
    static native void nStrokeText(long nativeViewHandle, String text, float x, float y, float maxWidth);
    static native float nMeasureText(long nativeViewHandle, String text);

    static native void nSetGlobalAlpha(long nativeViewHandle, float globalAlpha);
    static native float nGetGlobalAlpha(long nativeViewHandle);
    static native void nSetGlobalCompositeOperation(long nativeViewHandle, String op);

    static native void nSave(long nativeViewHandle);
    static native void nRestore(long nativeViewHandle);

    static native void nSetShadowColor(long nativeViewHandle, String color);
    static native void nSetShadowBlur(long nativeViewHandle, int v);
    static native void nSetShadowOffsetX(long nativeViewHandle, float v);
    static native void nSetShadowOffsetY(long nativeViewHandle, float v);

}
