package com.antgroup.antv.f2;

import android.content.Context;
import android.graphics.Color;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.alipay.antgraphic.CanvasSimpleLifeCycleListener;
import com.alipay.antgraphic.isolate.CanvasIsolate;
import com.alipay.antgraphic.isolate.CanvasIsolateConfig;
import com.alipay.antgraphic.misc.AGConstant;
import com.alipay.antgraphic.misc.CanvasBizModel;
import com.alipay.antgraphic.misc.CanvasCommonResult;
import com.alipay.antgraphic.misc.CanvasOptions;
import com.alipay.antgraphic.thread.BaseCanvasThreadWrap;
import com.alipay.antgraphic.view.CanvasViewContainer;
import com.antgroup.antv.f2.base.F2BaseCanvasHandle;
import com.antgroup.antv.f2.base.F2BaseCanvasView;

import java.lang.ref.WeakReference;
import java.util.ArrayList;

/**
 * antg需要的CanvasView
 * @author qingyuan.yl
 * @date 2020-09-17
 */
class F2AntGCanvasView extends CanvasViewContainer implements F2BaseCanvasView {

    static {
        System.loadLibrary("antgraphic");
        System.loadLibrary("f2");
    }

    private boolean hasCallInitCanvas = false;
    private CanvasIsolate mCanvasIsolate;
    private volatile F2RenderThread mCanvasRenderThread;

    private volatile F2CanvasView.Adapter mAdapter;
    private F2CanvasView.OnCanvasTouchListener mOnCanvasTouchListener;
    private CanvasListener mCanvasListener;

    private F2Config mOptions;
    private volatile int mDrawIndex = 0;

    private volatile boolean isTouchProcessing = false;

    private F2CanvasHandle mCanvasHandle = null;

    private volatile boolean bSurfaceReady = false;

    private volatile F2CanvasView mF2CanvasView;

    private ArrayList<Integer> mRenderCmdCountList = new ArrayList<>();

    public F2AntGCanvasView(Context context) {
        this(context, null);
    }

    public F2AntGCanvasView(Context context, AttributeSet attrs) {
        this(context, attrs, null);
    }

    public F2AntGCanvasView(Context context, AttributeSet attrs, F2CanvasView f2CanvasView) {
        super(context, attrs);
        this.mF2CanvasView = f2CanvasView;
        initView();
    }

    private void initView() {
        AntGLauncher.init();
        mCanvasListener = new CanvasListener(this);
    }

    @Override
    public View getView() {
        return this;
    }

    @Override
    public void setAdapter(F2CanvasView.Adapter adapter) {
        if (mAdapter != null && mAdapter != adapter) {
            mAdapter.onDestroy();
        }
        mAdapter = adapter;
        if (mAdapter == null) {
            return;
        }

        postCanvasDraw();
    }

    @Override
    public void setOnCanvasTouchListener(F2CanvasView.OnCanvasTouchListener onCanvasTouchListener) {
        mOnCanvasTouchListener = onCanvasTouchListener;
    }

    @Override
    public void initCanvasContext() {
        initCanvasContext(null);
    }

    @Override
    public void initCanvasContext(F2Config config) {
        initCanvasContext(config, null);
    }

    @Override
    public void initCanvasContext(F2Config config, F2RenderThreadFactory threadFactory) {
        if (hasCallInitCanvas) return;
        f2CanvasInnerLog("#initCanvasContext  callInitCanvas");
        callInitCanvas(getContext(), config, threadFactory);
    }

    private void callInitCanvas(Context context, F2Config config, F2RenderThreadFactory threadFactory) {
        if (hasCallInitCanvas) return;
        if (config == null) {
            config = new F2CanvasView.ConfigBuilder().build();
        }

        CanvasOptions options = new CanvasOptions();
        options.asyncRender = true;
        options.canvasIdPrefix = config.getStringField(F2Constants.CANVAS_ID);
        String bgColor = config.getStringField(F2CanvasView.ConfigBuilder.KEY_BACKGROUND_COLOR);
        if (!TextUtils.isEmpty(bgColor)) {
            options.backgroundColor = Color.parseColor(bgColor);
        }

        String canvasType = config.getStringField(F2CanvasView.ConfigBuilder.KEY_CANVAS_TYPE);
        if (canvasType == null) {
            canvasType = AGConstant.BACKEND_UNKNOWN;
        }

        CanvasIsolateConfig isolateConfig = new CanvasIsolateConfig();
        isolateConfig.isolateId = "F2CanvasView_" + System.currentTimeMillis();
        isolateConfig.backend = canvasType;
        mCanvasIsolate = new CanvasIsolate(isolateConfig);

        options.canvasIsolate = mCanvasIsolate;

        options.canvasBiz = new CanvasBizModel();
        options.canvasBiz.canvasBizType = "f2native";
        options.canvasBiz.canvasBizId = config.getStringField(F2CanvasView.ConfigBuilder.KEY_CANVAS_BIZ_ID);

        // 是否使用原生 view 渲染
        options.useGLFunctor = config.getBoolField(F2CanvasView.ConfigBuilder.KEY_NATIVE_DRAW, false);

        initCanvas(options);

        try {
            config.mConfig.put("session_id", mCanvasIsolate.getId());
        } catch (Exception e) {
            innerExceptionLog( "#callInitCanvas exception " + e);
        }

        if (threadFactory == null) {
            threadFactory = new F2StandaloneRenderThread.Factory();
        }

        mCanvasRenderThread = threadFactory.createRenderThread(config);
        mCanvasRenderThread.start();

        getCanvas().setThreadWrap((BaseCanvasThreadWrap) mCanvasRenderThread);
        getCanvas().addLifeCycleListener(mCanvasListener);
        hasCallInitCanvas = true;

        mOptions = config;
        String appId = mOptions.getStringField(F2CanvasView.ConfigBuilder.KEY_APP_ID);
        F2Event.eventPage(appId, options.canvasBiz.canvasBizId, "init");
    }

    @Override
    public final boolean isOnCanvasThread() {
        if (mCanvasRenderThread == null) return false;
        return mCanvasRenderThread.isOnRenderThread();
    }

    @Override
    public final long getNativeCanvas() {
        if (getCanvasView() == null || getCanvas() == null) return 0;
        return getCanvas().getNativeHandle();
    }

    @Override
    public final F2RenderThread getRenderThread() {
        return mCanvasRenderThread;
    }

    @Override
    public F2CanvasHandle getCanvasHandle() {
        return mCanvasHandle;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        final MotionEvent copyEvent = MotionEvent.obtain(event);
        if (mOnCanvasTouchListener != null) {
            if (event.getActionMasked() == MotionEvent.ACTION_DOWN && isTouchProcessing) {
                return true;
            }
            runOnRenderThread(new Runnable() {
                @Override
                public void run() {
                    isTouchProcessing = true;
                    if (mOnCanvasTouchListener != null) {
                        mOnCanvasTouchListener.onTouch(mF2CanvasView, new F2CanvasView.TouchEvent(copyEvent));
                    }
                    isTouchProcessing = false;
                }
            });

            if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                return true;
            }
        }
        return super.onTouchEvent(event);
    }

    @Override
    public void postCanvasDraw() {
        if (!hasCallInitCanvas || mCanvasRenderThread == null
                || mAdapter == null || getCanvasView() == null || getCanvas() == null
                || !isCanvasReady()) {
            return;
        }
        runOnRenderThread(new RenderTask(this));
    }

    final void onSurfaceReady() {
        f2CanvasInnerLog("#onSurfaceReady call initCanvasContext " + hasCallInitCanvas);
        if (!hasCallInitCanvas) return;

        mCanvasHandle = new F2CanvasHandle(this);
        postCanvasDraw();
    }

    @Override
    public boolean hasAdapter() {
        return mAdapter != null;
    }

    @Override
    public void destroy() {
        f2CanvasInnerLog("#destroy");
        bSurfaceReady = false;
        destroyCanvas();
        if (mCanvasIsolate != null) {
            mCanvasIsolate.destroy();
        }

        if (mCanvasRenderThread != null) {
            mCanvasRenderThread.askQuit();
            mCanvasRenderThread = null;
        }

        mOnCanvasTouchListener = null;
    }

    private boolean isCanvasReady() {
        return bSurfaceReady;
    }

    private void innerDestroy() {
        f2CanvasInnerLog("#innerDestroy adapter: " + mAdapter);
        bSurfaceReady = false;
        if (mAdapter != null) {
            mAdapter.onDestroy();
            mAdapter = null;
        }
    }

    @Override
    public void swapBuffer() {
        if (getCanvasView() != null && getCanvas() != null) {
            f2CanvasInnerLog("#swapBuffer");
            getCanvas().getCanvas2DContext().flushCommand();
            getCanvas().swapBuffer();
        }
    }

    private void clearRenderCmdCount() {
        mRenderCmdCountList.clear();
    }

    @Override
    public void appendRenderCmdCount(String chartName, int renderCmdCount) {
        mRenderCmdCountList.add(renderCmdCount);
    }

    public int getTotalRenderCmdCount() {
        int totalCount = 0;
        for (int i = 0; i < mRenderCmdCountList.size(); ++i) {
            int renderCmdCount = mRenderCmdCountList.get(i);
            if (renderCmdCount == 0) {
                F2Log.w("F2AntGCanvasView", "getTotalRenderCmdCount:ZERO at " + i);
                return 0;
            }
            totalCount += renderCmdCount;
        }
        return totalCount;
    }

    protected void f2CanvasInnerLog(String content) {
        if (getCanvasView() == null || getCanvas() == null) {
            F2Log.i("F2AntGCanvasView|unInited", content);
        } else {
            if (mOptions != null) {
                F2Log.i("F2AntGCanvasView|" + mOptions.getStringField(F2Constants.CANVAS_ID), content);
            } else {
                F2Log.i("F2AntGCanvasView|UnKnown", content);
            }
        }
    }

    private static void innerExceptionLog(String content) {
        F2Log.e("F2AntGCanvasView", content);
    }

    private static class CanvasListener extends CanvasSimpleLifeCycleListener {
        private WeakReference<F2AntGCanvasView> mCanvasView = null;
        private volatile boolean resetCanvasSize = false;

        CanvasListener(F2AntGCanvasView canvasView) {
            mCanvasView = new WeakReference<>(canvasView);
        }

        @Override
        public void onCanvasSurfaceAvailable(CanvasCommonResult result) {
            super.onCanvasSurfaceAvailable(result);
            final F2AntGCanvasView canvasView;

            if ((canvasView = mCanvasView.get()) != null) {
                try {
                    F2Log.i("F2AntGCanvasView", "#onCanvasSurfaceAvailable " + canvasView.getWidth() + ", " + canvasView.getHeight());
                    if (canvasView.mCanvasRenderThread == null || canvasView.getCanvasView() == null) {
                        // 还没走到回调，业务已经调了 destroy.
                        F2Log.i("F2AntGCanvasView", "#onCanvasSurfaceAvailable interupted. (thread: " + canvasView.mCanvasRenderThread + ", canvasView: " + canvasView.getCanvasView() + ")");
                        return;
                    }

                    canvasView.bSurfaceReady = true;
                    canvasView.mCanvasRenderThread.forcePost(new Runnable() {
                        @Override
                        public void run() {
                            if (!resetCanvasSize) {
                                if (canvasView.getCanvasView() != null && canvasView.getCanvas() != null) {
                                    canvasView.getCanvas().setDimension(canvasView.getWidth(), canvasView.getHeight());
                                }
                                resetCanvasSize = true;
                            }
                            canvasView.onSurfaceReady();
                        }
                    });
                } catch (Exception e) {
                    innerExceptionLog( "#onCanvasSurfaceAvailable exception " + e);
                }
            }
        }

        @Override
        public void onCanvasSurfaceDestroyed(CanvasCommonResult result) {
            super.onCanvasSurfaceDestroyed(result);
            F2Log.i("F2AntGCanvasView", "#onCanvasSurfaceDestroyed");
            resetCanvasSize = false;
            final F2AntGCanvasView canvasView;
            if ((canvasView = mCanvasView.get()) != null) {
                try {
                    canvasView.bSurfaceReady = false;
                    if (canvasView.mCanvasRenderThread != null) {
                        canvasView.mCanvasRenderThread.immediatePost(new Runnable() {
                            @Override
                            public void run() {
                                canvasView.innerDestroy();
                            }
                        });
                    } else {
                        canvasView.innerDestroy();
                    }
                } catch (Exception e) {
                    innerExceptionLog( "#onCanvasSurfaceDestroyed exception" + e);
                }
            }
        }
    }

    private void runOnRenderThread(Runnable runnable) {
        if (mCanvasRenderThread == null) return;
        mCanvasRenderThread.immediatePost(runnable);
    }

    @Override
    protected void finalize() {
        try {
            innerDestroy();
        } catch (Exception e) {
            innerExceptionLog( "#finalize exception " + e);
        } finally {
            try {
                super.finalize();
            } catch (Throwable throwable) {
                innerExceptionLog( "#finalize finally exception " + throwable);
            }
        }
    }

    private static final class RenderTask implements Runnable {
        private WeakReference<F2AntGCanvasView> wCanvasView;

        private RenderTask(F2AntGCanvasView canvasView) {
            this.wCanvasView = new WeakReference<>(canvasView);
        }

        @Override
        public void run() {
            try {
                F2AntGCanvasView canvasView = null;
                if ((canvasView = this.wCanvasView.get()) == null) {
                    F2Log.i("F2AntGCanvasView", "RenderTask:run:not matched");
                    return;
                }

                if (!canvasView.hasCallInitCanvas ||
                        canvasView.mCanvasRenderThread == null ||
                        canvasView.mAdapter == null ||
                        canvasView.getCanvasView() == null ||
                        canvasView.getCanvas() == null ||
                        !canvasView.isCanvasReady()) {
                    F2Log.i("F2AntGCanvasView", "RenderTask:run:not right status");
                    return;
                }

                canvasView.f2CanvasInnerLog("#postCanvasDraw");
                long duration = System.currentTimeMillis();
                canvasView.clearRenderCmdCount();
                canvasView.mAdapter.onCanvasDraw(canvasView.mF2CanvasView);

                String appId = canvasView.mOptions.getStringField(F2CanvasView.ConfigBuilder.KEY_APP_ID);
                String bizTypeID = canvasView.mOptions.getStringField(F2CanvasView.ConfigBuilder.KEY_CANVAS_BIZ_ID);
                duration = System.currentTimeMillis() - duration;
                if (BuildConfig.DEBUG) {
                    F2Log.i("F2AntGCanvasView", "#RenderTask onCanvasDraw cost = " + duration + "ms");
                }
                canvasView.mDrawIndex++;
                int cmdCount = canvasView.getTotalRenderCmdCount();
                F2Event.eventRender(appId, bizTypeID, canvasView.mDrawIndex, cmdCount, duration, canvasView.getWidth(), canvasView.getHeight(), canvasView.getResources().getDisplayMetrics().density);
            } catch (Exception e) {
                F2Log.w("F2AntGCanvasView", "#RenderTask:run:got " + e);
            }
        }
    }

    /**
     * antg需要的CanvasContext
     */
    public static final class F2CanvasHandle implements F2BaseCanvasHandle {
        private WeakReference<F2AntGCanvasView> mCanvasView;

        private F2CanvasHandle(F2AntGCanvasView canvasView) {
            mCanvasView = new WeakReference<>(canvasView);
        }

        private boolean checkCanvasView() {
            F2AntGCanvasView canvasView = mCanvasView.get();
            if (canvasView == null || !canvasView.hasCallInitCanvas || canvasView.getCanvasView() == null || canvasView.getCanvas() == null) {
                return false;
            }

            if (!canvasView.isOnCanvasThread()) {
                throw new RuntimeException("F2CanvasHandle operations must on render thread.");
            }
            return true;
        }

        @Override
        public void setFillStyle(String style) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setFillStyle(style);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setFillStyle exception" + e);
            }
        }

        public void setStrokeStyle(String style) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setStrokeStyle(style);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setStrokeStyle exception" + e);
            }
        }

        public void rect(int x, int y, int width, int height) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().rect(x, y, width, height);
                }
            } catch (Exception e) {
                innerExceptionLog( "#rect exception" + e);
            }
        }

        @Override
        public void fillRect(int x, int y, int width, int height) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().fillRect(x, y, width, height);
                }
            } catch (Exception e) {
                innerExceptionLog( "#fillRect exception" + e);
            }
        }

        public void strokeRect(int x, int y, int width, int height) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().strokeRect(x, y, width, height);
                }
            } catch (Exception e) {
                innerExceptionLog( "#strokeRect exception" + e);
            }
        }

        public void clearRect(int x, int y, int width, int height) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().clearRect(x, y, width, height);
                }
            } catch (Exception e) {
                innerExceptionLog( "#clearRect exception" + e);
            }
        }

        @Override
        public void fill() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().fill();
                }
            } catch (Exception e) {
                innerExceptionLog( "#fill exception" + e);
            }
        }

        public void stroke() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().stroke();
                }
            } catch (Exception e) {
                innerExceptionLog( "#stroke exception" + e);
            }
        }

        @Override
        public void beginPath() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().beginPath();
                }
            } catch (Exception e) {
                innerExceptionLog( "#beginPath exception" + e);
            }
        }

        public void moveTo(float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().moveTo(x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#moveTo exception" + e);
            }
        }

        public void closePath() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().closePath();
                }
            } catch (Exception e) {
                innerExceptionLog( "#closePath exception" + e);
            }
        }

        public void lineTo(float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().lineTo(x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#lineTo exception" + e);
            }
        }

        public void clip() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().clip();
                }
            } catch (Exception e) {
                innerExceptionLog( "#clip exception" + e);
            }
        }

        public void quadraticCurveTo(float cpx, float cpy, float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().quadraticCurveTo(cpx, cpy, x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#quadraticCurveTo exception" + e);
            }
        }

        public void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#bezierCurveTo exception" + e);
            }
        }

        @Override
        public void arc(float x, float y, float r, float sAngle, float eAngle, boolean antiClockwise) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().arc(x, y, r, sAngle, eAngle, antiClockwise);
                }
            } catch (Exception e) {
                innerExceptionLog( "#arc exception" + e);
            }
        }

        public void arcTo(float x1, float y1, float x2, float y2, float r) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().arcTo(x1, y1, x2, y2, r);
                }
            } catch (Exception e) {
                innerExceptionLog( "#arcTo exception" + e);
            }
        }

        public void scale(float sw, float sh) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().scale(sw, sh);
                }
            } catch (Exception e) {
                innerExceptionLog( "#scale exception" + e);
            }
        }

        public void rotate(float angle) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().rotate(angle);
                }
            } catch (Exception e) {
                innerExceptionLog( "#rotate exception" + e);
            }
        }

        public void translate(float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().translate(x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#translate exception" + e);
            }
        }

        public void transform(float a, float b, float c, float d, float e, float f) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().transform(a, b, c, d, e, f);
                }
            } catch (Exception exception) {
                innerExceptionLog( "#transform exception" + e);
            }
        }

        public void setTransform(float a, float b, float c, float d, float e, float f) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setTransform(a, b, c, d, e, f);
                }
            } catch (Exception exception) {
                innerExceptionLog( "#setTransform exception" + e);
            }
        }

        public void setLineCap(String lineCap) {

            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setLineCap(lineCap);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setLineCap exception" + e);
            }
        }

        public void setLineJoin(String lineJoin) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setLineJoin(lineJoin);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setLineJoin exception" + e);
            }
        }

        @Override
        public void setLineWidth(float lineWidth) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setLineWidth(lineWidth);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setLineWidth exception" + e);
            }
        }

        public void setLineDashOffset(float lineDashOffset) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setLineDashOffset(lineDashOffset);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setLineDashOffset exception" + e);
            }
        }

        public float[] getLineDashOffset() {
            throw new UnsupportedOperationException("getLineDashOffset is not support.");
        }

        public void setMiterLimit(float miterLimit) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setMiterLimit(miterLimit);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setMiterLimit exception" + e);
            }
        }

        public void setFont(String font) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setFont(font);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setFont exception" + e);
            }
        }

        @Override
        public void setTextAlign(String textAlign) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setTextAlign(textAlign);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setTextAlign exception" + e);
            }
        }

        public String getTextAlign() {
            throw new UnsupportedOperationException("getTextAlign is not support.");
        }

        public void setTextBaseline(String textBaseline) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setTextBaseline(textBaseline);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setTextBaseline exception" + e);
            }
        }

        public String getTextBaseline() {
            throw new UnsupportedOperationException("getTextBaseline is not support.");
        }

        @Override
        public void fillText(String text, float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().fillText(text, x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#fillText exception" + e);
            }
        }

        public void fillText(String text, float x, float y, float maxWidth) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().fillText(text, x, y, maxWidth);
                }
            } catch (Exception e) {
                innerExceptionLog( "#fillText maxWidth exception" + e);
            }
        }

        public void strokeText(String text, float x, float y) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().strokeText(text, x, y);
                }
            } catch (Exception e) {
                innerExceptionLog( "#strokeText exception" + e);
            }
        }

        public void strokeText(String text, float x, float y, float maxWidth) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().strokeText(text, x, y, maxWidth);
                }
            } catch (Exception e) {
                innerExceptionLog( "#strokeText maxWidth exception" + e);
            }
        }

        public float measureText(String text) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().measureText(text);
                }
            } catch (Exception e) {
                innerExceptionLog( "#measureText exception" + e);
            }
            return 0.f;
        }

        public void setGlobalAlpha(float alpha) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setGlobalAlpha(alpha);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setGlobalAlpha exception" + e);
            }
        }

        public float getGlobalAlpha() {
            throw new UnsupportedOperationException("getGlobalAlpha is not support.");
        }

        public void setGlobalCompositeOperation(String op) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setGlobalCompositeOperation(op);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setGlobalCompositeOperation exception" + e);
            }
        }

        @Override
        public void save() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().save();
                }
            } catch (Exception e) {
                innerExceptionLog( "#save exception" + e);
            }
        }

        @Override
        public void restore() {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().restore();
                }
            } catch (Exception e) {
                innerExceptionLog( "#restore exception" + e);
            }
        }

        public void setShadowColor(String color) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setShadowColor(color);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setShadowColor exception" + e);
            }
        }

        public void setShadowBlur(int v) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setShadowBlur(v);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setShadowBlur exception" + e);
            }
        }

        public void setShadowOffsetX(float v) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setShadowOffsetX(v);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setShadowOffsetX exception" + e);
            }
        }

        public void setShadowOffsetY(float v) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().setShadowOffsetY(v);
                }
            } catch (Exception e) {
                innerExceptionLog( "#setShadowOffsetY exception" + e);
            }
        }

        public void drawImage(String imageId, float imgWidth, float imgHeight, float srcX, float srcY, float srcWidth, float srcHeight, float destX, float destY, float destWidth, float destHeight) {
            try {
                if (checkCanvasView()) {
                    mCanvasView.get().getCanvas().getCanvas2DContext().drawImage(imageId, imgWidth, imgHeight, srcX, srcY, srcWidth, srcHeight, destX, destY, destWidth, destHeight);
                }
            } catch (Exception e) {
                innerExceptionLog( "#drawImage exception" + e);
            }
        }

    }
}
