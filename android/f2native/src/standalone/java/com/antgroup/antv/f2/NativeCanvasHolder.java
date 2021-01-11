package com.antgroup.antv.f2;

import android.content.Context;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.view.Surface;

import com.taobao.gcanvas.GCanvasJNI;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

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

    private HandlerThread mRenderThread;

    private Handler mRenderHandler;
    private Handler mUIHandler;

    private int surfaceWidth = 0;
    private int surfaceHeight = 0;

    private boolean hasDestroy;

    private ICanvasListener mListener;

    private String mCanvasId;

    private boolean mAsyncRender = true;
    private List<WeakReference<Runnable>> mRenderTasks = new ArrayList<>();


    NativeCanvasHolder(Context context, F2Config options) {
        mCanvasOptions = options;

        mCanvasId = options.getStringField(F2Constants.CANVAS_ID);

        onPreCreate(context);

        mNativeCanvasProxy = new NativeCanvasProxy(options);
    }

    public void setListener(ICanvasListener listener) {
        mListener = listener;

        if (surfaceReady && listener != null) {
            listener.onSurfaceReady();
        }
    }

    private void onPreCreate(Context context) {
        GCanvasJNI.load();
        NativeCanvasProxy.load();

        mAsyncRender = mCanvasOptions.getBoolField(F2CanvasView.ConfigBuilder.KEY_ASYNC_RENDER, true);

        if (mAsyncRender) {
            mRenderThread = new HandlerThread("t-" + mCanvasId);
            mRenderThread.start();
            mRenderHandler = new Handler(mRenderThread.getLooper());
        } else {
            // 同步操作为当前线程(不一定是主线程)
            mRenderHandler = new Handler(Looper.myLooper());
        }
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
            if (callback != null)
                callback.run();
            return;
        }
        hasDestroy = true;
        removeAllIdleTasks();
        runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                innerLog("#destroy");
                handleSurfaceDestroy();
                if (mNativeCanvasProxy != null) {
                    mNativeCanvasProxy.destroy();
                }

                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ECLAIR && mRenderThread != null) {
                    mRenderThread.quit();
                }

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
        if (mRenderHandler == null) return;
        if (mRenderHandler.getLooper() == Looper.myLooper()) {
            runnable.run();
        } else {
            mRenderHandler.post(runnable);
            mRenderTasks.add(new WeakReference<Runnable>(runnable));
        }
    }

    final boolean isOnRenderThread() {
        if (mRenderHandler == null) return false;
        return (mRenderHandler.getLooper() == Looper.myLooper());
    }

    private final void runOnUIThread(Runnable runnable) {
        if (Looper.getMainLooper() == Looper.myLooper()) {
            runnable.run();
        } else {
            mUIHandler.post(runnable);
        }
    }

    private void removeAllIdleTasks() {
        if (mRenderHandler != null && !mRenderTasks.isEmpty()) {
            for (WeakReference<Runnable> renderTask : mRenderTasks) {
                Runnable task = null;
                if ((task = renderTask.get()) != null) {
                    mRenderHandler.removeCallbacks(task);
                }
            }
            mRenderTasks.clear();
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
