//
//  F2Reflection.cpp
//  F2
//
//  Created by weiqing.twq on 8/8/22.
//

#include "F2Reflection.h"
#import "../../core/reflection/param.h"
#import "../../core/reflection/constructor.h"
#import "../../core/reflection/field.h"

std::vector<float> F2Reflection::CreateNumberArray(NSArray *array, const Type *type) {
    __block std::vector<float> rst;
    rst.reserve(array.count);
    [array enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
     rst.push_back([obj floatValue]);
     }];
    return rst;
}

std::vector<std::string> F2Reflection::CreateStringArray(NSArray *array, const Type *type) {
    __block std::vector<std::string> rst;
    rst.reserve(array.count);
    [array enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
         if ([obj isKindOfClass:NSNumber.class]) {
            rst.push_back([obj stringValue].UTF8String);
         } else if ([obj isKindOfClass:NSString.class]) {
            rst.push_back([obj UTF8String]);
         }  else {
            NSCAssert(NO, @"unsupported string type");
         }
    }];
    return rst;
}
  

Any F2Reflection::CreateStruct(NSDictionary *config, const Type *type) {
    auto cfg = type->GetConstructor()->Invoke();
    [config enumerateKeysAndObjectsUsingBlock:^(NSString  *_Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        auto field = type->GetField(key.UTF8String);
        if ([obj isKindOfClass:NSString.class]) {
            field->Set(cfg, std::string([obj UTF8String]));
        } else if ([obj isKindOfClass:NSNumber.class]) {
            if (field->GetType().IsBool()) {
                field->Set(cfg, [obj boolValue]);
            } else {
                field->Set(cfg, [obj floatValue]);
            }
        } else  if([obj isKindOfClass:NSArray.class]) {
             id firstObj = [obj firstObject];
             if([firstObj isKindOfClass:NSString.class]) {
                 auto rst = F2Reflection::CreateStringArray(obj, type);
                 field->Set(cfg, rst);
             } else if ([firstObj isKindOfClass:NSNumber.class]) {
                 auto rst = F2Reflection::CreateNumberArray(obj, type);
                 field->Set(cfg, rst);
             } else {
                NSCAssert(NO, @"unsupported array type");
             }          
         } else {
             NSCAssert(NO, @"unsupported dict type");
         }
    }];
    return cfg;
}
