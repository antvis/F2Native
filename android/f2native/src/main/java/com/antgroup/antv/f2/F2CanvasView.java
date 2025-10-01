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
    public static boolean mLoadedF2Library = false;

    static {
        Class<?>[] nativeClasses = new Class<?>[] {
                F2CanvasView.class,
                F2ChartBridge.class,
                NativeChartProxy.class
        };
        F2CommonHelper.get().loadLibrary("f2", nativeClasses);
        mLoadedF2Library = true;
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

    // 单位是px
    public void setCanvasInfo(int widthPixel, int heightPixel, String canvasBizId) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.setCanvasInfo(widthPixel, heightPixel, canvasBizId);
        }
    }

    private void innerLog(String content) {
        F2Log.get().i("F2CanvasView", content);
    }

    public void setCanvasBizId(String canvasBizId) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.setCanvasBizId(canvasBizId);
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

    public void setOnCanvasGestureListener(final OnCanvasGestureListener onCanvasGestureListener) {
        if (mF2BaseCanvasView != null) {
            mF2BaseCanvasView.setOnCanvasGestureListener(onCanvasGestureListener);
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

    public interface OnCanvasTouchListener {
        void onTouch(F2CanvasView canvasView, TouchEvent event);
    }

    public interface OnCanvasGestureListener {

        String TOUCH_START_TYPE = "touchstart";
        String TOUCH_MOVE_TYPE = "touchmove";
        String TOUCH_END_TYPE = "touchend";

        boolean onSingleTapUp(MotionEvent event);

        void onLongPress(MotionEvent e, boolean isPress, String eventType);

        boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY);

        void onTouchChange(MotionEvent event, boolean b);

        boolean onScale(MotionEvent event, boolean isScale, String eventType);
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
                F2Log.get().e("F2CanvasView", "#TouchEvent exception " + e.toString());
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

    public native static long nCreateCanvasContextHandle(Object thisObj);

    public native static void nDestroyCanvasContextHandle(long canvasHolder);
}
