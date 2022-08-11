////
////  XCubeBridgeWidget.h
////  AFWFinanceChart
////
////  Created by weiqing.twq on 2022/6/1.
////  Copyright © 2022 Alipay. All rights reserved.
////
//
//#ifndef XG_BRIDGE_BRIGE
//#define XG_BRIDGE_BRIGE
//
//#include <string>
//#include "AbstractBridgeRailing.h"
//#include "../graphics/XChart.h"
//
//using namespace xg;
//
//typedef std::function<const std::string (const std::string &, const std::string &)> ChannelCallback;
//typedef std::function<void(const std::string &)> BridgeCallback;
//
//
//namespace xg {
//namespace bridge {
//
/////bridge的回调方法
//class BridgeChannel final: public func::F2Function {
//  public:
//    BridgeChannel(): func::F2Function() {}
//
//    static BridgeChannel Callback(ChannelCallback callback) {
//        BridgeChannel function;
//        function.callback_ = callback;
//        return function;
//    }
//
//    const std::string Execute(const std::string &functionId, const std::string &param) override {
//        return callback_(functionId,  param);
//    }
//
//  public:
//    ChannelCallback callback_;
//};
//
//
//
///// bridge的入口类
//class ChartBridge final {
//public:
//    /// 初始化bridge
//    /// @param width view的宽
//    /// @param height view的高
//    /// @param ratio view（屏幕的密度）
//    ChartBridge(double width, double height, double ratio);
//
//    /// 设置bridge中像素的转换，如bridge中getPostion返回的是像素，可通过除以pixelRatio转换成rpx
//    /// @param ratio bridge业务侧使用的单位
//    inline void SetPixelRatio(double ratio) { pixelRatio_ = ratio;}
//
//    /// 设置bridge的扶栏，这里是设置平台的工具类
//    /// @param railing 工具类
//    inline void SetRailing(AbstractBridgeRailing *railing) { railing_ = railing; }
//    inline AbstractBridgeRailing *GetRailing() { return railing_; }
//
//    /// bridge的主要入口方法
//    /// @param methodName 方法的名字
//    /// @param params 是一个json格式的string
//    /// @param bridgeCallback 是一个异步的回调，会回调bridge运行的正确和错误消息
//    bool InvokeMethod(const std::string &methodName,
//                      const std::string &params,
//                      BridgeCallback bridgeCallback);
//
//    /// 发送手势时间
//    /// @param json 是一个json的string
//    inline bool OnTouchEvent(const std::string &json) {
//       return chart_->OnTouchEvent(json);
//    }
//
//
//    /// 兼容代码，设置是否需要手势
//    /// @param tooltip 是否需要手势
//    inline void SetNeedTooltip(bool tooltip) { needToolTip_ = tooltip; }
//private:
//    bool InvokeRender(const nlohmann::json &dsl, const nlohmann::json &itemPosition, BridgeCallback bridgeCallback);
//    void CallbackSuccess(BridgeCallback callback, const nlohmann::json &itemPosition,long renderCmdCount, long renderDurationMM);
//    void CallbackError(BridgeCallback callback, const std::string &msg);
//    void CompatToolTip(BridgeCallback callback);
//    void CompatScales(const nlohmann::json &dsl);
//    void CompatScale(const std::string &field, const nlohmann::json &scaleConfig);
//    std::string FormatPrice(const std::string &content, int precision);
//    std::string FormatPercent(const std::string &content, double divisor, int precision);
//    std::string FormatTime(const std::string &value, const std::string &timezoneName, const std::string &formatter);
//    nlohmann::json GetPosition(const XSourceItem &itemData) ;
//
//    private:
//    AbstractBridgeRailing *railing_ = nullptr;
//    std::unique_ptr<XChart> chart_ = nullptr;
//    BridgeChannel commonFunction_;
//    std::map<std::string, BridgeChannel> callbackFunctions_;
//    std::string requestFrameFuncId_ = xg::func::MakeFunctionId();
//    double pixelRatio_ = 1;
//    bool needToolTip_ = false;
//};
//
//}
//}
//
//#endif /* XG_BRIDGE_BRIGE */
