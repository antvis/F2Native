package com.antgroup.antv.f2.samples;

import com.antgroup.antv.f2.F2CanvasView;

import java.io.Serializable;

/**
 * @author qingyuan.yl
 * @version 10.1.1
 * @date 2020-09-25
 */
public class ChartModel implements Serializable {
    public String title;

    public Class<? extends F2CanvasView.Adapter> adapterClass;

    public boolean horizontal = false;

    public ChartModel(String title, Class<? extends F2CanvasView.Adapter> adapterClass) {
        this.title = title;
        this.adapterClass = adapterClass;
    }

    public ChartModel(String title, Class<? extends F2CanvasView.Adapter> adapterClass, boolean horizontal) {
        this.title = title;
        this.adapterClass = adapterClass;
        this.horizontal = horizontal;
    }
}
