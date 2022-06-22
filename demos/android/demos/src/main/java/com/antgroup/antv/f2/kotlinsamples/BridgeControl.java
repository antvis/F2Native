package com.antgroup.antv.f2.kotlinsamples;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.text.TextUtils;
import android.view.View;
import android.view.ViewGroup;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2BridgeRailing;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2ChartBridge;
import com.antgroup.antv.f2.F2ChartBridgeListener;
import com.antgroup.antv.f2.F2Log;

/**
 * author by luna
 * date on 2022/5/29
 */

public class BridgeControl implements View.OnAttachStateChangeListener {
    private static final String TAG = "BridgeControl";
    private F2CanvasView mCanvasView;
    private static int instanceCount;

    private String mCanvasBizId = "test";
    private String mBizName;

    private int mHeight;
    private int mWidth;
    private F2ChartBridge mF2ChartBridge;
    private F2BridgeRailing mF2BridgeRailing;
    private double mRatio;


    public BridgeControl() {
        instanceCount++;
        F2Log.e(TAG, "create instanceCount: " + instanceCount);
    }

    public F2CanvasView createWidgetView(Context context, Map<String, Object> map, int width, int height) {
        reset();

        mCanvasView = new F2CanvasView(context);
        initBridge(context, width, height);
        handleMap(map);

        mHeight = height;
        mWidth = width;
        mCanvasView.addOnAttachStateChangeListener(this);
        return mCanvasView;
    }

    private void handleMap(Map<String, Object> map) {
        if (map != null && map.containsKey("attrs")) {
            Object obj = map.get("attrs");
            if (obj instanceof HashMap) {
                try {
                    handleAttr((HashMap) obj);
                } catch (Exception e) {
                    F2Log.e(TAG, "handleMap e " + e.toString());
                }
            }
        }
    }


    private void initBridge(Context context, int width, int height) {
        if (mCanvasView == null) {
            return;
        }
        mRatio = context.getResources().getDisplayMetrics().density;
        if (mRatio == 0) {
            mRatio = 1;
        }
        mCanvasView.init(width, height, null);
        mF2BridgeRailing = new F2BridgeRailing(mCanvasView);
        double rpxRatio = 1;
        mF2ChartBridge = new F2ChartBridge(mF2BridgeRailing.getRailingHandle(),
                mCanvasView.getNativeCanvas(), width / mRatio, height / mRatio, mRatio, 1 / (rpxRatio * mRatio));
    }

    private void reset() {
        if (mCanvasView != null) {
            mCanvasView.removeOnAttachStateChangeListener(this);
            mCanvasView.destroy();
            mCanvasView = null;
        }
        if (mF2ChartBridge != null) {
            mF2ChartBridge.destroy();
            mF2ChartBridge = null;
        }
        if (mF2BridgeRailing != null) {
            mF2BridgeRailing.destroy();
            mF2BridgeRailing = null;
        }
    }

    private void handleAttr(HashMap<String, String> map) {
        if (map == null) {
            return;
        }
        if ("true".equalsIgnoreCase(map.get("needToolTip"))) {
            setNeedToolTip();
        }
        if ("true".equalsIgnoreCase(map.get("longPress"))) {
            addLongPressListener();
        }
        String bizId = map.get("canvasBizId");
        if (!TextUtils.isEmpty(bizId)) {
            mCanvasBizId = bizId;
        }
        String bizName = map.get("bizName");
        if (!TextUtils.isEmpty(bizName)) {
            mBizName = bizName;
        }
    }

    private void addLongPressListener() {
        mCanvasView.setOnCanvasTouchListener(new F2CanvasView.OnCanvasTouchListener() {
            @Override
            public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
                if (event == null || mF2ChartBridge == null) {
                    return;
                }
                if (handleLongPress(event)) {
                    mCanvasView.swapBuffer();
                }
            }
        });
    }


    private long lastTouchStartTime;
    private boolean isLongClick;
    private F2CanvasView.TouchEvent holdEvent;
    private String currentTouchEvent;

    private boolean handleLongPress(F2CanvasView.TouchEvent event) {
        if (event == null) {
            return false;
        }
        if (isLongClick) {
            if ("touchstart".equalsIgnoreCase(event.getTouchEvent())) {
                isLongClick = false;
            }
        }
        if (!isLongClick) {
            if ("touchstart".equalsIgnoreCase(event.getTouchEvent())) {
                holdEvent = event;
                lastTouchStartTime = System.currentTimeMillis();
                return false;
            } else if ("touchmove".equalsIgnoreCase(event.getTouchEvent())) {
                long currentTime = System.currentTimeMillis();
                if (lastTouchStartTime != 0) {
                    if (currentTime - lastTouchStartTime > 300) { //300ms认为是长按
                        isLongClick = true;
                        currentTouchEvent = event.getTouchEvent();
                        if (mF2ChartBridge != null && holdEvent != null) {
                            mF2ChartBridge.sendTouchEvent(holdEvent);
                        }
                    } else {
                        return false;
                    }
                } else {
                    lastTouchStartTime = currentTime;
                    return false;
                }
            } else if ("touchend".equalsIgnoreCase(event.getTouchEvent())) {
            } else {
                return false;
            }
        }
        if (!TextUtils.equals(currentTouchEvent, event.getTouchEvent())) {
            currentTouchEvent = event.getTouchEvent();
        }
        if ("touchcancel".equalsIgnoreCase(currentTouchEvent) || "touchend".equalsIgnoreCase(currentTouchEvent)) {
            isLongClick = false;
            lastTouchStartTime = 0;
        }
        return mF2ChartBridge.sendTouchEvent(event);
    }


    private void setNeedToolTip() {
        if (mF2ChartBridge == null) {
            return;
        }
        addLongPressListener();
        mF2ChartBridge.setNeedTooltip(true);
    }

    public void invokeMethod(final String methodName, final Object param) {
        F2Log.i(TAG, "invokeMethod methodName: " + methodName + " param :" + JSONObject.toJSONString(param));
        if (!(param instanceof JSONObject) || mF2ChartBridge == null) {
            return;
        }
//        if (TextUtils.isEmpty(mCanvasBizId)) {
//            mCanvasBizId = CubeUtil.getBizCode(mCanvasView);
//        }
        String bizId = TextUtils.isEmpty(mBizName) ? mCanvasBizId : mCanvasBizId + "_" + mBizName;
//        mCSCallback = cSCallback;
        mF2ChartBridge.invokeMethod(
                methodName,
                JSONObject.toJSONString(param),
                new F2ChartBridgeListener(methodName, bizId) {

                    @Override
                    public void onResult(String methodNameStr, String bizIdStr, String result) {
                        F2Log.i(TAG, "invokeMethod onResult methodName: " + methodNameStr + ",bizIdStr: " + bizIdStr + ",result:" + result);
                        handleResult(methodNameStr, bizIdStr, result);
                    }
                }
        );
    }

    private void handleResult(String methodName, String bizId, String resultStr) {
        if (TextUtils.isEmpty(resultStr)) {
            sendRenderResult(null);
            return;
        }
        JSONObject result = JSON.parseObject(resultStr);
        sendRenderResult(result);
        if (mCanvasView != null && TextUtils.equals(methodName, "render") && result != null) {
            JSONObject renderResult = result.getJSONObject("renderResult");
            if (renderResult != null) {
                // 渲染是否成功
                boolean isRenderSuccess = renderResult.getBooleanValue("state");
                // 渲染指令的个数
                int renderCount = renderResult.getIntValue("renderCmdCount");
                // 获取渲染指令耗时
                long renderDuration = renderResult.getLongValue("renderDurationMM");
                // 上报绘制检测，包含白屏检测
                mCanvasView.initCanvasContext(new F2CanvasView.ConfigBuilder().canvasBizId(bizId).build());
                mCanvasView.sendRenderDetectEvent(renderDuration, isRenderSuccess, renderCount, isRenderSuccess, bizId);
            }
        }
    }

    private void sendRenderResult(JSONObject renderResult) {
        HashMap<String, Object> map = new HashMap<>();
        for (String key : renderResult.keySet()) {
            map.put(key, renderResult.get(key));
        }
        sendEvent(map);
    }

    private void sendEvent(final HashMap<String, Object> map) {
//        HandlerUtils.runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                if (mCSCallback != null && map != null) {
//                    mCSCallback.callbackKeepAlive(map, true);
//                }
//            }
//        });
    }

    public void onDestroy() {
        try {
            reset();
            instanceCount--;
            F2Log.e(TAG, "onDestroy instanceCount: " + instanceCount);
        } catch (Throwable t) {
            F2Log.e(TAG, "onDestroy throwable: " + t.getMessage());
        }
    }


    @Override
    public void onViewAttachedToWindow(View v) {
        if (mCanvasView != null && mHeight != 0 && mWidth != 0) {
            ViewGroup.LayoutParams layoutParams = mCanvasView.getLayoutParams();
            // 如果长宽不等的时候 强制设置长宽
            if (layoutParams != null && (layoutParams.height != mHeight || layoutParams.width != mWidth)) {
                layoutParams.height = mHeight;
                layoutParams.width = mWidth;
                mCanvasView.setLayoutParams(layoutParams);
            }
        }
    }

    @Override
    public void onViewDetachedFromWindow(View v) {
    }
}
