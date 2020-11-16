package com.antgroup.antv.f2;

import android.util.Log;

/**
 * @author qingyuan.yl
 * @date 2020-09-18
 */
public final class F2Log {

    private static final String TAG = "F2Native";

    public static void i(String traceId, String content) {
        Log.i(TAG + "[" + threadName() + "]|" + traceId, content);
    }

    public static void w(String traceId, String content) {
        Log.w(TAG + "[" + threadName() + "]|" + traceId, content);
    }

    public static void e(String traceId, String content) {
        Log.e(TAG + "[" + threadName() + "]|" + traceId, content);
    }

    public static void e(String traceId, String msg, Throwable t) {
        Log.e(TAG + "[" + threadName() + "]|" + traceId, msg, t);
    }

    private static String threadName() {
        return Thread.currentThread().getName();
    }
}
