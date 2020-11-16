package com.antgroup.antv.f2;

import android.view.Surface;

/**
 * @author qingyuan.yl
 * @date 2020-09-17
 */
final class NativeCanvasProxy {

    long mNativeViewHandle = 0;

    private static boolean libLoaded = false;
    static void load() {
        if (!libLoaded) {
            System.loadLibrary("f2");
        }
        libLoaded = true;
    }

    NativeCanvasProxy(F2Config options) {
        mNativeViewHandle = nCreateNativeChartView(options.toJsonString());
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
}
