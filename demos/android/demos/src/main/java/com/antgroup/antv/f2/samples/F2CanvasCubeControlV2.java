package com.antgroup.antv.f2.samples;

import java.util.HashMap;
import java.util.Map;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import com.antgroup.antv.f2.F2BridgeRailing;
import com.antgroup.antv.f2.F2CanvasView;
import com.antgroup.antv.f2.F2ChartBridge;
import com.antgroup.antv.f2.F2Log;
import com.antgroup.antv.f2.IF2ChartInvokeCallback;
import com.antgroup.antv.f2.gesture.F2GestureListener;

/**
 * author by luna
 * date on 2022/5/29
 */

@TargetApi(Build.VERSION_CODES.HONEYCOMB_MR1)
public class F2CanvasCubeControlV2 extends CSWidgetControl<F2CanvasView> implements
        F2CanvasView.OnCanvasTouchListener, View.OnAttachStateChangeListener, F2CanvasView.OnCanvasGestureListener {

    private static final String TAG = "F2CanvasCubeControlV2";
    private static final String INVOKE_METHOD_RENDER = "render";

    private F2CanvasView mCanvasView;
    private static int instanceCount;

    private String mCanvasBizId;
    private String mBizName;
    // 外部传入
    private boolean mNeedDarkMode;

    private int mHeight;
    private int mWidth;
    private F2ChartBridge mF2ChartBridge;
    private F2BridgeRailing mF2BridgeRailing;
    private final HashMap<String, CSCallback> mCSCallbackMap = new HashMap<String, CSCallback>();
    private final HashMap<String, Boolean> mRenderRecorders = new HashMap<String, Boolean>();

    public F2CanvasCubeControlV2(Context context) {
        super(context);
        instanceCount++;
        F2Log.get().i(TAG, "create instanceCount: " + instanceCount);
    }

    @Override
    public F2CanvasView createWidgetView(Context context, Map<String, Object> map, View view, int width, int height) {
        reset();
        mCanvasView = new F2CanvasView(context);
        initBridge(context, width, height);
        handleMap(map);
        initDarkMode(map);
        F2Log.get().i(TAG, "createWidgetView CanvasBizId: " + mCanvasBizId);
        putRenderRecorder();

        mHeight = height;
        mWidth = width;
        mCanvasView.addOnAttachStateChangeListener(this);
        return mCanvasView;
    }

    private void initDarkMode(Map<String, Object> map) {
        if (!mNeedDarkMode) {
            F2Log.get().i(TAG, "initDarkMode needDarkMode false");
            return;
        }
        boolean supportDarkMode = F2CubeUtil.getDarkModeConfig();
        if (!supportDarkMode) {
            F2Log.get().i(TAG, "initDarkMode cs supportDarkMode false");
            return;
        }
        // 只有开关支持暗黑模式后面的链路才执行
        F2Log.get().i(TAG, "initDarkMode map: " + map);
        boolean isDarkMode = false;
        if (map != null && map.containsKey("ext")) {
            Map<String, Object> dataMap = (Map<String, Object>) map.get("ext");
            F2Log.get().i(TAG, "initDarkMode dataMap: " + dataMap);
            if (dataMap.containsKey("isTplDarkMode")) {
                isDarkMode = ((String) dataMap.get("isTplDarkMode")).equals("true");
                F2Log.get().i(TAG, "initDarkMode isDarkMode: " + isDarkMode);
            }
        }
//        setThemeTokenMap(AlipayApplication.getInstance().getApplicationContext(), isDarkMode);
    }

//    private void setThemeTokenMap(final Context context, final boolean isDarkMode) {
//        F2Log.get().i(TAG, "setThemeTokenMap isDarkMode: " + isDarkMode);
//        Map<String, String> colorMaps = AUTokenManager.getColorMaps(context);
//        updateThemeToken(colorMaps, isDarkMode);
//    }

    private void updateThemeToken(Map<String, String> replaceColorTokens, boolean isDark) {
        F2Log.get().i(TAG, "setThemeTokenMap isDark:" + isDark);
        if (mF2ChartBridge == null) {
            return;
        }
        Map<Integer, Integer> sReplaceColors = F2CubeUtil.parseReplaceMap(replaceColorTokens);
        mF2ChartBridge.updateThemeToken(sReplaceColors, isDark);
    }

    private void putRenderRecorder() {
        if (!TextUtils.isEmpty(mCanvasBizId)) {
            mRenderRecorders.put(mCanvasBizId, false);
        }
    }

    private void setRenderRecorder() {
        if (!TextUtils.isEmpty(mCanvasBizId) && mRenderRecorders.containsKey(mCanvasBizId)) {
            mRenderRecorders.put(mCanvasBizId, true);
        }
    }

    private void checkRenderRecorder() {
        if (!TextUtils.isEmpty(mCanvasBizId) && mRenderRecorders.containsKey(mCanvasBizId)) {
            Boolean renderRecorder = mRenderRecorders.get(mCanvasBizId);
            mRenderRecorders.remove(mCanvasBizId);
            if (renderRecorder == null || !renderRecorder) {
                F2CubeUtil.sendNoInvokeRenderEvent(mCanvasBizId, false);
            }
        }
    }

    private void handleMap(Map<String, Object> map) {
        if (map != null && map.containsKey("attrs")) {
            Object obj = map.get("attrs");
            if (obj instanceof HashMap) {
                try {
                    handleAttr((HashMap) obj);
                } catch (Exception e) {
                    F2Log.get().e(TAG, "handleMap e " + e.getMessage());
                }
            }
        }
    }

    @Override
    public void modifyData(Map<String, Object> map) {

    }


    private void initBridge(Context context, int width, int height) {
        if (mCanvasView == null) {
            return;
        }
        double ratio = context.getResources().getDisplayMetrics().density;
        if (ratio < 1) {
            ratio = 1;
        }
        mCanvasView.setCanvasInfo(width, height, "");
        mF2BridgeRailing = new F2BridgeRailing(mCanvasView);
        double rpxRatio = F2CubeUtil.getPx2RpxRatio(context);
        mF2ChartBridge = new F2ChartBridge(mF2BridgeRailing.getRailingHandle(),
                mCanvasView.getNativeCanvas(), width / ratio, height / ratio, ratio, 1 / (rpxRatio * ratio));
        mF2ChartBridge.setF2ChartInvokeCallback(new IF2ChartInvokeCallback() {
            @Override
            public void onResult(String result) {
                handleInvokeResult(result);
            }
        });

        String enableListConfig = getEnableListConfig();
        if (!TextUtils.isEmpty(enableListConfig)) {
            mF2ChartBridge.setEnableListConfig(enableListConfig);
        }
    }

    private String mEnableListConfig = null;

    private String getEnableListConfig() {
        if (mEnableListConfig == null) {
            mEnableListConfig = "{\"imageGuideEnable\":false,\"legendCenterEnable\":false}";
        }
        return mEnableListConfig;
    }

    private void reset() {
        mIsRendering = false;
        mCSCallbackMap.clear();
        mCurrentTouchEvent = "";
        if (mCanvasView != null) {
            mCanvasView.removeOnAttachStateChangeListener(this);
            mCanvasView.destroy();
            mCanvasView = null;
        }
        if (mF2BridgeRailing != null) {
            mF2BridgeRailing.destroy();
            mF2BridgeRailing = null;
        }
        if (mF2ChartBridge != null) {
            mF2ChartBridge.destroy();
            mF2ChartBridge = null;
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
        if ("true".equalsIgnoreCase(map.get("click"))) {
            addClickListener();
        }
        if ("true".equalsIgnoreCase(map.get("pan"))) {
            addPanListener();
        }
        if ("true".equalsIgnoreCase(map.get("pinch"))) {
            addPinchListener();
        }
        mNeedDarkMode = map.containsKey("needDarkMode") && "true".equalsIgnoreCase(map.get("needDarkMode"));
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
        mCanvasView.setOnCanvasTouchListener(this);
    }

    private void addClickListener() {
        mCanvasView.setOnCanvasGestureListener(this);
    }

    private void addPanListener() {
        mCanvasView.setOnCanvasGestureListener(this);
    }

    private void addPinchListener() {
        mCanvasView.setOnCanvasGestureListener(this);
    }

    private long mLastTouchStartTime;
    private boolean mIsLongClick;
    private F2CanvasView.TouchEvent mHoldEvent;
    private String mCurrentTouchEvent;


    private void setNeedToolTip() {
        if (mF2ChartBridge == null) {
            return;
        }
        addLongPressListener();
        mF2ChartBridge.setNeedTooltip(true);
    }

    @Override
    public void invokeMethod(final String methodName, final Object param, final CSCallback cSCallback) {
        F2Log.get().i(TAG, "invokeMethod methodName: " + methodName + " ,CanvasBizId: " + mCanvasBizId);
        if (TextUtils.isEmpty(methodName)) {
            return;
        }
        if (TextUtils.isEmpty(mCanvasBizId)) {
            mCanvasBizId = F2CubeUtil.getBizCode(mCanvasView);
        }
        if (TextUtils.isEmpty(mBizId)) {
            mBizId = TextUtils.isEmpty(mBizName) ? mCanvasBizId : mCanvasBizId + "_" + mBizName;
        }
        F2Log.get().i(TAG, "invokeMethod bizId: " + mBizId);
        if (TextUtils.equals(INVOKE_METHOD_RENDER, methodName)) {
            setRenderRecorder();
            doRender(param, cSCallback);
        } else {
            handleInvokeMethod(methodName, param, cSCallback);
        }
    }

    private Object mConfig;
    private boolean mIsRendering;
    private String mBizId;

    private void doRender(final Object param, final CSCallback cSCallback) {
        if (mIsRendering) {
            return;
        }
        try {
            if (cSCallback != null) {
                mCSCallbackMap.put(INVOKE_METHOD_RENDER, cSCallback);
            }
            if (mCanvasView == null || mF2ChartBridge == null || mF2ChartBridge.getChartBridgeHandle() == 0) {
                mIsRendering = false;
                sendRenderFailResult();
                return;
            }
            mConfig = param;
            mIsRendering = true;
            if (!mCanvasView.hasAdapter()) {
                F2Log.get().i(TAG, "doRender hasAdapter " + mCanvasBizId);
                mCanvasView.setAdapter(mAdapter);
            } else {
                F2Log.get().i(TAG, "doRender postCanvasDraw " + mCanvasBizId);
                mCanvasView.postCanvasDraw(); // setAdapter 自带postCanvasDraw
            }
        } catch (Exception e) {
            F2Log.get().e(TAG, "doRender exception " + e.getMessage());
            mIsRendering = false;
        }
    }

    private F2CanvasView.Adapter mAdapter = new F2CanvasView.Adapter() {
        @Override
        public void onCanvasDraw(F2CanvasView canvasView) {
            if (mConfig == null) {
                mIsRendering = false;
                sendRenderFailResult();
                return;
            }
            handleInvokeMethod(INVOKE_METHOD_RENDER, mConfig, mCSCallbackMap.get(INVOKE_METHOD_RENDER));
        }

        @Override
        public void onDestroy() {
            mIsRendering = false;
        }
    };

    private void handleInvokeMethod(final String methodName, final Object param, final CSCallback cSCallback) {
        if (TextUtils.isEmpty(methodName)) {
            return;
        }
        if (cSCallback != null) {
            mCSCallbackMap.put(methodName, cSCallback);
        }
        if (mF2ChartBridge == null) {
            if (TextUtils.equals(INVOKE_METHOD_RENDER, methodName)) {
                mIsRendering = false;
                sendRenderFailResult();
            }
            return;
        }
        String paramStr = JSONObject.toJSONString(param);
        F2Log.get().i(TAG, "handleInvokeMethod methodName: " + methodName + " param :" + paramStr);
        mF2ChartBridge.invokeMethod(methodName, TextUtils.isEmpty(paramStr) ? "" : paramStr);
    }

    private void handleInvokeResult(String resultStr) {
        if (TextUtils.isEmpty(resultStr)) {
            return;
        }
        try {
            JSONObject result = JSON.parseObject(resultStr);
            if (result == null) {
                return;
            }
            String methodName = result.getString("method");
            if (TextUtils.isEmpty(methodName)) {
                return;
            }
            sendInvokeEvent(result, mCSCallbackMap.get(methodName));

            if (TextUtils.equals(INVOKE_METHOD_RENDER, methodName)) {
                mIsRendering = false;
                sendRenderDetectEvent(result.getJSONObject("renderResult"), mBizId);
            }
        } catch (Exception e) {
            F2Log.get().e(TAG, "handleInvokeResult exception " + e.getMessage());
        }
    }

    // 上报事件
    private void sendRenderDetectEvent(JSONObject renderResult, String bizId) {
        if (mCanvasView != null && renderResult != null) {
            // 渲染是否成功
            boolean isRenderSuccess = renderResult.getBooleanValue("state");
            // 渲染指令的个数
            int renderCount = renderResult.getIntValue("renderCmdCount");
            // 获取渲染指令耗时
            long renderDuration = renderResult.getLongValue("renderDurationMM");
            // 上报绘制检测，包含白屏检测
            mCanvasView.setCanvasBizId(bizId);
            mCanvasView.sendRenderDetectEvent(renderDuration, isRenderSuccess, renderCount, isRenderSuccess, bizId);
        }
    }

    // 通知cube 绘制失败
    private void sendRenderFailResult() {
        F2Log.get().i(TAG, "sendRenderFailResult");
        HashMap<String, Object> map = new HashMap<>();
        JSONObject state = new JSONObject();
        state.put("state", false);
        map.put("renderResult", state);
        sendEvent(map, mCSCallbackMap.get(INVOKE_METHOD_RENDER));
    }

    private void sendTouchEvent(final HashMap<String, Object> map) {
        sendEvent(map, mCSCallbackMap.get(INVOKE_METHOD_RENDER));
    }

    private void sendInvokeEvent(JSONObject renderResult, final CSCallback csCallback) {
        if (renderResult == null) {
            return;
        }
        HashMap<String, Object> map = new HashMap<>();
        for (String key : renderResult.keySet()) {
            map.put(key, renderResult.get(key));
        }
        sendEvent(map, csCallback);
    }

    private static final Handler HANDLER = new Handler(Looper.getMainLooper());

    private void sendEvent(final HashMap<String, Object> map, final CSCallback csCallback) {
        HANDLER.post(new Runnable() {
            @Override
            public void run() {
                if (csCallback != null && map != null) {
                    csCallback.callbackKeepAlive(map, true);
                }
            }
        });
    }

    @Override
    public void onDestroy() {
        try {
            checkRenderRecorder();
            reset();
            instanceCount--;
            F2Log.get().i(TAG, "onDestroy instanceCount: " + instanceCount + " ,CanvasBizId: " + mCanvasBizId);
        } catch (Throwable t) {
            F2Log.get().e(TAG, "onDestroy throwable: " + t.getMessage());
        }
    }

    private boolean mDetached = false;

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
        if (mDetached) {
            mDetached = false;
            if (mIsLongClick) {
                mIsLongClick = false;
                mLastTouchStartTime = 0;
                if (mCanvasView != null) {
                    mCanvasView.postCanvasDraw();
                    HashMap<String, Object> map = new HashMap<>();
                    map.put("touchEvent", "touchend");
                    sendTouchEvent(map);
                }
            }
        }
    }

    @Override
    public void onViewDetachedFromWindow(View v) {
        mDetached = true;
    }

    @Override
    public void onTouch(F2CanvasView canvasView, F2CanvasView.TouchEvent event) {
        handleLongPress(event);
    }

    private void handleLongPress(F2CanvasView.TouchEvent event) {
        if (event == null || mF2ChartBridge == null || mCanvasView == null) {
            return;
        }
        if (mIsLongClick) {
            if ("touchstart".equalsIgnoreCase(event.getTouchEvent())) {
                mIsLongClick = false;
            }
        }
        if (!mIsLongClick) {
            if ("touchstart".equalsIgnoreCase(event.getTouchEvent())) {
                mHoldEvent = event;
                mLastTouchStartTime = System.currentTimeMillis();
                return;
            } else if ("touchmove".equalsIgnoreCase(event.getTouchEvent())) {
                long currentTime = System.currentTimeMillis();
                if (mLastTouchStartTime != 0) {
                    if (currentTime - mLastTouchStartTime > 200) { //300ms认为是长按
                        mIsLongClick = true;
                        mCurrentTouchEvent = event.getTouchEvent();
                        HashMap<String, Object> map = new HashMap<>();
                        map.put("touchEvent", "touchstart");
                        sendTouchEvent(map);
                        if (mHoldEvent != null) {
                            if (mF2ChartBridge.sendTouchEvent(mHoldEvent)) {
                                mCanvasView.swapBuffer();
                            }
                        }
                    } else {
                        return;
                    }
                } else {
                    mLastTouchStartTime = currentTime;
                    return;
                }
            } else if ("touchend".equalsIgnoreCase(event.getTouchEvent())) {
                F2Log.get().w(TAG, "onTouch:find touch end event out of long click");
            } else {
                return;
            }
        }
        if (!TextUtils.equals(mCurrentTouchEvent, event.getTouchEvent())) {
            mCurrentTouchEvent = event.getTouchEvent();
            HashMap<String, Object> map = new HashMap<>();
            map.put("touchEvent", mCurrentTouchEvent);
            sendTouchEvent(map);
        }
        if (mF2ChartBridge.sendTouchEvent(event)) {
            mCanvasView.swapBuffer();
        }
        if ("touchcancel".equalsIgnoreCase(mCurrentTouchEvent) || "touchend".equalsIgnoreCase(mCurrentTouchEvent)) {
            mIsLongClick = false;
            mLastTouchStartTime = 0;
        }
    }

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        if (event == null || mF2ChartBridge == null || mCanvasView == null) {
            F2Log.get().i(TAG, "onSingleTapUp  return");
            return false;
        }
        String jsonString = F2GestureListener.createSinglePointEvent(event.getX(), event.getY(), "tap");
        F2Log.get().i(TAG, "onSingleTapUp " + jsonString);
        String result = mF2ChartBridge.onTapEvent(jsonString);
        if (!TextUtils.isEmpty(result)) {
            mCanvasView.swapBuffer();
            sendTapEvent(result, mCSCallbackMap.get(INVOKE_METHOD_RENDER));
        }
        return true;
    }

    private void sendTapEvent(String result, final CSCallback csCallback) {
        F2Log.get().i(TAG, "sendTapEvent  " + result);
        if (TextUtils.isEmpty(result)) {
            return;
        }
        try {
            JSONObject jsonObject = JSON.parseObject(result);
            jsonObject.put("touchEvent", "tap");

            HashMap<String, Object> map = new HashMap<>();
            for (String key : jsonObject.keySet()) {
                map.put(key, jsonObject.get(key));
            }
            sendEvent(map, csCallback);
        } catch (Throwable t) {
            F2Log.get().e(TAG, "sendTapEvent throwable: " + t.getMessage());
        }
    }

    private String mLongPressEventType;
    private String mScrollEventType;
    private String mFlingEventType;
    private String mScaleEventType;

    @Override
    public void onLongPress(MotionEvent e, boolean isPress, String eventType) {
        if (e == null || mF2ChartBridge == null || mCanvasView == null) {
            F2Log.get().i(TAG, "onLongPress  return");
            return;
        }
        if (TextUtils.isEmpty(mLongPressEventType)) {
            String jsonString = F2GestureListener.createSinglePointEvent(e.getX(), e.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_START_TYPE);
            if (mF2ChartBridge.sendTouchEvent(jsonString)) {
                mCanvasView.swapBuffer();
            }
        }
        String singlePointEvent = F2GestureListener.createSinglePointEvent(e.getX(), e.getY(), eventType);
        if (mF2ChartBridge.sendTouchEvent(singlePointEvent)) {
            mCanvasView.swapBuffer();
        }
        mLongPressEventType = isPress ? eventType : "";
    }

    @Override
    public void onTouchChange(MotionEvent e, boolean isDown) {
        if (e == null || mF2ChartBridge == null || mCanvasView == null) {
            F2Log.get().i(TAG, "onTouch  return");
            return;
        }
        HashMap<String, Object> map = new HashMap<>();
        map.put("touchEvent", isDown ? "touchstart" : "touchend");
//        sendTouchEvent(map);
        F2Log.get().i(TAG, "onTouchChange  " + isDown);
        if (!isDown) {
            String singlePointEvent = F2GestureListener.createSinglePointEvent(e.getX(), e.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_END_TYPE);
            if (mF2ChartBridge.sendTouchEvent(singlePointEvent)) {
                mCanvasView.swapBuffer();
            }
            mScrollEventType = "";
            mFlingEventType = "";
            mLongPressEventType = "";
            mScaleEventType = "";
        }
    }

    @Override
    public boolean onScale(MotionEvent scale, boolean isScale, String eventType) {
        if (scale == null || mF2ChartBridge == null || mCanvasView == null) {
            F2Log.get().i(TAG, "onScale  return");
            return false;
        }
        if (TextUtils.isEmpty(mScaleEventType)) {
            String jsonString = F2GestureListener.createMorePointEvent(scale, F2CanvasView.OnCanvasGestureListener.TOUCH_START_TYPE);
            if (mF2ChartBridge.sendTouchEvent(jsonString)) {
                mCanvasView.swapBuffer();
            }
        }
        String jsonString = F2GestureListener.createMorePointEvent(scale, eventType);
        F2Log.get().i(TAG, "onScale  " + jsonString);
        if (mF2ChartBridge.sendTouchEvent(jsonString)) {
            mCanvasView.swapBuffer();
        }
        mScaleEventType = isScale ? eventType : "";
        return true;
    }


    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        if (e1 == null || e2 == null || mF2ChartBridge == null || mCanvasView == null) {
            F2Log.get().i(TAG, "onScroll  return");
            return false;
        }
        if (TextUtils.isEmpty(mScrollEventType)) {
            String jsonString = F2GestureListener.createSinglePointEvent(e1.getX(), e1.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_START_TYPE);
            F2Log.get().i(TAG, "onScroll start " + jsonString);
            if (mF2ChartBridge.sendTouchEvent(jsonString)) {
                mCanvasView.swapBuffer();
            }
        }
        String jsonString = F2GestureListener.createSinglePointEvent(e2.getX(), e2.getY(), F2CanvasView.OnCanvasGestureListener.TOUCH_MOVE_TYPE);
        F2Log.get().i(TAG, "onScroll " + jsonString);
        if (mF2ChartBridge.sendTouchEvent(jsonString)) {
            mCanvasView.swapBuffer();
        }
        return true;
    }
}