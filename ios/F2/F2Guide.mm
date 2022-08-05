#import "F2Guide.h"
#import "F2Utils.h"
#import "../../core/graphics/guide/GuideController.h"
#import "../../core/reflection/param.h"
#import "../../core/reflection/type.h"
#import "../../core/graphics/guide/Point.h"

@interface F2Guide () {
    xg::guide::GuideController *_guide;
}

@end

@implementation F2Guide

- (instancetype)initWithGuide:(void *)guide {
    if(self = [super init]) {
        _guide = (xg::guide::GuideController *)guide;
    }
    return self;
}

- (F2Guide * (^)(NSDictionary *config))flag {
    return ^id(NSDictionary *config) {
        self->_guide->Flag([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))text {
    return ^id(NSDictionary *config) {
        self->_guide->Text([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))line {
    return ^id(NSDictionary *config) {
        self->_guide->Line([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))background {
    return ^id(NSDictionary *config) {
        self->_guide->Background([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))image {
    return ^id(NSDictionary *config) {
        self->_guide->Image([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))point {
    return ^id(NSDictionary *config) {
//        const Type *type = typeof(xg::guide::PointCfg);
//        Any cfg = type->GetConstructor()->Invoke();
//        try{
//        [config enumerateKeysAndObjectsUsingBlock:^(NSString  *_Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
//            auto field = type->GetField(key.UTF8String);
//            if ([obj isKindOfClass:NSString.class]) {
//                field->Set(cfg, std::string([obj UTF8String]));
//            } else if ([obj isKindOfClass:NSNumber.class]) {        
//                if (field->GetType().IsBool()) {
//                    field->Set(cfg, [obj boolValue]);
//                } else {
//                    field->Set(cfg, [obj floatValue]);
//                }
//            } else  if([obj isKindOfClass:NSArray.class]) {
//                NSArray *eles = (NSArray *)obj;
//                
////                __block vector<string> rst;
////                rst.reserve(eles.count);
////                [eles enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
////                    if ([obj isKindOfClass:NSString.class]) {
////                        rst.push_back([obj UTF8String]);
////                    } else {
////                        NSCAssert(NO, @"not supported");
////                    }
////                }];
//                if ([key isEqualToString:@"position"]) {
//                    vector<string> position = {"min", "min"};
////                    field->Set(cfg, &position);
//                } else if([key isEqualToString:@"margin"]) {
//                    vector<float> margin = {10, 10};                    
////                    field->Set(cfg, margin);
//                }
//               
//            }
//        }];
//        } catch (const Exception& ex){
//            std::cout << "error: " << ex.What() << "  at: " << ex.Where() << std::endl;
//        }
//        
//        
//        auto &vv = (const xg::guide::PointCfg &)(*cfg.Cast());
////        self->_guide->Point([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
//        self->_guide->PointObject(vv);
        return self;
    };
}

@end
