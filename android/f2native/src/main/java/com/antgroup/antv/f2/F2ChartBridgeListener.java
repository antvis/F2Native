package com.antgroup.antv.f2;

/**
 * cube bridge中invokeMethod调用的callback
 * author by luna
 * date on 2022/5/28
 */

public abstract class F2ChartBridgeListener {

    private String methodName;
    private String bizId;

    public F2ChartBridgeListener(String methodName, String bizId) {
        this.methodName = methodName;
        this.bizId = bizId;
    }

    public String getMethodName() {
        return methodName;
    }

    public String getBizId() {
        return bizId;
    }

    public abstract void onResult(String methodName, String bizId, String result);

}
