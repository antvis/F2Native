package com.antgroup.antv.f2.gesture;

import android.content.Context;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;

import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2Log;

import org.json.JSONArray;

public class F2GestureListener implements View.OnTouchListener {
    
    private static final String TAG = F2GestureListener.class.getSimpleName();

    private Context mContext;
    protected boolean mIsLongPress = false;
    protected boolean mOnScaling = false;
    protected int myPointerDown = MotionEvent.ACTION_POINTER_DOWN;

    private F2CanvasView.OnCanvasGestureListener mOnCanvasGestureListener;
    private GestureDetector mGestureDetector;
    protected ScaleGestureDetector mScaleGestureDetector;

    public F2GestureListener(Context context) {
        this.mContext = context;
    }

    public void setOnCanvasGestureListener(F2CanvasView.OnCanvasGestureListener gestureListener) {
        this.mOnCanvasGestureListener = gestureListener;
        initGestureDetector();
        initScaleGestureDetector();
    }

    private void initScaleGestureDetector() {
        if (mScaleGestureDetector != null) {
            return;
        }
        if (this.mContext == null) {
            return;
        }
        mScaleGestureDetector = new ScaleGestureDetector(mContext, new ScaleGestureDetector.SimpleOnScaleGestureListener() {

            @Override
            public boolean onScaleBegin(ScaleGestureDetector detector) {
                mOnScaling = true;
                return true;
            }

            @Override
            public void onScaleEnd(ScaleGestureDetector detector) {
                mOnScaling = false;
            }
        });
    }

    private void initGestureDetector() {
        if (mGestureDetector != null) {
            return;
        }
        if (this.mContext == null) {
            return;
        }
        mGestureDetector = new GestureDetector(mContext, new GestureDetector.SimpleOnGestureListener() {

            @Override
            public boolean onDown(MotionEvent e) {
                return true;
            }

            @Override
            public boolean onSingleTapUp(MotionEvent e) {
                F2Log.get().i("F2GestureListener", "#GestureDetector onSingleTapUp  ");
                return mOnCanvasGestureListener.onSingleTapUp(e);
            }

            @Override
            public void onLongPress(MotionEvent e) {
                mOnCanvasGestureListener.onLongPress(e, true, F2CanvasView.OnCanvasGestureListener.TOUCH_MOVE_TYPE);
                mIsLongPress = true;
            }

            @Override
            public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
                if (mOnScaling || mIsLongPress
                        || (myPointerDown != MotionEvent.ACTION_POINTER_DOWN && myPointerDown != MotionEvent.ACTION_DOWN)) {
                    return true;
                }
                return mOnCanvasGestureListener.onScroll(e1, e2, distanceX, distanceY);
            }
        });
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (mGestureDetector != null) {
            mGestureDetector.onTouchEvent(event);
        }
        if (mScaleGestureDetector != null) {
            mScaleGestureDetector.onTouchEvent(event);
        }
        switch (event.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
                myPointerDown = event.getAction();
                mOnCanvasGestureListener.onTouchChange(event, true);
                break;
            case MotionEvent.ACTION_CANCEL:
            case MotionEvent.ACTION_UP:
                if (mIsLongPress) {
                    mOnCanvasGestureListener.onLongPress(event, false, F2CanvasView.OnCanvasGestureListener.TOUCH_END_TYPE);
                    mIsLongPress = false;
                }
                if (mOnScaling){
                    mOnCanvasGestureListener.onScale(event, false,F2CanvasView.OnCanvasGestureListener.TOUCH_END_TYPE);
                    mOnScaling = false;
                }
                mOnCanvasGestureListener.onTouchChange(event, false);
                break;
            case MotionEvent.ACTION_MOVE:
                if (mIsLongPress) {
                    mOnCanvasGestureListener.onLongPress(event, true, F2CanvasView.OnCanvasGestureListener.TOUCH_MOVE_TYPE);
                }
                if (mOnScaling){
                     mOnCanvasGestureListener.onScale(event, true,F2CanvasView.OnCanvasGestureListener.TOUCH_MOVE_TYPE);
                }
                break;
        }
        return true;
    }
    
    public static String createSinglePointEvent(float x, float y, String eventType) {
        F2Config.Builder builder = new F2Config.Builder();
        builder.setOption("eventType", eventType);
        JSONArray points = new JSONArray();
        try {
            org.json.JSONObject point = new org.json.JSONObject();
            point.put("x", x);
            point.put("y", y);
            points.put(point);
        } catch (Exception e) {
            F2Log.get().e(TAG, "#F2GestureListener.createSinglePointEvent exception " + e.toString());
        }
        builder.setOption("points", points);
        return builder.build().toJsonString();
    }

    public static String createMorePointEvent(MotionEvent event, String eventType) {
        F2Config.Builder builder = new F2Config.Builder();
        builder.setOption("eventType", eventType);
        org.json.JSONArray points = new org.json.JSONArray();
        try {
            if (event.getPointerCount() > 1) {
                for (int i = 0; i < event.getPointerCount(); i++) {
                    org.json.JSONObject point = new org.json.JSONObject();
                    point.put("x", event.getX(i));
                    point.put("y", event.getY(i));
                    points.put(point);
                }
            } else {
                org.json.JSONObject point = new org.json.JSONObject();
                point.put("x", event.getX());
                point.put("y", event.getY());
                points.put(point);
            }
        } catch (Exception e) {
            F2Log.get().e(TAG, "#createMorePointEvent exception " + e.toString());
        }
        builder.setOption("points", points);
        return builder.build().toJsonString();
    }
}
