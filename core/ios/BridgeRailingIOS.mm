//
//  F2CommonIOSPlatform.cpp
//  F2
//
//  Created by weiqing.twq on 2022/6/1.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#include <UIKit/UIKit.h>
#include "F2Utils.h"
#include "BridgeRailingIOS.h"
#include "../graphics/XChart.h"

@interface F2RailingProxy : NSObject

@end

@implementation F2RailingProxy

-(void)PlayAnimation:(NSDictionary *)param view:(UIView *) view{
    NSTimeInterval delay = [param[@"delay"] doubleValue];
    long commandPointer = [param[@"command"] longValue];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
        if (view) {
            xg::func::Command *command = reinterpret_cast<xg::func::Command *>(commandPointer);
            command->run();
            [view setNeedsDisplay];
            delete command;
        }
    });
}

@end


using namespace xg;
using namespace xg::bridge;

BridgeRailingIOS::BridgeRailingIOS(void *view) {
    view_ = view;
    //__bridge_retained 让oc对象所有权转移到c++对象，保持oc不被释放掉
    proxy_ = (__bridge_retained void *)[[F2RailingProxy alloc] init];
}

BridgeRailingIOS::~BridgeRailingIOS() {
    view_ = nullptr;
    context_ = nullptr;
    //__bridge_transfer把c++对象所有权转移到oc对象，从而被释放oc的proxy
    F2RailingProxy *proxy = (__bridge_transfer F2RailingProxy *)proxy_;
    proxy = nullptr;
}

void BridgeRailingIOS::PlayAnimation(const std::string &params) {
    if (view_ && proxy_) {
        NSDictionary *param = [F2Utils toJsonObject:[NSString stringWithUTF8String:params.c_str()]];
        //不能在c++写gcd的代码，因为c++没有gcd的引用计数，在c++对象释放的时候gcd不会自动被释放掉，需要手动释放
        //这里使用一个oc对象帮助管理gcd
        F2RailingProxy *proxy = (__bridge F2RailingProxy *)proxy_;
        UIView *view = (__bridge UIView *)view_;
        [proxy PlayAnimation:param view:view];
    }
}

void BridgeRailingIOS::Swap() {
    if (view_) {
        UIView *view = (__bridge UIView *)view_;
        [view setNeedsDisplay];
    }
}

long BridgeRailingIOS::GetTimezoneOffset(const std::string &timezoneName) {
    NSString *timezoneNameStr = [NSString stringWithUTF8String:timezoneName.c_str()];
    NSInteger timezoneoffset = ([[NSTimeZone timeZoneWithName:timezoneNameStr] secondsFromGMT]);
    return timezoneoffset;
}

std::string BridgeRailingIOS::FormatTime(const std::string &value, const std::string &timezoneName, const std::string &timestampFormatter) {
    double timeStamp = stod(value);
    NSDateFormatter *formatter = [NSDateFormatter new];
    formatter.dateFormat = [NSString stringWithUTF8String:timestampFormatter.c_str()];
    formatter.timeZone = [NSTimeZone timeZoneWithName:[NSString stringWithUTF8String:timezoneName.c_str()]];
    NSDate *date = [NSDate dateWithTimeIntervalSince1970:timeStamp/1000.f];
    return [formatter stringFromDate:date].UTF8String;
}
