package com.antgroup.antv.f2.base;

import android.view.View;

import com.antgroup.antv.f2.F2CanvasView;

/**
 * 业务层调用F2CanvasView的接口，antg和native canvas子类分别实现
 */
public interface F2BaseCanvasView {

    // 单位是px
    void setCanvasInfo(int widthPixel, int heightPixel, String canvasBizId);

    View getView();

    void setAdapter(F2CanvasView.Adapter adapter);

    void setOnCanvasTouchListener(F2CanvasView.OnCanvasTouchListener onCanvasTouchListener);

    void setOnCanvasGestureListener(F2CanvasView.OnCanvasGestureListener onCanvasGestureListener);

    void setCanvasBizId(String canvasBizId);

    long getNativeCanvas();

    boolean swapBuffer();

    void destroy();

    void postCanvasDraw();

    boolean hasAdapter();

    void sendRenderDetectEvent(long renderDuration, boolean renderSuccess, int renderCmdCount,
                               boolean drawSuccess, String chartId);

    boolean isDrawSuccess();

    boolean hadOOM();

}
