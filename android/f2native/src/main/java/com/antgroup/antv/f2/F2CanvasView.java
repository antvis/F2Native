package com.antgroup.antv.f2;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.antgroup.antv.f2.base.F2BaseCanvasView;

import org.json.JSONArray;
import org.json.JSONObject;

public class F2CanvasView extends FrameLayout {

    protected F2BaseCanvasView mF2BaseCanvasView;
    public static boolean mLoadedPlotLibrary = false;

    static {
        if (!mLoadedPlotLibrary) {
            System.loadLibrary("f2");
            mLoadedPlotLibrary = true;
        }
    }

    public F2CanvasView(Context context) {
        this(context, null);
    }

    public F2CanvasView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    protected void init(Context context, AttributeSet attrs) {
        mF2BaseCanvasView = new F2AndroidCanvasView(context, attrs, this);
        ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        addView(mF2BaseCanvasView.getView(), params);
    }

    protected void init(int width, int height, F2Config config) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.init(width, height, config);
        }
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

    public boolean swapBuffer() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.swapBuffer();
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

    public boolean hasAdapter() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.hasAdapter();
        }
        return false;
    }

    public void sendRenderDetectEvent(long renderDuration, boolean renderSuccess, int renderCmdCount,
                                      boolean drawSuccess, String chartId) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.sendRenderDetectEvent(renderDuration, renderSuccess, renderCmdCount, drawSuccess, chartId);
        }
    }

    public boolean isDrawSuccess() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.isDrawSuccess();
        }
        return false;
    }

    public boolean hadOOM() {
        if (mF2BaseCanvasView != null) {
            return mF2BaseCanvasView.hadOOM();
        }
        return false;
    }

    public interface Adapter {
        void onCanvasDraw(F2CanvasView canvasView);

        void onDestroy();
    }

    public static class ConfigBuilder extends F2Config.Builder<ConfigBuilder> {
        protected static final String KEY_CANVAS_BIZ_ID = "canvasBizId";
        protected static final String KEY_APP_ID = "appId";

        public ConfigBuilder canvasBizId(String canvasBizId) {
            return setOption(KEY_CANVAS_BIZ_ID, canvasBizId);
        }

        public ConfigBuilder appId(String appId) {
            return setOption(KEY_APP_ID, appId);
        }

        @Override
        public F2Config build() {
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

    native static long nCreateCanvasContextHandle(Object thisObj);

    native static void nDestroyCanvasContextHandle(long canvasHolder);
}
