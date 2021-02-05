//
//  F2CallbackObj.m
//  F2
//
//  Created by XiaLuo on 2020/12/7.
//  Copyright © 2020 com.alipay.xgraph. All rights reserved.
//
#import "F2CallbackObj.h"
#import "F2Utils.h"
#import <graphics/func/Func.h>
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
        callbackObj.call = cexecute;
        void *pointObj = (__bridge void *)callbackObj;
        callbackObj->_func = new xg::func::IOSF2Function(pointObj, callbackObj.call);
        xg::func::FunctionManager::GetInstance().Add(callbackObj->_func);
        NSString *key = [NSString stringWithCString:callbackObj->_func->functionId.c_str()
                                           encoding:[NSString defaultCStringEncoding]];
        callbackObj.key = key;
    }
    return callbackObj;
}

- (void *)execute:(const char *)param {
    NSString *paramStr = [NSString stringWithUTF8String:param];
    if(paramStr && self.callback) {
        NSDictionary *resultDic = self.callback(paramStr);
        _result = nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:resultDic]) UTF8String]);
        return &_result;
    }
    return nil;
}
@end
