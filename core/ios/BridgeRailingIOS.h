//
//  F2CommonIOSPlatform.hpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#if defined(__APPLE__)

#ifndef XG_BRIDGE_RAILINGIOS
#define XG_BRIDGE_RAILINGIOS

#include "../bridge/AbstractBridgeRailing.h"

namespace xg {
namespace bridge {

class BridgeRailingIOS final : public AbstractBridgeRailing {
public:
    ///传入view，在这里是UIView的指针
    BridgeRailingIOS(void *view);
    ~BridgeRailingIOS();
    
    void PlayAnimation(const std::string &params)  override;
    
    void Swap() override;
    
    long GetTimezoneOffset(const std::string &timezoneName) override;
    
    std::string FormatTime(const std::string &value, const std::string &timezoneName, const std::string &timestampFormatter) override;
    
    inline void SetCanvasContext(void *context) override { context_ = context; }
    inline void *GetCanvasContext() override { return context_; }
private:
    void *view_ = nullptr;
    void *context_ = nullptr;
    void *proxy_ = nullptr;
};
}
}

#endif /* XG_BRIDGE_RAILINGIOS */

#endif
