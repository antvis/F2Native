package com.antgroup.antv.f2;

import android.os.Handler;
import android.os.HandlerThread;

/**
 * author by luna
 * date on 2023/2/23
 */

public class HandlerThreadFactory {

    // 图片下载
    private static HandlerThreadWrapper mBitmapLoopThread = new HandlerThreadWrapper("create-bitmap");

    private HandlerThreadFactory() {
        throw new InstantiationError("Must not instantiate this class");
    }

    // 图片下载
    public static Handler getLoadBitmapThreadHandler() {
        return mBitmapLoopThread.getHandler();
    }

    private static class HandlerThreadWrapper {
        private Handler handler = null;

        private HandlerThreadWrapper(String threadName) {
            HandlerThread handlerThread = new HandlerThread("F2Native-" + threadName);
            handlerThread.start();
            handler = new Handler(handlerThread.getLooper());
        }

        public Handler getHandler() {
            return handler;
        }
    }
}
