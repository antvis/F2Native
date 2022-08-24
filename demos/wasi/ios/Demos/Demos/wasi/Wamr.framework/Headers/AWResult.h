//
//  AWResult.h
//  AntWasm
//
//  Created by the one on 2020/6/30.
//  Copyright © 2020 Alipay. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, AWResultCode) {
    AWResultCodeOK = 0,             //  成功
    AWResultWasmDisable = 1,        //  Wasm 降级
    AWResultWasmParamerError = 2,   //  参数异常
    AWResultCodeUnknownError,       //  未知错误
};

@interface AWResult : NSObject

@property (nonatomic, assign) AWResultCode resultCode;  //  结果码
@property (nonatomic, strong) NSString *resultDesc;     //  结果描述
@property (nonatomic, strong) id result;                //  结果数据
@property (nonatomic, strong) NSDictionary *extras;     //  扩展信息

@end

NS_ASSUME_NONNULL_END
