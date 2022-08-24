//
//  WasmDefine.h
//  warm_x86_64
//
//  Created by the one on 2020/6/30.
//  Copyright © 2020 alipay. All rights reserved.
//

#ifndef WasmDefine_h
#define WasmDefine_h

#define WASM_CONCAT(a, b)   a ## b
/*
 * Concatenate preprocessor tokens a and b after macro-expanding them.
 */
#define WASM_CONCAT_WRAPPER(a, b)    WASM_CONCAT(a, b)

#define WASM_EXPORT_METHOD_ASYNC_TAG        wasm_export_method_async_
#define WASM_EXPORT_METHOD_SYNC_TAG         wasm_export_method_sync_

#define WASM_EXPORT_METHOD_INTERNAL(method, token) \
+ (NSString *)WASM_CONCAT_WRAPPER(token, __LINE__) { \
    return NSStringFromSelector(method); \
}

/**
 *  @abstract export public method
 */
#define WASM_EXPORT_METHOD(method) WASM_EXPORT_METHOD_INTERNAL(method,WASM_EXPORT_METHOD_SYNC_TAG)

/**
 *  @abstract export public method, support sync return value
 *  @warning the method can only be called on js thread
 */
#define WASM_EXPORT_METHOD_ASYNC(method) WASM_EXPORT_METHOD_INTERNAL(method,WASM_EXPORT_METHOD_ASYNC_TAG)

//NOTE Wasm Monitor paramer key
#define PLATFORM_PARAM_WASM_EXECUTE_METHOD               @"PARAM_WASM_EXECUTE_METHOD"
#define PLATFORM_PARAM_WASM_EXECUTE_SIG                  @"PARAM_WASM_EXECUTE_SIG"
#define PLATFORM_PARAM_WASM_WASM_DATA_LENGTH             @"PARAM_WASM_WASM_DATA_LENGTH"
#define PLATFORM_PARAM_WASM_BIZ                          @"PARAM_WASM_BIZ"

typedef NS_ENUM(NSUInteger, WASMLogType) {
    WASMLogTypeDebug = 0,
    WASMLogTypeInfo,
    WASMLogTypeWarn,
    WASMLogTypeError
};




#endif /* WasmDefine_h */
