package com.antgroup.antv.f2;

public interface F2RenderThread {

    boolean isStarted();

    void start();

    /**
     * Only ask for quit thread, you should decide to quit the thread if it is matched conditions.
     */
    void askQuit();

    boolean isOnRenderThread();

    /**
     *  insert task into the end of the taskQueue.
     *
     * @param run task.
     */
    void forcePost(Runnable run);

    void forcePost(Runnable run, long delayMillis);

    /**
     *  if isOnRenderThread() is true, execute task immediately.
     * @param run task.
     */
    void immediatePost(Runnable run);
}
