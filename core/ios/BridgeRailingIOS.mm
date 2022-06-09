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
#include "graphics/XChart.h"

using namespace xg;
using namespace xg::bridge;

void BridgeRailingIOS::PlayAnimation(const std::string &params) {
    NSDictionary *param = [F2Utils toJsonObject:[NSString stringWithUTF8String:params.c_str()]];
    NSTimeInterval delay = [param[@"delay"] doubleValue];
    long commandPointer = [param[@"command"] longValue];
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
        xg::func::Command *command = reinterpret_cast<xg::func::Command *>(commandPointer);
        command->run();
        Swap();
        delete command;
    });
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
