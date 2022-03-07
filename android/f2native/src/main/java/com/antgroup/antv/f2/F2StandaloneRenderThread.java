package com.antgroup.antv.f2;

import android.os.Handler;
import android.os.Looper;

import com.alipay.antgraphic.thread.JavaCanvasThreadWrap;
import com.alipay.antgraphic.thread.JavaCanvasThreadWrapManager;

import java.lang.ref.WeakReference;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author qingyuan.yl
 * @date 2021/2/7
 */
public abstract class F2StandaloneRenderThread {

    public static class Factory implements F2RenderThreadFactory {
        @Override
        public F2RenderThread createRenderThread(F2Config canvasOptions) {
            boolean async = canvasOptions.getBoolField(F2CanvasView.ConfigBuilder.KEY_ASYNC_RENDER, true);
            String sessionId = canvasOptions.getStringField("session_id");
            if (async) {
                return new Worker(sessionId).assign();
            } else {
                return new Main(sessionId);
            }
        }
    }

    public static class Main extends JavaCanvasThreadWrap implements F2RenderThread {

        private Handler mMainHandler = null;
        private Method nExecuteCallback = null;
        private List<WeakReference<Runnable>> mRenderTasks;
        public Main(String sessionId) {
            mMainHandler = new Handler(Looper.myLooper()); // 关联当前线程(不一定是主线程)
            this.setSessionId(sessionId);
            mRenderTasks = new ArrayList<>();

            try {
                nExecuteCallback = JavaCanvasThreadWrap.class.getDeclaredMethod("nExecCallback", long.class);
                nExecuteCallback.setAccessible(true);
            } catch (Exception e) {
                F2Log.e("F2StandaloneRenderThread", "#Main exception" + e.toString());
            }
        }

        @Override
        public boolean isStarted() {
            return true;
        }

        @Override
        public void askQuit() {
            removeAllIdleTasks();
        }

        @Override
        public boolean isOnRenderThread() {
            return mMainHandler.getLooper() == Looper.myLooper();
        }

        @Override
        public void forcePost(Runnable run) {
            mMainHandler.post(run);
            mRenderTasks.add(new WeakReference<Runnable>(run));
        }

        @Override
        public void forcePost(Runnable run, long delayMillis) {
            mMainHandler.postDelayed(run, delayMillis);
            mRenderTasks.add(new WeakReference<Runnable>(run));
        }

        @Override
        public void postAtFront(Runnable r) {
            mMainHandler.postAtFrontOfQueue(r);
            mRenderTasks.add(new WeakReference<Runnable>(r));
        }

        @Override
        public void postDelayed(Runnable r, long delay) {
            mMainHandler.postDelayed(r, delay);
            mRenderTasks.add(new WeakReference<Runnable>(r));
        }

        @Override
        public void run(Runnable r) {
            immediatePost(r);
        }

        @Override
        public void immediatePost(Runnable run) {
            if (isOnRenderThread()) {
                run.run();
            } else {
                mMainHandler.post(run);
                mRenderTasks.add(new WeakReference<Runnable>(run));
            }
        }

        protected void removeAllIdleTasks() {
            final List<WeakReference<Runnable>> renderTasks = new ArrayList<>(mRenderTasks);
            mRenderTasks.clear();
            if (!renderTasks.isEmpty()) {
                for (WeakReference<Runnable> renderTask : renderTasks) {
                    Runnable task = null;
                    if ((task = renderTask.get()) != null) {
                        mMainHandler.removeCallbacks(task);
                    }
                }
            }
        }

        @Override
        public boolean isOnCanvasThread() {
            return isOnRenderThread();
        }

        @Override
        public void postFromNative(long callbackHandle) {
            if (isOnRenderThread() && nExecuteCallback != null) {
                try {
                    nExecuteCallback.invoke(null, callbackHandle);
                    F2Log.w(getSessionId(), "postFromNative success");
                } catch (Exception e) {
                    F2Log.w(getSessionId(), "postFromNative failed " + e.toString());
                    super.postFromNative(callbackHandle);
                }
            } else {
                super.postFromNative(callbackHandle);
            }
        }
    }

    public static class Worker extends JavaCanvasThreadWrap implements F2RenderThread {

        private List<WeakReference<Runnable>> mRenderTasks;
        private String token;
        private String sessionId;
        private JavaCanvasThreadWrap mCanvasThreadWrap;
        private boolean isStarted = false;

        // assign 模式下使用变量
        private Worker mHost = null;
        private AtomicInteger mIndexCount = new AtomicInteger(0);

        public Worker(String sessionId) {
            this.sessionId = sessionId;
            this.token = hashCode() +"_" + System.currentTimeMillis();
            mRenderTasks = new ArrayList<>();

            mCanvasThreadWrap = JavaCanvasThreadWrapManager.getInstance().attachCanvasThread(this.sessionId, this.token);
            isStarted = true;
        }

        private Worker(Worker worker) {
            this.mHost = worker;
            this.mCanvasThreadWrap = worker.mCanvasThreadWrap;
            this.token = worker.token;
            this.sessionId = worker.sessionId;
            mRenderTasks = new ArrayList<>();

            mHost.mIndexCount.incrementAndGet();
        }

        public Worker assign() {
            return new Worker(this);
        }

        @Override
        public void start() {
            if (mHost == null) {
                throw new IllegalStateException("Please use work.assign() instance.");
            }
            // mCanvasThreadWrap attach 时已完成启动，不需要再操作了。
        }

        @Override
        protected void init(String name) {
            // ignore
        }

        @Override
        public void post(Runnable r) {
            mCanvasThreadWrap.post(r);
        }

        @Override
        public void postAtFront(Runnable r) {
            mCanvasThreadWrap.postAtFront(r);
        }

        @Override
        public void postDelayed(Runnable r, long delay) {
            mCanvasThreadWrap.postDelayed(r, delay);
        }

        @Override
        public void run(Runnable r) {
            mCanvasThreadWrap.run(r);
        }

        @Override
        public void runBlocked(Runnable r) {
            mCanvasThreadWrap.runBlocked(r);
        }

        @Override
        public boolean isStarted() {
            return isStarted;
        }

        @Override
        public void askQuit() {
            removeAllIdleTasks();
            AtomicInteger indexCount = mHost == null ? mIndexCount : mHost.mIndexCount;
            int count = indexCount.decrementAndGet();
            F2Log.i(getSessionId(), "WorkerThread askQuit refCount: " + count);
            if (count == 0) {
                JavaCanvasThreadWrapManager.getInstance().detachCanvasThread(this.sessionId, this.token);
            }
            mHost = null;
        }

        @Override
        public void quit() {
            if (mCanvasThreadWrap != null) {
                mCanvasThreadWrap.quit();
            }
        }

        @Override
        public String getName() {
            return mCanvasThreadWrap.getName();
        }

        @Override
        public void postFromNative(long callbackHandle) {
            mCanvasThreadWrap.postFromNative(callbackHandle);
        }

        @Override
        public boolean isOnRenderThread() {
            return isOnCanvasThread();
        }

        @Override
        public void forcePost(Runnable run) {
            if (mCanvasThreadWrap == null) return;
            mCanvasThreadWrap.post(run);
            synchronized (this) {
                mRenderTasks.add(new WeakReference<>(run));
            }
        }

        @Override
        public void forcePost(Runnable run, long delayMillis) {
            if (mCanvasThreadWrap == null) return;
            mCanvasThreadWrap.postDelayed(run, delayMillis);
            synchronized (this) {
                mRenderTasks.add(new WeakReference<>(run));
            }
        }

        @Override
        public void immediatePost(Runnable run) {
            if (mCanvasThreadWrap == null)return;
            if (isOnRenderThread()) {
                run.run();
            } else {
                mCanvasThreadWrap.post(run);
                synchronized (this) {
                    mRenderTasks.add(new WeakReference<>(run));
                }
            }
        }

        @Override
        public long getShipNativeHandle() {
            if (mCanvasThreadWrap == null) return 0;
            return mCanvasThreadWrap.getShipNativeHandle();
        }

        @Override
        public boolean isOnCanvasThread() {
            if (mCanvasThreadWrap == null) return false;
            return mCanvasThreadWrap.isOnCanvasThread();
        }

        @Override
        public void removeCallback(Runnable r) {
            if (mCanvasThreadWrap != null) {
                mCanvasThreadWrap.removeCallback(r);
            }
        }

        @Override
        public boolean isOnCanvasThreadFromNative() {
            if (mCanvasThreadWrap == null) return false;
            return mCanvasThreadWrap.isOnCanvasThreadFromNative();
        }

        protected void removeAllIdleTasks() {
            List<WeakReference<Runnable>> renderTasks;
            synchronized (this) {
                renderTasks = new ArrayList<>(mRenderTasks);
                mRenderTasks.clear();
            }
            if (!renderTasks.isEmpty() && mCanvasThreadWrap != null) {
                for (WeakReference<Runnable> renderTask : renderTasks) {
                    Runnable task = renderTask != null ? renderTask.get() : null;
                    if (task != null) {
                        mCanvasThreadWrap.removeCallback(task);
                    }
                }
            }
        }
    }
}
