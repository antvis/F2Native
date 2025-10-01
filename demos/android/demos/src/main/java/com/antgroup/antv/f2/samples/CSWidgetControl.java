package com.antgroup.antv.f2.samples;

import android.content.Context;
import android.view.View;

import java.util.Map;

public abstract class CSWidgetControl<T extends View> {

    public CSWidgetControl(Context context) {

    }

    public abstract T createWidgetView(Context context, Map<String, Object> map, View view, int width, int height);


    public abstract void modifyData(Map<String, Object> map);


    public abstract void invokeMethod(final String methodName, final Object param, final CSCallback cSCallback);

    public abstract void onDestroy();

    public interface CSCallback {
        void callbackKeepAlive(Map<String, Object> var1, boolean var2);
    }
}
