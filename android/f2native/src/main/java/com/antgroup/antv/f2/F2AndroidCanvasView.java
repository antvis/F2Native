package com.antgroup.antv.f2;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PaintFlagsDrawFilter;
import android.os.Build;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import com.antgroup.antv.f2.base.F2BaseCanvasView;
import com.antgroup.antv.f2.gesture.F2GestureListener;

/**
 * android native canvas需要的CanvasView
 *
 * @author ruize
 */
@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
public class F2AndroidCanvasView extends View implements F2BaseCanvasView {

    private PaintFlagsDrawFilter paintFlagsDrawFilter = new PaintFlagsDrawFilter(0, Paint.FILTER_BITMAP_FLAG | Paint.ANTI_ALIAS_FLAG);

    //画布
    private F2AndroidCanvasContext mCanvasContext;
    //屏幕密度
    protected float mRatio;
    //F2CanvasContext在JNI中对应的地址
    private long mCanvasHolder;
    private F2CanvasView.Adapter mAdapter;
    private F2CanvasView.OnCanvasTouchListener mOnCanvasTouchListener;
    private F2CanvasView.OnCanvasGestureListener mOnCanvasGestureListener;
    private F2GestureListener mF2GestureListener;

    private boolean isTouchProcessing = false;

    private int mHeight; // View的宽
    private int mWidth; // View的高
    private F2CanvasView mF2CanvasView;
    protected String mCanvasBizId = "";
    // 是否首次绘制
    private boolean mFirstRender = true;
    protected String TAG = "F2AndroidCanvasView";

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

    public void setCanvasInfo(int widthPixel, int heightPixel, String canvasBizId) {
        setCanvasBizId(canvasBizId);
        setCanvasSize(widthPixel, heightPixel);
    }

    protected boolean setCanvasSize(int width, int height) {
        if (0 == width || 0 == height) {
            innerLog("#setCanvasSize width or height is 0 " + mCanvasBizId);
            return false;
        }
        if (mHeight == height && mWidth == width) {
            return false;
        }
        mHeight = height;
        mWidth = width;
        innerLog("#setCanvasSize  width = " + width + ", height = " + height + ", bizId = " + mCanvasBizId);
        long startTime = System.currentTimeMillis();
        mCanvasContext = new F2AndroidCanvasContext(width, height, mRatio);
        try {
            if (mCanvasContext != null) {
                mCanvasHolder = F2CanvasView.nCreateCanvasContextHandle(mCanvasContext);
            }
        } catch (Exception e) {
            innerLog("#setCanvasSize " + mCanvasBizId + " ,catch exception:" + e.getMessage());
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
        if (F2CommonHelper.get().isChartSizeChangeEnable()) {
            boolean change = setCanvasSize(getWidth() - (getPaddingLeft() + getPaddingRight()), getHeight() - (getPaddingTop() + getPaddingBottom()));
            if (change) {
                innerLog("#onDraw change ");
                postCanvasDraw();
            }
        }
        if (null != mCanvasContext && null != mCanvasContext.bitmap) {
            canvas.drawBitmap(mCanvasContext.bitmap, getPaddingLeft(), getPaddingTop(), null);
            innerLog("#onDraw success " + mCanvasBizId);
        } else {
            innerLog("#onDraw canvasContext is null " + mCanvasBizId);
        }
    }

    @Override
    public void sendRenderDetectEvent(final long renderDuration, final boolean renderSuccess, final int renderCmdCount,
                                      final boolean drawSuccess, final String chartId) {
        // 开关打开 && 上屏成功，才进行白屏检测
        if (F2CommonHelper.get().isDetectEnable() && drawSuccess && mCanvasContext != null && mCanvasContext.bitmap != null && mCanvasHolder != 0) {
            F2CommonHelper.get().sendRenderDetectEvent(mCanvasContext.bitmap.copy(Bitmap.Config.ARGB_8888, false),
                    mCanvasBizId, renderDuration, renderSuccess,
                    renderCmdCount, drawSuccess, mWidth, mHeight, mRatio, chartId, mFirstRender);
        } else {
            // 说明开关关闭 || 上屏失败
            String desc = chartId + (mCanvasContext != null && mCanvasContext.mHadOOM ? "_OOM" : "");
            F2CommonHelper.get().eventDetectRender(mCanvasBizId, renderDuration, renderSuccess,
                    renderCmdCount, drawSuccess, null, 0, mWidth, mHeight, mRatio, desc, mFirstRender);
        }
        mFirstRender = false;
    }

    @Override
    public boolean isDrawSuccess() {
        return mCanvasContext != null && mCanvasContext.bitmap != null && mCanvasHolder != 0;
    }

    @Override
    public boolean hadOOM() {
        return mCanvasContext != null && mCanvasContext.mHadOOM;
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
        innerLog("#setAdapter " + mCanvasBizId);
        mAdapter = adapter;
        postCanvasDraw();
    }

    @Override
    public void setOnCanvasTouchListener(F2CanvasView.OnCanvasTouchListener onCanvasTouchListener) {
        mOnCanvasTouchListener = onCanvasTouchListener;
    }

    @Override
    public void setOnCanvasGestureListener(F2CanvasView.OnCanvasGestureListener onCanvasGestureListener) {
        mOnCanvasGestureListener = onCanvasGestureListener;
        createGestureDetector();
    }

    @Override
    public void setCanvasBizId(String canvasBizId) {
        innerLog("#setCanvasBizId " + canvasBizId);
        mCanvasBizId = canvasBizId;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (!useNewTouch() && mOnCanvasTouchListener != null) {
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
        innerLog("#postCanvasDraw " + mCanvasBizId);
        if (mCanvasContext != null && hasAdapter()) {
            innerLog("#postCanvasDraw resetContext " + mCanvasBizId);
            mCanvasContext.resetContext();
            mAdapter.onCanvasDraw(mF2CanvasView);
        }
    }

    @Override
    public boolean hasAdapter() {
        return mAdapter != null;
    }

    @Override
    public void destroy() {
        innerLog("#destroy " + mCanvasBizId);
        innerDestroy();
        mFirstRender = true;
        mOnCanvasTouchListener = null;
        if (mCanvasContext != null && mCanvasContext.bitmap != null) {
            mCanvasContext.bitmap.recycle();
            mCanvasContext.bitmap = null;
            mCanvasContext = null;
        }
        // 销毁图片下载
        F2ImageLoadUtil.removeHandlerCallbacks();
        //销毁对应的JNI对象
        if (mCanvasHolder != 0) {
            F2CanvasView.nDestroyCanvasContextHandle(mCanvasHolder);
            mCanvasHolder = 0;
        }
    }

    private void innerDestroy() {
        if (mAdapter != null) {
            mAdapter.onDestroy();
        }
    }

    @Override
    public boolean swapBuffer() {
        invalidate();
        innerLog("#swapBuffer");
        return null != mCanvasContext && null != mCanvasContext.bitmap && mCanvasHolder != 0;
    }

    @Override
    public final long getNativeCanvas() {
        return mCanvasHolder;
    }

    private void innerLog(String content) {
        F2Log.get().i(TAG, hashCode() + ", " + content);
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            innerLog("finalize...");
            destroy();
        } catch (Exception e) {
            F2Log.get().e(TAG, "#finalize exception " + e.toString());
        } finally {
            super.finalize();
        }
    }

    private boolean useNewTouch() {
        return mOnCanvasGestureListener != null && mF2GestureListener != null;
    }

    private void createGestureDetector() {
        if (mF2GestureListener == null) {
            mF2GestureListener = new F2GestureListener(getContext());
            setOnTouchListener(mF2GestureListener);
        }
        mF2GestureListener.setOnCanvasGestureListener(mOnCanvasGestureListener);
    }
}