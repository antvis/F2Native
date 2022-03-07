package com.antgroup.antv.f2.base;

import android.view.View;

import com.alipay.antgraphic.CanvasView;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2Config;
import com.antgroup.antv.f2.F2RenderThread;
import com.antgroup.antv.f2.F2RenderThreadFactory;

/**
 *  业务层调用F2CanvasView的接口，antg和native canvas子类分别实现
 */
public interface F2BaseCanvasView {

    View getView();

    void setAdapter(F2CanvasView.Adapter adapter);

    void setOnCanvasTouchListener(F2CanvasView.OnCanvasTouchListener onCanvasTouchListener);

    void initCanvasContext();

    void initCanvasContext(F2Config config);

    void initCanvasContext(F2Config config, F2RenderThreadFactory threadFactory);

    long getNativeCanvas();

    void swapBuffer();

    void appendRenderCmdCount(String name, int renderCmdCount);

    boolean isOnCanvasThread();

    F2RenderThread getRenderThread();

    void destroy();

    void postCanvasDraw();

    F2BaseCanvasHandle getCanvasHandle();

    boolean hasAdapter();

}
