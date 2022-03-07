package com.antgroup.antv.f2;

import java.util.ArrayList;

import android.graphics.Canvas;
import android.content.Context;

import android.annotation.TargetApi;
import android.graphics.Paint;
import android.graphics.PaintFlagsDrawFilter;
import android.os.Build;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.antgroup.antv.f2.base.F2BaseCanvasView;


/**
 * android native canvas需要的CanvasView
 * @author ruize
 */
@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
class F2AndroidCanvasView extends View implements F2BaseCanvasView {


    static {
        System.loadLibrary("f2");
    }

    private PaintFlagsDrawFilter paintFlagsDrawFilter = new PaintFlagsDrawFilter(0, Paint.FILTER_BITMAP_FLAG | Paint.ANTI_ALIAS_FLAG);

    //画布
    private F2AndroidCanvasContext mCanvasContext;
    //屏幕密度
    private float mRatio;
    //F2CanvasContext在JNI中对应的地址
    private long mCanvasHolder;
    private F2CanvasView.Adapter mAdapter;
    private F2CanvasView.OnCanvasTouchListener mOnCanvasTouchListener;
    private boolean isTouchProcessing = false;

    private int mHeight; // View的宽
    private int mWidth; // View的高
    private F2CanvasView mF2CanvasView;
    private String mAppId = "";
    private String mCanvasBizId = "";
    private ArrayList<Integer> mRenderCmdCountList = new ArrayList<>();
    private int mDrawIndex = 0;

    public F2AndroidCanvasView(Context context) {
        this(context, null);
    }

    public F2AndroidCanvasView(Context context, AttributeSet attrs) {
        this(context, attrs, null);
    }

    public F2AndroidCanvasView(Context context, AttributeSet attrs, F2CanvasView f2CanvasView) {
        super(context, attrs);
        this.mF2CanvasView = f2CanvasView;
        mRatio = context.getResources().getDisplayMetrics().density;
    }

    public boolean setCanvasSize(int width, int height) {
        if (0 == width || 0 == height) {
            innerLog("#setCanvasSize width or height is 0");
            return false;
        }
        if (mHeight == height && mWidth == width) {
            return false;
        }
        mHeight = height;
        mWidth = width;
        innerLog("#setCanvasSize  width=" + width + ",height=" + height);
        long startTime = System.currentTimeMillis();
        mCanvasContext = new F2AndroidCanvasContext(width, height, mRatio);
        try {
            mCanvasHolder = nCreateCanvasContextHandle(mCanvasContext);
        } catch (Exception e) {
            innerLog("#setCanvasSize catch exception:" + e);
            return false;
        }
        //通知业务渲染
        innerLog("#setCanvasSize create context: cost = " + (System.currentTimeMillis() - startTime) + "ms");
        return true;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.setDrawFilter(paintFlagsDrawFilter);
        boolean change = setCanvasSize(getWidth() - (getPaddingLeft() + getPaddingRight()), getHeight() - (getPaddingTop() + getPaddingBottom()));
        if (change) {
            postCanvasDraw();
        }
        if (null != mCanvasContext && null != mCanvasContext.bitmap) {
            canvas.drawBitmap(mCanvasContext.bitmap, getPaddingLeft(), getPaddingTop(), null);
        } else {
            innerLog("#onDraw canvasContext is null");
        }
    }

    private void clearRenderCmdCount() {
        mRenderCmdCountList.clear();
    }

    @Override
    public void appendRenderCmdCount(String chartName, int renderCmdCount) {
        mRenderCmdCountList.add(renderCmdCount);
    }

    private int getTotalRenderCmdCount() {
        int totalCount = 0;
        for (int i = 0; i < mRenderCmdCountList.size(); ++i) {
            int renderCmdCount = mRenderCmdCountList.get(i);
            if (renderCmdCount == 0) {
                innerLog("getTotalRenderCmdCount:ZERO at " + i);
                return 0;
            }
            totalCount += renderCmdCount;
        }
        return totalCount;
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
        innerLog("#initCanvasContext F2Config threadFactory");
        if (config != null) {
            mAppId = config.getStringField(F2CanvasView.ConfigBuilder.KEY_APP_ID);
            mCanvasBizId = config.getStringField(F2CanvasView.ConfigBuilder.KEY_CANVAS_BIZ_ID);
        }
        F2Event.eventPage(mAppId, mCanvasBizId, "init", true);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (mOnCanvasTouchListener != null) {
            final MotionEvent copyEvent = MotionEvent.obtain(event);
            if (event.getActionMasked() == MotionEvent.ACTION_DOWN && isTouchProcessing) {
                return true;
            }
            isTouchProcessing = true;
            if (mOnCanvasTouchListener != null) {
                mOnCanvasTouchListener.onTouch(mF2CanvasView, new F2CanvasView.TouchEvent(copyEvent));
            }
            isTouchProcessing = false;
            if (event.getActionMasked() == MotionEvent.ACTION_DOWN) {
                return true;
            }
        }
        return super.onTouchEvent(event);
    }

    @Override
    public void postCanvasDraw() {
        if (hasAdapter() == true && mCanvasContext != null) {
            clearRenderCmdCount();
            long startTime = System.currentTimeMillis();
            mCanvasContext.resetContext();
            mAdapter.onCanvasDraw(mF2CanvasView);
            long duration = System.currentTimeMillis() - startTime;
            if (BuildConfig.DEBUG) {
                innerLog("#onDraw onCanvasDraw cost = " + duration + "ms");
            }
            mDrawIndex++;
            F2Event.eventRender(mAppId, mCanvasBizId, mDrawIndex, getTotalRenderCmdCount(), duration, mWidth, mHeight, mRatio, true);
        }
    }

    @Override
    public final boolean isOnCanvasThread() {
        return true;
    }

    @Override
    public boolean hasAdapter() {
        return mAdapter != null;
    }

    @Override
    public void destroy() {
        innerLog("#destroy");
        innerDestroy();
        mOnCanvasTouchListener = null;
        mCanvasContext = null;
        //销毁对应的JNI对象
        if (mCanvasHolder != 0) {
            nDestroyCanvasContextHandle(mCanvasHolder);
            mCanvasHolder = 0;
        }
        clearRenderCmdCount();
        mDrawIndex = 0;
    }

    private void innerDestroy() {
        if (mAdapter != null) {
            mAdapter.onDestroy();
        }
    }

    @Override
    public void swapBuffer() {
        postInvalidate();
    }

    @Override
    public F2AndroidCanvasContext getCanvasHandle() {
        return mCanvasContext;
    }

    @Override
    public final long getNativeCanvas() {
        return mCanvasHolder;
    }

    private void innerLog(String content) {
        F2Log.i("F2AndroidCanvasView", content);
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            innerLog("finalize...");
            destroy();
        } catch (Exception e) {
            F2Log.e("F2AndroidCanvasView", "#finalize exception " + e.toString());
        } finally {
            super.finalize();
        }
    }

    private native static long nCreateCanvasContextHandle(Object thisObj);

    private native static void nDestroyCanvasContextHandle(long canvasHolder);

}
