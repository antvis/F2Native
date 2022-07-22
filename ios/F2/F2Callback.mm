//
//  F2CallbackObj.m
//  F2
//
//  Created by XiaLuo on 2020/12/7.
//  Copyright © 2020 com.alipay.xgraph. All rights reserved.
//
#import "F2Callback.h"
#import "F2CanvasView.h"
#import "F2Chart.h"
#import "F2Utils.h"
#import "../../core/graphics/func/Command.h"

@interface F2Callback ()
@property (nonatomic, copy) NSString *functionId;
@property (nonatomic, copy) ItemCallback callback;
@end

@implementation F2Callback

+ (instancetype)callback:(ItemCallback)block {
    F2Callback *callbackObj = [[F2Callback alloc] init];
    callbackObj.callback = block;
    return callbackObj;
}

- (instancetype)init {
    if(self = [super init]) {
        self.functionId = [[NSUUID UUID] UUIDString];
    }
    return self;
}

- (NSDictionary *)execute:(NSDictionary *)param {
    if(self.callback) {
        return self.callback(param);
    }else {
        return @{};
    }
}
@end

@interface F2RequestAnimationFrameHandle ()
@property (nonatomic, weak) F2Chart *chart;
@property (nonatomic, weak) F2CanvasView *canvasView;
@end

@implementation F2RequestAnimationFrameHandle

+ (instancetype)initWithF2Chart:(id)chart canvas:(id)canvasView {
    F2RequestAnimationFrameHandle *handle = [[F2RequestAnimationFrameHandle alloc] init];
    if(handle) {
        handle.chart = chart;
        handle.canvasView = canvasView;
    }
    return handle;
}

- (NSDictionary *)execute:(NSDictionary *)param {
    if (![param isKindOfClass:[NSDictionary class]]) {
        return @{};
    }
    
    NSTimeInterval delay =  [param[@"delay"] doubleValue];
    long commandPointer = [param[@"command"] longValue];
    if(delay <= 0) {
        delay = 16;
    }
    
    F2WeakSelf;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
        F2StrongSelf;
        [strongSelf runCommand:commandPointer];
    });
  
    return @{};
}

-(void)runCommand:(long)commandPointer {
    xg::func::Command *command = reinterpret_cast<xg::func::Command *>(commandPointer);
    if (self.canvasView && self.canvasView.canvasContext) {
        command->run();
        [self.canvasView setNeedsDisplay];
    }
    delete command;
}

@end
