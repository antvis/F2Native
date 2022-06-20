package com.antgroup.antv.f2.samples

import android.content.Context
import android.text.TextUtils
import android.view.View
import android.view.View.OnAttachStateChangeListener
import com.alibaba.fastjson.JSON
import com.alibaba.fastjson.JSONObject
import com.antgroup.antv.f2.*
import com.antgroup.antv.f2.F2CanvasView.ConfigBuilder
import com.antgroup.antv.f2.F2CanvasView.OnCanvasTouchListener
import java.util.*

/**
 * author by luna
 * date on 2022/5/29
 */
class BridgeControl : OnAttachStateChangeListener {
    private var mCanvasView: F2CanvasView? = null
    private var mCanvasBizId: String? = "test"
    private var mBizName: String? = null
    private var mHeight = 0
    private var mWidth = 0
    private var mF2ChartBridge: F2ChartBridge? = null
    private var mF2BridgeRailing: F2BridgeRailing? = null
    private var mRatio = 0.0
    fun createWidgetView(
        context: Context,
        map: Map<String, Any>?,
        width: Int,
        height: Int
    ): F2CanvasView {
        reset()
        mCanvasView = F2CanvasView(context)
        initBridge(context, width, height)
        handleMap(map)
        mHeight = height
        mWidth = width
        mCanvasView!!.addOnAttachStateChangeListener(this)
        return mCanvasView
    }

    private fun handleMap(map: Map<String, Any>?) {
        if (map != null && map.containsKey("attrs")) {
            val obj = map["attrs"]
            if (obj is HashMap<*, *>) {
                try {
                    handleAttr(obj as HashMap<*, *>?)
                } catch (e: Exception) {
                    F2Log.e(TAG, "handleMap e $e")
                }
            }
        }
    }

    private fun initBridge(context: Context, width: Int, height: Int) {
        if (mCanvasView == null) {
            return
        }
        mRatio = context.resources.displayMetrics.density.toDouble()
        if (mRatio == 0.0) {
            mRatio = 1.0
        }
        mCanvasView!!.init(width, height, null)
        mF2BridgeRailing = F2BridgeRailing(mCanvasView)
        val rpxRatio = 1.0
        mF2ChartBridge = F2ChartBridge(
            mF2BridgeRailing!!.railingHandle,
            mCanvasView!!.nativeCanvas,
            width / mRatio,
            height / mRatio,
            mRatio,
            1 / (rpxRatio * mRatio)
        )
    }

    private fun reset() {
        if (mCanvasView != null) {
            mCanvasView!!.removeOnAttachStateChangeListener(this)
            mCanvasView!!.destroy()
            mCanvasView = null
        }
        if (mF2ChartBridge != null) {
            mF2ChartBridge!!.destroy()
            mF2ChartBridge = null
        }
        if (mF2BridgeRailing != null) {
            mF2BridgeRailing!!.destroy()
            mF2BridgeRailing = null
        }
    }

    private fun handleAttr(map: HashMap<String, String>?) {
        if (map == null) {
            return
        }
        if ("true".equals(map["needToolTip"], ignoreCase = true)) {
            setNeedToolTip()
        }
        if ("true".equals(map["longPress"], ignoreCase = true)) {
            addLongPressListener()
        }
        val bizId = map["canvasBizId"]
        if (!TextUtils.isEmpty(bizId)) {
            mCanvasBizId = bizId
        }
        val bizName = map["bizName"]
        if (!TextUtils.isEmpty(bizName)) {
            mBizName = bizName
        }
    }

    private fun addLongPressListener() {
        mCanvasView!!.setOnCanvasTouchListener(OnCanvasTouchListener { canvasView, event ->
            if (event == null || mF2ChartBridge == null) {
                return@OnCanvasTouchListener
            }
            if (handleLongPress(event)) {
                mCanvasView!!.swapBuffer()
            }
        })
    }

    private var lastTouchStartTime: Long = 0
    private var isLongClick = false
    private var holdEvent: F2CanvasView.TouchEvent? = null
    private var currentTouchEvent: String? = null
    private fun handleLongPress(event: F2CanvasView.TouchEvent?): Boolean {
        if (event == null) {
            return false
        }
        if (isLongClick) {
            if ("touchstart".equals(event.touchEvent, ignoreCase = true)) {
                isLongClick = false
            }
        }
        if (!isLongClick) {
            if ("touchstart".equals(event.touchEvent, ignoreCase = true)) {
                holdEvent = event
                lastTouchStartTime = System.currentTimeMillis()
                return false
            } else if ("touchmove".equals(event.touchEvent, ignoreCase = true)) {
                val currentTime = System.currentTimeMillis()
                if (lastTouchStartTime != 0L) {
                    if (currentTime - lastTouchStartTime > 300) { //300ms认为是长按
                        isLongClick = true
                        currentTouchEvent = event.touchEvent
                        if (mF2ChartBridge != null && holdEvent != null) {
                            mF2ChartBridge!!.sendTouchEvent(holdEvent)
                        }
                    } else {
                        return false
                    }
                } else {
                    lastTouchStartTime = currentTime
                    return false
                }
            } else if ("touchend".equals(event.touchEvent, ignoreCase = true)) {
            } else {
                return false
            }
        }
        if (!TextUtils.equals(currentTouchEvent, event.touchEvent)) {
            currentTouchEvent = event.touchEvent
        }
        if ("touchcancel".equals(currentTouchEvent, ignoreCase = true) || "touchend".equals(
                currentTouchEvent,
                ignoreCase = true
            )
        ) {
            isLongClick = false
            lastTouchStartTime = 0
        }
        return mF2ChartBridge!!.sendTouchEvent(event)
    }

    private fun setNeedToolTip() {
        if (mF2ChartBridge == null) {
            return
        }
        addLongPressListener()
        mF2ChartBridge!!.setNeedTooltip(true)
    }

    fun invokeMethod(methodName: String, param: Any?) {
        F2Log.i(
            TAG,
            "invokeMethod methodName: " + methodName + " param :" + JSONObject.toJSONString(param)
        )
        if (param !is JSONObject || mF2ChartBridge == null) {
            return
        }
        //        if (TextUtils.isEmpty(mCanvasBizId)) {
//            mCanvasBizId = CubeUtil.getBizCode(mCanvasView);
//        }
        val bizId = if (TextUtils.isEmpty(mBizName)) mCanvasBizId else mCanvasBizId + "_" + mBizName
        //        mCSCallback = cSCallback;
        mF2ChartBridge!!.invokeMethod(
            methodName,
            JSONObject.toJSONString(param),
            object : F2ChartBridgeListener(methodName, bizId) {
                override fun onResult(methodNameStr: String, bizIdStr: String, result: String) {
                    F2Log.i(
                        TAG,
                        "invokeMethod onResult methodName: $methodNameStr,bizIdStr: $bizIdStr,result:$result"
                    )
                    handleResult(methodNameStr, bizIdStr, result)
                }
            }
        )
    }

    private fun handleResult(methodName: String, bizId: String, resultStr: String) {
        if (TextUtils.isEmpty(resultStr)) {
            sendRenderResult(null)
            return
        }
        val result = JSON.parseObject(resultStr)
        sendRenderResult(result)
        if (mCanvasView != null && TextUtils.equals(methodName, "render") && result != null) {
            val renderResult = result.getJSONObject("renderResult")
            if (renderResult != null) {
                // 渲染是否成功
                val isRenderSuccess = renderResult.getBooleanValue("state")
                // 渲染指令的个数
                val renderCount = renderResult.getIntValue("renderCmdCount")
                // 获取渲染指令耗时
                val renderDuration = renderResult.getLongValue("renderDurationMM")
                // 上报绘制检测，包含白屏检测
                mCanvasView!!.initCanvasContext(ConfigBuilder().canvasBizId(bizId).build())
                mCanvasView!!.sendRenderDetectEvent(
                    renderDuration,
                    isRenderSuccess,
                    renderCount,
                    isRenderSuccess,
                    bizId
                )
            }
        }
    }

    private fun sendRenderResult(renderResult: JSONObject?) {
        val map = HashMap<String, Any?>()
        for (key in renderResult!!.keys) {
            map[key] = renderResult[key]
        }
        sendEvent(map)
    }

    private fun sendEvent(map: HashMap<String, Any?>) {
//        HandlerUtils.runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                if (mCSCallback != null && map != null) {
//                    mCSCallback.callbackKeepAlive(map, true);
//                }
//            }
//        });
    }

    fun onDestroy() {
        try {
            reset()
            instanceCount--
            F2Log.e(TAG, "onDestroy instanceCount: " + instanceCount)
        } catch (t: Throwable) {
            F2Log.e(TAG, "onDestroy throwable: " + t.message)
        }
    }

    override fun onViewAttachedToWindow(v: View) {
        if (mCanvasView != null && mHeight != 0 && mWidth != 0) {
            val layoutParams = mCanvasView!!.layoutParams
            // 如果长宽不等的时候 强制设置长宽
            if (layoutParams != null && (layoutParams.height != mHeight || layoutParams.width != mWidth)) {
                layoutParams.height = mHeight
                layoutParams.width = mWidth
                mCanvasView!!.layoutParams = layoutParams
            }
        }
    }

    override fun onViewDetachedFromWindow(v: View) {}

    companion object {
        private const val TAG = "BridgeControl"
        private var instanceCount = 0
    }

    init {
        instanceCount++
        F2Log.e(TAG, "create instanceCount: " + instanceCount)
    }
}