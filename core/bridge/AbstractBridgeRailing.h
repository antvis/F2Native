//
//  F2CommonPlatform.hpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#ifndef XG_BRIDGE_RAILING
#define XG_BRIDGE_RAILING

#include <string>

namespace xg {
namespace bridge {

/// bridge上的扶栏，这里主要提供平台上的工具方法
class AbstractBridgeRailing {
public:
    virtual ~AbstractBridgeRailing() = default;
    
    /// 播放动画
    /// @param params  是一个jsonstring，需要解析成平台的map对象，读取其中delay和command两个key，从而延迟执行command
    virtual void PlayAnimation(const std::string &params) = 0;
    
    /// 这是上屏，在iOS上调用了[view setNeedsDisplay]， 在android上调用invalidate
    virtual void Swap() = 0;
    
    ///传入一个时区的名字，返回一个时区相对去GMT的偏移，单位是秒
    ///@param timezoneName 时区的名字
    ///@return 时区的偏移量
    virtual long GetTimezoneOffset(const std::string &timezoneName) = 0;
    
    ///传入一个string的值和时区，按传入的格式化进行格式
    ///@param value 时间戳
    ///@param timezoneName 时区的名字
    ///@param formatter 格式化的方式 YYYY-MM
    ///@return 格式化后的时间
    virtual std::string FormatTime(const std::string &value, const std::string &timezoneName, const std::string &formatter) = 0;
    
    
    /// 设置画布的上下文， context在iOS上是CGContextRef， 在android上是java object
    /// @param context 画布的上下文
    virtual inline void SetCanvasContext(void *context) = 0;
    virtual inline void *GetCanvasContext() = 0;
};
}
}

#endif /* XG_BRIDGE_RAILING */
