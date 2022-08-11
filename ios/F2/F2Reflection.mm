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

using namespace xg;

std::vector<std::vector<long long>> F2Reflection::CreateTimeRangeArray(NSArray *array, const Type *type) {
    __block std::vector<std::vector<long long>> rst;
    [array enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if([obj isKindOfClass:NSArray.class]) {
            __block std::vector<long long> subRst;
            [(NSArray*)obj enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj isKindOfClass:NSNumber.class]) {
                    subRst.push_back([obj longLongValue]);
                }
            }];
            rst.push_back(subRst);
        }
     }];
    return rst;
}

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
        NSCAssert(field != nullptr, @"field is nullptr");
        if (!field) {
            return;
        }
        if (field->GetType().IsString()) {
            field->Set(cfg, std::string([obj UTF8String]));
        } else if (field->GetType().IsBool()) {
            field->Set(cfg, [obj boolValue]);
        } else if(field->GetType().IsFloatingNumber()) {
            field->Set(cfg, [obj floatValue]);
        } else if(field->GetType().IsArray()) {
             id firstObj = [obj firstObject];
             if([firstObj isKindOfClass:NSString.class]) {
                 auto rst = F2Reflection::CreateStringArray(obj, type);
                 field->Set(cfg, rst);
             } else if ([firstObj isKindOfClass:NSNumber.class]) {
                 auto rst = F2Reflection::CreateNumberArray(obj, type);
                 field->Set(cfg, rst);
             } else if([firstObj isKindOfClass:NSArray.class]) {
                 auto rst = F2Reflection::CreateTimeRangeArray(obj, type);
                 field->Set(cfg, rst);
             }
             else {
                NSCAssert(NO, @"unsupported array type");
             }          
         } else if([obj isKindOfClass:NSDictionary.class]) {
             auto rst = F2Reflection::CreateStruct((NSDictionary *)obj, field->GetType().GetType());
             field->Set(cfg, rst);
         }
    }];
    return cfg;
}

util::XSourceItem F2Reflection::CreateaSourceItem(NSDictionary *obj) {
    __block util::XSourceItem item;
    [obj enumerateKeysAndObjectsUsingBlock:^(NSString  *_Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        if ([obj isKindOfClass:NSString.class]) {
            item[key.UTF8String] = Any(std::string([obj UTF8String]));
        } else if ([obj isKindOfClass:NSNumber.class]) {
            item[key.UTF8String] = Any([obj doubleValue]);
        } else if ([obj isKindOfClass:NSNumber.class]) {
            item[key.UTF8String] = Any([obj integerValue]);
        } else  if([obj isKindOfClass:NSArray.class]) {
            __block std::vector<Any> vals;
            NSArray *array = (NSArray *)obj;
            [array enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
                if ([obj isKindOfClass:NSNumber.class]) {
                    vals.push_back(Any([obj doubleValue]));
                } else {
                    NSCAssert(NO, @"unsuportd type");
                }
            }];
            item[key.UTF8String] = Any(vals);
        }
    }];
    return item;
}
