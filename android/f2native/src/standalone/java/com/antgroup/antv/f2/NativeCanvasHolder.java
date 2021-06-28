package com.antgroup.antv.f2;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.view.Surface;

import com.taobao.gcanvas.GCanvasJNI;

/**
 * @author qingyuan.yl
 * @date 2020-09-17
 */
final class NativeCanvasHolder {

    interface ICanvasListener {

        void onSurfaceReady();

        void onCanvasFrameUpdate();

        void onCanvasDestroyed();
    }

    private final Object mSurfaceLock = new Object();
    private volatile Surface mSurface;
    private volatile boolean surfaceReady = false;

    private NativeCanvasProxy mNativeCanvasProxy;

    private F2Config mCanvasOptions;

    private F2RenderThread mRenderThread;

    private Handler mUIHandler;

    private int surfaceWidth = 0;
    private int surfaceHeight = 0;

    private boolean hasDestroy;

    private ICanvasListener mListener;

    private String mCanvasId;

    NativeCanvasHolder(Context context, F2Config options, F2RenderThreadFactory threadFactory) {
        mCanvasOptions = options;

        mCanvasId = options.getStringField(F2Constants.CANVAS_ID);

        onPreCreate(context, threadFactory);

        mNativeCanvasProxy = new NativeCanvasProxy(options);
    }

    public void setListener(ICanvasListener listener) {
        mListener = listener;

        if (surfaceReady && listener != null) {
            listener.onSurfaceReady();
        }
    }

    private void onPreCreate(Context context, F2RenderThreadFactory threadFactory) {
        GCanvasJNI.load();
        NativeCanvasProxy.load();

        if (threadFactory == null) {
            threadFactory = new F2StandaloneRenderThread.Factory();
        }

        mRenderThread = threadFactory.createRenderThread(mCanvasOptions);
        mRenderThread.start();
        mUIHandler = new Handler(Looper.getMainLooper());
        hasDestroy = false;
    }

    public void initCanvasContext() {
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                if (mNativeCanvasProxy == null) return;
                boolean isInited = mNativeCanvasProxy.isCanvasContextInited();
                innerLog("#initCanvasContext isInited: " + isInited);
                if (!isInited) {
                    int code = mNativeCanvasProxy.initCanvasContext();
                    if (F2Constants.isSuccessCode(code)) {
                        dispatchCanvasFrameUpdate();
                    }
                }
            }
        });
    }

    public void onSurfaceAvailable(final Surface surface, final int width, final int height) {
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                innerLog("#onSurfaceAvailable: " + surface + ", width: " + width + ", height: " + height);
                synchronized (mSurfaceLock) {
                    mSurface = surface;
                    int code = mNativeCanvasProxy.onSurfaceTextureAvailable(mSurface);
                    //saveSurfaceSize
                    saveSurfaceSize(width, height);
                    surfaceReady = true;

                    dispatchSurfaceReady();
                    if (F2Constants.isSuccessCode(code)) {
                        dispatchCanvasFrameUpdate();
                    }
                }
            }
        });
    }

    public void onSurfaceSizeChanged(Surface surface, final int width, final int height) {
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                synchronized (mSurfaceLock) {
                    if (Math.abs(surfaceWidth - width) >= 1 || Math.abs(surfaceHeight - height) >= 1) {
                        innerLog(String.format("#onSurfaceSizeChanged: (%d, %d) to (%d, %d)", surfaceWidth, surfaceHeight, width, height));
                        int code = mNativeCanvasProxy.onSurfaceTextureSizeChanged(width, height);
                        if (F2Constants.isSuccessCode(code)) {
                            dispatchCanvasFrameUpdate();
                        }
                    }
                    saveSurfaceSize(width, height);
                }
            }
        });
    }

    public void onSurfaceDestroyed(Surface surface) {
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                synchronized (mSurfaceLock) {
                    handleSurfaceDestroy();
                }
            }
        });
    }

    public void swapBuffer() {
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                if (mNativeCanvasProxy != null) {
                    int code = mNativeCanvasProxy.performSwapFrame();
                    if (F2Constants.isSuccessCode(code)) {
                        dispatchCanvasFrameUpdate();
                    }
                }
            }
        });
    }

    void destroy(final Runnable callback) {
        if (hasDestroy) {
            innerLog("#destroy");
            if (callback != null)
                callback.run();
            return;
        }
        hasDestroy = true;
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                innerLog("#destroyInner");
                handleSurfaceDestroy();
                if (mNativeCanvasProxy != null) {
                    mNativeCanvasProxy.destroy();
                }

                mRenderThread.askQuit();

                dispatchCanvasDestroyed();

                mListener = null;

                if (callback != null)
                    callback.run();
            }
        });
    }


    private void handleSurfaceDestroy() {
        if (!surfaceReady) {
            return;
        }

        mNativeCanvasProxy.onSurfaceTextureDestroyed();
        if (mSurface != null) {
            mSurface = null;
        }

        surfaceReady = false;
    }

    public boolean isValid() {
        return mSurface != null;
    }

    public boolean isSurfaceReady() {
        return surfaceReady;
    }

    public final F2Config getCanvasOptions() {
        return mCanvasOptions;
    }

    final NativeCanvasProxy getNativeCanvasProxy() {
        return mNativeCanvasProxy;
    }

    F2RenderThread getRenderThread() {
        return mRenderThread;
    }

    private void saveSurfaceSize(int width, int height) {
        surfaceWidth = width;
        surfaceHeight = height;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            innerLog("finalize");
            destroy(null);
        } catch (Exception ex) {

        } finally {
            try {
                super.finalize();
            } catch (Exception e) {
            }
        }
    }

    private void dispatchSurfaceReady() {
        if (mListener == null) return;

        mListener.onSurfaceReady();
    }

    private void dispatchCanvasFrameUpdate() {
        if (mListener == null) return;
        runOnUIThread(new Runnable() {
            @Override
            public void run() {
                mListener.onCanvasFrameUpdate();
            }
        });
    }

    private void dispatchCanvasDestroyed() {
        if (mListener == null) return;
        mListener.onCanvasDestroyed();
    }

    final void runOnRenderThread(Runnable runnable) {
        mRenderThread.immediatePost(runnable);
    }

    final boolean isOnRenderThread() {
        return mRenderThread.isOnRenderThread();
    }

    private final void runOnUIThread(Runnable runnable) {
        if (Looper.getMainLooper() == Looper.myLooper()) {
            runnable.run();
        } else {
            mUIHandler.post(runnable);
        }
    }

    void innerLog(String content) {
        if (mCanvasOptions == null) {
            F2Log.i("NativeCanvasHolder|unknown", content);
        } else {
            F2Log.i("NativeCanvasHolder|" + mCanvasId, content);
        }
    }
}
