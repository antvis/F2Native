package com.antgroup.antv.f2;

import org.json.JSONArray;
import org.json.JSONObject;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.antgroup.antv.f2.base.F2BaseCanvasHandle;
import com.antgroup.antv.f2.base.F2BaseCanvasView;

public class F2CanvasView extends FrameLayout {

    private F2BaseCanvasView mF2BaseCanvasView;

    public F2CanvasView(Context context) {
        this(context, null);
    }

    public F2CanvasView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        mF2BaseCanvasView = new F2AndroidCanvasView(context, attrs, this);
        ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        addView(mF2BaseCanvasView.getView(), params);
    }

    private void innerLog(String content) {
        F2Log.i("F2CanvasView", content);
    }

    public void initCanvasContext() {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.initCanvasContext();
        }
    }

    public void initCanvasContext(F2Config config) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.initCanvasContext(config);
        }
    }

    public long getNativeCanvas() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.getNativeCanvas();
        }
        return 0;
    }

    public void swapBuffer() {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.swapBuffer();
        }
    }

    public void appendRenderCmdCount(String name, int renderCmdCount) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.appendRenderCmdCount(name, renderCmdCount);
        }
    }

    public boolean isOnCanvasThread() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.isOnCanvasThread();
        }
        return false;
    }

    public void setAdapter(final F2CanvasView.Adapter adapter) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.setAdapter(adapter);
        }
    }

    public void destroy() {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.destroy();
        }
        removeAllViews();
    }

    public void postCanvasDraw() {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.postCanvasDraw();
        }
    }

    public void setOnCanvasTouchListener(final OnCanvasTouchListener onCanvasTouchListener) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.setOnCanvasTouchListener(onCanvasTouchListener);
        }
    }

    public F2BaseCanvasHandle getCanvasHandle() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.getCanvasHandle();
        }
        return null;
    }

    public boolean hasAdapter() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.hasAdapter();
        }
        return false;
    }

    private Boolean mIsAndroidCanvas;

    public Boolean isAndroidCanvas() {
        if (mIsAndroidCanvas == null) {
            mIsAndroidCanvas = F2CSUtils.isAndroidCanvas();
        }
        return mIsAndroidCanvas;
    }


    public interface Adapter {
        void onCanvasDraw(F2CanvasView canvasView);

        void onDestroy();
    }

    public static class ConfigBuilder extends F2Config.Builder<ConfigBuilder> {
        protected static final String KEY_ASYNC_RENDER = "asyncRender";
        protected static final String KEY_BACKGROUND_COLOR = "backgroundColor";
        protected static final String KEY_CANVAS_BIZ_ID = "canvasBizId";
        protected static final String KEY_APP_ID = "appId";
        protected static final String KEY_NATIVE_DRAW = "useGLFunctor";
        protected static final String KEY_CANVAS_TYPE = "canvasType";

        public ConfigBuilder canvasId(String canvasId) {
            return setOption(F2Constants.CANVAS_ID, canvasId);
        }

        public ConfigBuilder asyncRender(boolean async) {
            return setOption(KEY_ASYNC_RENDER, async);
        }

        public ConfigBuilder backgroundColor(String color) {
            return setOption(KEY_BACKGROUND_COLOR, color);
        }

        public ConfigBuilder canvasBizId(String canvasBizId) {
            return setOption(KEY_CANVAS_BIZ_ID, canvasBizId);
        }

        public ConfigBuilder appId(String appId) {
            return setOption(KEY_APP_ID, appId);
        }

        public ConfigBuilder useNativeDraw(boolean b) {
            return setOption(KEY_NATIVE_DRAW, b);
        }

        public ConfigBuilder canvasType(String canvasType) {
            return setOption(KEY_CANVAS_TYPE, canvasType);
        }

        @Override
        public F2Config build() {
            if (!options.has(F2Constants.CANVAS_ID)) {
                canvasId(F2Util.generateId());
            }
            if (!options.has(KEY_ASYNC_RENDER)) {
                asyncRender(true);
            }

            if (!options.has(KEY_CANVAS_BIZ_ID)) {
                throw new NullPointerException("Not found canvasBizId");
            }
            return super.build();
        }
    }

    public interface OnCanvasTouchListener {
        void onTouch(F2CanvasView canvasView, TouchEvent event);
    }

    public static class TouchEvent {
        private F2Config mData;
        private String touchEvent;

        public TouchEvent(MotionEvent event) {

            F2Config.Builder builder = new F2Config.Builder();
            switch (event.getActionMasked()) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN: {
                    touchEvent = "touchstart";
                    builder.setOption("eventType", "touchstart");
                    break;
                }
                case MotionEvent.ACTION_MOVE: {
                    touchEvent = "touchmove";
                    builder.setOption("eventType", "touchmove");
                    break;
                }
                case MotionEvent.ACTION_UP:
                case MotionEvent.ACTION_POINTER_UP: {
                    touchEvent = "touchend";
                    builder.setOption("eventType", "touchend");
                    break;
                }
                case MotionEvent.ACTION_CANCEL: {
                    touchEvent = "touchcancel";
                    builder.setOption("eventType", "touchcancel");
                    break;
                }
                default:
                    break;
            }

            JSONArray points = new JSONArray();
            try {
                if (event.getPointerCount() > 1) {
                    for (int i = 0; i < event.getPointerCount(); i++) {
                        JSONObject point = new JSONObject();
                        point.put("x", event.getX(i));
                        point.put("y", event.getY(i));
                        points.put(point);
                    }
                } else {
                    JSONObject point = new JSONObject();
                    point.put("x", event.getX());
                    point.put("y", event.getY());
                    points.put(point);
                }
            } catch (Exception e) {
                F2Log.e("F2CanvasView", "#TouchEvent exception " + e.toString());
            }

            builder.setOption("points", points);

            mData = builder.build();
        }

        F2Config getData() {
            return mData;
        }

        public String getTouchEvent() {
            return touchEvent;
        }
    }

    private native static long nCreateCanvasContextHandle(Object thisObj);

}
