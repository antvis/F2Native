package com.antgroup.antv.f2;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.SurfaceTexture;
import android.os.Build;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.TextureView;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * @author qingyuan.yl
 * @date 2020-09-17
 */
@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
public class F2CanvasView extends TextureView implements TextureView.SurfaceTextureListener {

    private static final String KEY_UPDATE_LAYER = "mUpdateLayer";
    private static final String KEY_UPDATE_LISTENER = "mUpdateListener";

    public interface Adapter {
        void onCanvasDraw(F2CanvasView canvasView);

        void onDestroy();
    }

    public interface OnCanvasTouchListener {
        void onTouch(F2CanvasView canvasView, TouchEvent event);
    }

    private boolean hasCallInitCanvas = false;
    private NativeCanvasHolder mCanvasHolder;
    private Surface surface;

    // 反射系统字段
    private static Field fieldUpdateLayer;
    private static Field fieldUpdateListener;

//    private boolean replaceFrameListenerFlag = false;

    // 系统frame listener
    private SurfaceTexture.OnFrameAvailableListener systemFrameListener;

    // 是否替换了TextureView默认刷新，使用canvas frame刷新机制
    // 使用CanvasFrameUpdate的方式刷新很稳，但有可能慢一点
    // 在小程序中问题更严重，由于需要在SurfaceTextureUpdated()中再postInvalidate解决UC兼容问题，
    // 这个会导致CanvasFrameUpdate的post(canvasFrameUpdateRunnable)必须等待上次invlidate执行完，
    // 最终FPS会降到30，因此小程序下不要使用canvasFrameUpdate
    protected boolean useCanvasFrameUpdate = true;

    // 当前是否有frame待渲染
    private volatile boolean canvasFrameWaitToDraw;

    // 用来替换系统TextureView默认FrameListener的，如果替换成功 则更新帧走此listener
    private SurfaceTexture.OnFrameAvailableListener canvasFrameAvailableListener;

    private Runnable canvasFrameUpdateRunnable;
    private Runnable canvasFrameUpdateFinishRunnable;

    private final Object canvasFrameLock = new Object();

    private Adapter mAdapter;
    private OnCanvasTouchListener mOnCanvasTouchListener;

    private CanvasListener mCanvasListener;

    public F2CanvasView(Context context) {
        this(context, null);
    }

    public F2CanvasView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public F2CanvasView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        setOpaque(false);
        setSurfaceTextureListener(this);
        mCanvasListener = new CanvasListener(this);

        canvasFrameAvailableListener = new SurfaceTexture.OnFrameAvailableListener() {
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                if (!useCanvasFrameUpdate) {
                    setCanvasFrameWaitToDraw(true);
                    scheduleDrawCanvasFrame();
                }
            }
        };

        canvasFrameUpdateRunnable = new Runnable() {
            @Override
            public void run() {
                scheduleDrawCanvasFrame();
            }
        };

        canvasFrameUpdateFinishRunnable = new Runnable() {
            @Override
            public void run() {
                // 必须先设置frame状态
                synchronized (canvasFrameLock) {
                    canvasFrameWaitToDraw = false;
                }
            }
        };

        initReflectGetField();
        tryReplaceFrameUpdateListener();
    }

    public void setAdapter(Adapter adapter) {
        if (mAdapter != null && mAdapter != adapter) {
            mAdapter.onDestroy();
        }
        mAdapter = adapter;
        if (mAdapter == null) return;

        postCanvasDraw();
    }

    public void setOnCanvasTouchListener(OnCanvasTouchListener onCanvasTouchListener) {
        mOnCanvasTouchListener = onCanvasTouchListener;
    }

    public void initCanvasContext() {
        initCanvasContext(null);
    }

    public void initCanvasContext(F2Config config) {
        if (hasCallInitCanvas) return;
        innerLog("#initCanvasContext  callInitCanvas");
        callInitCanvas(getContext(), config);
    }

    @Override
    public boolean onTouchEvent(final MotionEvent event) {
        if (mOnCanvasTouchListener != null) {
            mCanvasHolder.runOnRenderThread(new Runnable() {
                @Override
                public void run() {
                    mOnCanvasTouchListener.onTouch(F2CanvasView.this, new TouchEvent(F2CanvasView.this, event));
                }
            });

            if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                return true;
            }
        }
        return super.onTouchEvent(event);
    }

    public void postCanvasDraw() {
        if (!hasCallInitCanvas || mCanvasHolder == null || mAdapter == null || surface == null) return;
        mCanvasHolder.runOnRenderThread(new Runnable() {
            @Override
            public void run() {
                innerLog("#postCanvasDraw");
                mAdapter.onCanvasDraw(F2CanvasView.this);
            }
        });
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        innerLog("#onDetachedFromWindow");
    }

    private void callInitCanvas(Context context, F2Config config) {
        if (hasCallInitCanvas)  {
            return;
        }

        if (config == null) {
            config = new ConfigBuilder().build();
        }

        mCanvasHolder = new NativeCanvasHolder(context, config);
        mCanvasHolder.setListener(mCanvasListener);
        hasCallInitCanvas = true;

        innerLog("#callInitCanvas");
    }

    final void onSurfaceReady() {
        if (mCanvasHolder != null) {
            innerLog("#onSurfaceReady call holder.initCanvasContext");
            mCanvasHolder.initCanvasContext();
        }

        if (mAdapter != null) {
            mAdapter.onCanvasDraw(this);
        }
    }

    final void onCanvasFrameUpdate() {
        innerLog("#onCanvasFrameUpdate");
        if (mCanvasHolder == null || !mCanvasHolder.isValid() || !mCanvasHolder.isSurfaceReady()) {
            return;
        }

        if (useCanvasFrameUpdate) {
            setCanvasFrameWaitToDraw(true);
            post(canvasFrameUpdateRunnable);
        }
    }

    final void onCanvasDestroyed() {
        setSurfaceTextureListener(null);
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture texture, int width, int height) {
        if (!isCanvasFrameWaitToDraw()) {
            cancelFrameUpdate();
        }

        if (surface == null) {
            surface = new Surface(texture);
            innerLog("#onSurfaceTextureAvailable create surface: " + surface);
            if (mCanvasHolder != null) {
                mCanvasHolder.onSurfaceAvailable(surface, width, height);
            }
        } else {
            innerLog("#onSurfaceTextureAvailable again, surface is already initialized. surface:  " + surface);
        }
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture texture, int width, int height) {
        innerLog("#onSurfaceTextureSizeChanged ");
        if (!isCanvasFrameWaitToDraw()) {
            cancelFrameUpdate();
        }

        if (mCanvasHolder != null) {
            mCanvasHolder.onSurfaceSizeChanged(surface, width, height);
        }
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture texture) {
        innerLog("#onSurfaceTextureDestroyed ");
        if (useCanvasFrameUpdate) {
            removeCallbacks(canvasFrameUpdateRunnable);
        }

        canvasFrameUpdateFinishRunnable.run();

        if (mCanvasHolder != null) {
            mCanvasHolder.onSurfaceDestroyed(surface);
        }

        if (surface != null && surface.isValid()) {
            surface.release();
        }
        surface = null;
        return true;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        innerLog("#onSurfaceTextureUpdated");
        post(canvasFrameUpdateFinishRunnable);
    }

    public void destroy() {
        innerLog("#destroy");
        if (mAdapter != null) {
            mAdapter.onDestroy();
        }

        if (mCanvasHolder != null) {
            mCanvasHolder.destroy();
        }

        mAdapter = null;
        mOnCanvasTouchListener = null;
    }

    public void swapBuffer() {
        // proxy swap buffer
        if (mCanvasHolder != null) {
            mCanvasHolder.swapBuffer();
        }
    }

    protected void setCanvasFrameWaitToDraw(boolean v) {
        synchronized (canvasFrameLock) {
            canvasFrameWaitToDraw = v;
        }
    }

    public boolean isCanvasFrameWaitToDraw() {
        boolean result;
        synchronized (canvasFrameLock) {
            result = canvasFrameWaitToDraw;
        }
        return result;
    }

    private void scheduleDrawCanvasFrame() {
        // innerLog("scheduleDrawCanvasFrame");
        if (systemFrameListener != null) { // 如果系统listener存在，则调用系统接口
            systemFrameListener.onFrameAvailable(getSurfaceTexture());
        } else {
            hackUpdateLayer();
            invalidate();
        }
    }

    private void cancelFrameUpdate() {
        hackCancelLayerUpdate();
        canvasFrameUpdateFinishRunnable.run();
    }

    final NativeCanvasHolder getCanvasHolder() {
        return mCanvasHolder;
    }

    private void innerLog(String content) {
        if (mCanvasHolder == null) {
            F2Log.i("F2NativeView|uninited", content);
        } else {
            F2Log.i("F2NativeView|" + mCanvasHolder.getCanvasOptions().getStringField(F2Constants.CANVAS_ID), content);
        }
    }

    private void initReflectGetField() {
        try {
            if (fieldUpdateLayer == null) {
                fieldUpdateLayer = queryFieldByName(KEY_UPDATE_LAYER);
            }

            if (fieldUpdateListener == null) {
                fieldUpdateListener = queryFieldByName(KEY_UPDATE_LISTENER);
            }
        } catch (Exception e) {
            F2Log.e("F2CanvasView", "#initReflectGetField", e);
        }
    }


    private Field queryFieldByName(String fieldName) {
        Field result = null;
        try {
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.P) {
                result = F2Util.reflectGetFieldByName(TextureView.class.getDeclaredFields(), fieldName);
            } else {
                Method metaGetDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredFields");
                // 系统类通过反射使用隐藏 API，检查直接通过。
                Object fields = metaGetDeclaredMethod.invoke(TextureView.class);
                if (fields instanceof Field[]) {
                    Field[] fieldsArray = (Field[]) fields;
                    Field f = F2Util.reflectGetFieldByName(fieldsArray, fieldName);
                    if (f != null) {
                        result = f;
                    }
                }
            }
        } catch (Exception e) {
            F2Log.e("F2CanvasView", "#queryFieldByName: " + fieldName, e);
        }

        return result;
    }

    private void tryReplaceFrameUpdateListener() {
//        replaceFrameListenerFlag = false;
        if (fieldUpdateListener == null) {
            innerLog("replaceFrameUpdate:fail,fieldUpdateListener=null");
        } else {
            try {
                // 注意，系统queue的Buffer如果一直不消费，会导致下次buffer full, buffer full后不再触发onFrameListener
                // 不依赖OnFrameAvailableListener?
                fieldUpdateListener.setAccessible(true);
                systemFrameListener = (SurfaceTexture.OnFrameAvailableListener) fieldUpdateListener.get(this);
                if (systemFrameListener != null) {
//                    replaceFrameListenerFlag = true;
                    fieldUpdateListener.set(this, canvasFrameAvailableListener);
                    innerLog("replaceFrameUpdate:success,override to canvas frame");
                } else {
                    innerLog("replaceFrameUpdate:fail,query systemUpdateListener null");
                }
            } catch (Exception ex) {
                F2Log.e("F2CanvasView", "#tryReplaceFrameUpdateListener", ex);
                try {
                    // 回滚
                    if (systemFrameListener != null) {
                        fieldUpdateListener.set(this, systemFrameListener);
                        innerLog("replaceFrameUpdate:fail,rollback set update to systemUpdateListener");
                    }
                } catch (Exception ex2) {
                    F2Log.e("F2CanvasView", "#tryReplaceFrameUpdateListener", ex2);
                }
            }
        }
    }

    private void hackUpdateLayer() {
        hackInnerUpdateLayer(true);
    }

    private void hackCancelLayerUpdate() {
        hackInnerUpdateLayer(false);
    }

    private void hackInnerUpdateLayer(boolean v) {
        try {
            if (fieldUpdateLayer != null) {
                // innerLog("hackUpdateLayer:" + v);
                fieldUpdateLayer.setAccessible(true);
                fieldUpdateLayer.set(this, v);
            } else {
                innerLog("hackUpdateLayer:fail,mUpdateLayer not exist");
            }
        } catch (Exception e) {
            F2Log.e("F2CanvasView", "#hackInnerUpdateLayer " + v, e);
        }
    }

    private static class CanvasListener implements NativeCanvasHolder.ICanvasListener {
        WeakReference<F2CanvasView> mChartView;

        CanvasListener(F2CanvasView canvasView) {
            this.mChartView = new WeakReference<>(canvasView);
        }

        @Override
        public void onSurfaceReady() {
            F2CanvasView canvasView = null;
            if ((canvasView = mChartView.get()) != null) {
                canvasView.onSurfaceReady();
            }
        }

        @Override
        public void onCanvasFrameUpdate() {
            F2CanvasView canvasView = null;
            if ((canvasView = mChartView.get()) != null) {
                canvasView.onCanvasFrameUpdate();
            }
        }

        @Override
        public void onCanvasDestroyed() {
            F2CanvasView canvasView = null;
            if ((canvasView = mChartView.get()) != null) {
                canvasView.onCanvasDestroyed();
            }
        }
    }

    public static class TouchEvent {
        private F2Config mData;
        private TouchEvent(F2CanvasView view, MotionEvent event) {

            F2Config.Builder builder = new F2Config.Builder();
            switch (event.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    {
                    builder.setOption("eventType", "touchstart");
                    break;
                }
                case MotionEvent.ACTION_MOVE: {
                    builder.setOption("eventType", "touchmove");
                    break;
                }
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_POINTER_UP:
                    {
                    builder.setOption("eventType", "touchend");
                    break;
                }
                case MotionEvent.ACTION_CANCEL: {
                    builder.setOption("eventType", "touchcancel");
                    break;
                }
            }


            int[] location = new int[2];
            view.getLocationOnScreen(location);
            JSONArray points = new JSONArray();
            try {
                if (event.getPointerCount() > 1) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        JSONObject point = new JSONObject();
                        point.put("x", event.getX(i) - location[0]);
                        point.put("y", event.getY(i) - location[1]);
                        points.put(point);
                    }
                } else {
                    JSONObject point = new JSONObject();
                    point.put("x", event.getX() - location[0]);
                    point.put("y", event.getY() - location[1]);
                    points.put(point);
                }
            } catch (Exception e) {
            }

            builder.setOption("points", points);

            mData = builder.build();
        }

        F2Config getData() {
            return mData;
        }
    }

    public static class ConfigBuilder extends F2Config.Builder {
        protected static final String KEY_ASYNC_RENDER  = "asyncRender";
        protected static final String KEY_BACKGROUND_COLOR = "backgroundColor";

        public ConfigBuilder canvasId(String canvasId) {
            setOption(F2Constants.CANVAS_ID, canvasId);
            return this;
        }

        public ConfigBuilder asyncRender(boolean async) {
            setOption(KEY_ASYNC_RENDER, async);
            return this;
        }

        public ConfigBuilder backgroundColor(String color) {
            setOption(KEY_BACKGROUND_COLOR, color);
            return this;
        }


        @Override
        public F2Config build() {
            if (!options.has(F2Constants.CANVAS_ID)) {
                canvasId(F2Util.generateId());
            }
            if (!options.has(KEY_ASYNC_RENDER)) {
                asyncRender(true);
            }
            return super.build();
        }
    }
}
