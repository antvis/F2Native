//
//  F2CallbackObj.m
//  F2
//
//  Created by XiaLuo on 2020/12/7.
//  Copyright © 2020 com.alipay.xgraph. All rights reserved.
//
#import "F2CallbackObj.h"
#import "F2CanvasView.h"
#import "F2Chart.h"
#import "F2Utils.h"
#import <graphics/func/Command.h>
#import <graphics/func/Func.h>
#import <graphics/util/json.h>

namespace xg {
namespace func {
class IOSF2Function : public func::F2Function {
  public:
    IOSF2Function(void *_handle, interface _call) : func::F2Function(), handle_(_handle), call_(_call) {}
    nlohmann::json Execute(nlohmann::json t = nlohmann::json()) override {
        std::string param;
        if(t.is_string()) {
            param = t;
        } else {
            param = t.dump();
        }
        nlohmann::json rst = *(nlohmann::json *)call_(handle_, param.data());
        return rst;
    }
    ~IOSF2Function() override {
        handle_ = nullptr;
        call_ = nullptr;
    }

  private:
    void *handle_;
    interface call_;
};
} // namespace func
} // namespace xg

void *cexecute(void *caller, const char *parameter) { return [(__bridge id)caller execute:parameter]; }
@interface F2CallbackObj () {
    nlohmann::json _result;
    xg::func::IOSF2Function *_func;
}
@property (nonatomic, strong) NSString *key;
@property (nonatomic, copy) ItemCallback callback;
@property interface call;
@end
@implementation F2CallbackObj

- (void)dealloc {
    xg::func::FunctionManager::GetInstance().Remove(_func->functionId);
    delete _func;
    _func = nil;
}

+ (instancetype)initWithCallback:(ItemCallback)block {
    if(!block) {
        return nil;
    }
    F2CallbackObj *callbackObj = [[F2CallbackObj alloc] init];
    if(callbackObj) {
        callbackObj.callback = block;
    }
    return callbackObj;
}

- (instancetype)init {
    self = [super init];
    if(self) {
        self.call = cexecute;
        void *pointObj = (__bridge void *)self;
        self->_func = new xg::func::IOSF2Function(pointObj, self.call);
        xg::func::FunctionManager::GetInstance().Add(self->_func);
        NSString *key = [NSString stringWithCString:self->_func->functionId.c_str() encoding:[NSString defaultCStringEncoding]];
        self.key = key;
    }
    return self;
}

- (void *)execute:(const char *)param {
    NSString *paramStr = [NSString stringWithUTF8String:param];
    if(paramStr && self.callback) {
        NSDictionary *resultDic = self.callback(paramStr);
        _result = xg::json::ParseString([F2SafeJson([F2Utils toJsonString:resultDic]) UTF8String]);
        return &_result;
    }
    return nil;
}
@end

@interface RequestAnimationFrameHandle () {
    nlohmann::json _result;
}

@property (nonatomic, weak) F2Chart *chart;
@property (nonatomic, weak) F2CanvasView *canvasView;
@end

@implementation RequestAnimationFrameHandle

+ (instancetype)initWithF2Chart:(id)chart canvas:(id)canvasView {
    RequestAnimationFrameHandle *handle = [[RequestAnimationFrameHandle alloc] init];
    if(handle) {
        handle.chart = chart;
        handle.canvasView = canvasView;
    }
    return handle;
}

- (void *)execute:(const char *)param {
    NSString *paramStr = [NSString stringWithUTF8String:param];
    if(!paramStr) {
        return &_result;
    }
    NSDictionary *dic = [F2Utils toJsonObject:paramStr];
    NSTimeInterval delay =  [dic[@"delay"] doubleValue];
    long commandPointer = [dic[@"command"] longValue];
    if(delay <= 0) {
        delay = 16;
    }
    
    F2WeakSelf;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
        F2StrongSelf;
        [strongSelf runCommand:commandPointer];
    });
  
    return &_result;
}

-(void)runCommand:(long)commandPointer {
    xg::func::Command *command = reinterpret_cast<xg::func::Command *>(commandPointer);
    if (self.canvasView && self.canvasView.canvasContext) {
        command->run();
        [self.canvasView drawFrame];
    }
    delete command;
}

@end
