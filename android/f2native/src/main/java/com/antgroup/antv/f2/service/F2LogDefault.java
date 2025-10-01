package com.antgroup.antv.f2.service;

import android.util.Log;

public class F2LogDefault implements F2LogService {

    @Override
    public void i(String traceId, String content) {
        Log.i(TAG + "[" + threadName() + "]|" + traceId, content);
    }

    @Override
    public void w(String traceId, String content) {
        Log.w(TAG + "[" + threadName() + "]|" + traceId, content);
    }

    @Override
    public void e(String traceId, String content) {
        Log.e(TAG + "[" + threadName() + "]|" + traceId, content);
    }

    @Override
    public void e(String traceId, String msg, Throwable t) {
        Log.e(TAG + "[" + threadName() + "]|" + traceId, msg, t);
    }

    private static String threadName() {
        return Thread.currentThread().getName();
    }
}
