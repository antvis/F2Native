package com.antgroup.antv.f2;

/**
 * @author qingyuan.yl
 * @date 2020/12/1
 */
public abstract class F2Function {

    protected String functionId = null;

    public F2Function() {
        try {
            this.functionId = nCreateFunction(this);
        } catch (Error error) {
            functionId = null;
        }
    }

    final void bindChart(F2Chart chart) {
        if (functionId != null && chart.getChartProxy().getNativeChartHandler() != 0) {
            nBindChart(functionId, chart.getChartProxy().getNativeChartHandler());
        }
    }

    public abstract F2Config execute(String param);

    protected final String nExecute(String param) {
        try {
            F2Config config = execute(param);
            if (config == null) return null;
            return config.toJsonString();
        } catch (Exception e) {
            F2Log.e("F2Function", "execute failed id: " + functionId, e);
            return null;
        }
    }

    private static native String nCreateFunction(Object thisObj);

    private static native void nBindChart(String functionId, long nativeChart);
}
