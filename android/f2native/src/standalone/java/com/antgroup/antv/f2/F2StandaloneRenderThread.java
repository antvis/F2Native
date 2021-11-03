package com.antgroup.antv.f2;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author qingyuan.yl
 * @date 2021/2/7
 */
public abstract class F2StandaloneRenderThread implements F2RenderThread {

    public static class Factory implements F2RenderThreadFactory {

        @Override
        public F2RenderThread createRenderThread(F2Config canvasOptions) {
            boolean asyncRender = canvasOptions.getBoolField(F2CanvasView.ConfigBuilder.KEY_ASYNC_RENDER, true);

            if (asyncRender) {
                String canvasId = canvasOptions.getStringField(F2Constants.CANVAS_ID);
                return new Worker("t-"+ canvasId).assign();
            } else {
                return new Main();
            }
        }
    }

    public static class Main extends F2StandaloneRenderThread {
        private List<WeakReference<Runnable>> mRenderTasks = new ArrayList<>();
        private Handler mHandler;
        public Main() {
            mHandler = new Handler(Looper.getMainLooper());
        }

        @Override
        public boolean isStarted() {
            return true;
        }

        @Override
        public void start() {
        }

        @Override
        public void askQuit() {
            removeAllIdleTasks();
        }

        @Override
        public boolean isOnRenderThread() {
            return mHandler.getLooper() == Looper.myLooper();
        }

        @Override
        public void forcePost(Runnable run) {
            mHandler.post(run);
            mRenderTasks.add(new WeakReference<Runnable>(run));
        }

        @Override
        public void forcePost(Runnable run, long delayMillis) {
            mHandler.postDelayed(run, delayMillis);
            mRenderTasks.add(new WeakReference<Runnable>(run));
        }

        @Override
        public void immediatePost(Runnable run) {
            if (isOnRenderThread()) {
                run.run();
            } else {
                mHandler.post(run);
                mRenderTasks.add(new WeakReference<Runnable>(run));
            }
        }

        protected void removeAllIdleTasks() {
            if (!mRenderTasks.isEmpty()) {
                for (WeakReference<Runnable> renderTask : mRenderTasks) {
                    Runnable task = null;
                    if ((task = renderTask.get()) != null) {
                        mHandler.removeCallbacks(task);
                    }
                }
                mRenderTasks.clear();
            }
        }
    }

    public static class Worker extends F2StandaloneRenderThread {

        private List<WeakReference<Runnable>> mRenderTasks;
        private HandlerThread mThread;
        private Handler mHandler;

        // assign 模式下使用变量
        private Worker mHost = null;
        private AtomicInteger mIndexCount = new AtomicInteger(0);
        public Worker(String name) {
            mThread = new HandlerThread(name);
            mRenderTasks = new ArrayList<>();
        }

        private Worker(Worker worker) {
            mHost = worker;
            mRenderTasks = new ArrayList<>();
            this.mThread = mHost.mThread;
            mHost.mIndexCount.incrementAndGet();
        }

        public Worker assign() {
            Worker assign = new Worker(this);
            return assign;
        }

        @Override
        public boolean isStarted() {
            return mThread.isAlive();
        }

        @Override
        public void start() {
            if (mHost == null) {
                throw new IllegalStateException("Please use the worker.assign() instance.");
            }
            if (!mThread.isAlive()) {
                mThread.start();
            }
            if (mHandler == null) {
                mHandler = new Handler(mThread.getLooper());
            }
        }

        @Override
        public void askQuit() {
            try {
                removeAllIdleTasks();
                AtomicInteger indexCount = mHost == null ? mIndexCount : mHost.mIndexCount;
                int count = indexCount.decrementAndGet();
                if (count == 0) {
                    mThread.quitSafely();
                }
                mHost = null;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        protected void finalize() throws Throwable {
            askQuit();
            super.finalize();
        }

        @Override
        public boolean isOnRenderThread() {
            return mThread.getLooper() == Looper.myLooper();
        }

        @Override
        public void forcePost(Runnable run) {
            if (!isStarted() || mHandler == null) return;
            mHandler.post(run);
            mRenderTasks.add(new WeakReference<Runnable>(run));
        }

        @Override
        public void forcePost(Runnable run, long delayMillis) {
            if (!isStarted() || mHandler == null) return;
            mHandler.postDelayed(run, delayMillis);
            mRenderTasks.add(new WeakReference<Runnable>(run));
        }

        @Override
        public void immediatePost(Runnable run) {
            if (!isStarted() || mHandler == null) {
                return;
            }
            if (isOnRenderThread()) {
                run.run();
            } else {
                mHandler.post(run);
                mRenderTasks.add(new WeakReference<>(run));
            }
        }

        protected void removeAllIdleTasks() {
            if (!mRenderTasks.isEmpty()) {
                for (WeakReference<Runnable> renderTask : mRenderTasks) {
                    Runnable task = renderTask != null ? renderTask.get() : null;
                    if (task != null) {
                        mHandler.removeCallbacks(task);
                    }
                }
                mRenderTasks.clear();
            }
        }
    }
}
