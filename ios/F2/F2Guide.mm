#import "F2Guide.h"
#import "F2Utils.h"
#import "F2Reflection.h"
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
        auto cfg = F2Reflection::CreateStruct(config, typeof(xg::guide::FlagCfg));
        auto cast = cfg.Cast<xg::guide::FlagCfg &>();
        self->_guide->FlagObject(cast);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))text {
    return ^id(NSDictionary *config) {
        auto cfg = F2Reflection::CreateStruct(config, typeof(xg::guide::TextCfg));
        auto cast = cfg.Cast<xg::guide::TextCfg &>();
        self->_guide->TextObject(cast);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))line {
    return ^id(NSDictionary *config) {
        auto cfg = F2Reflection::CreateStruct(config, typeof(xg::guide::LineCfg));
        auto cast = cfg.Cast<xg::guide::LineCfg &>();
        self->_guide->LineObject(cast);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))background {
    return ^id(NSDictionary *config) {
        auto cfg = F2Reflection::CreateStruct(config, typeof(xg::guide::BackgroundCfg));
        auto cast = cfg.Cast<xg::guide::BackgroundCfg &>();
        self->_guide->BackgroundObject(cast);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))image {
    return ^id(NSDictionary *config) {
        auto cfg = F2Reflection::CreateStruct(config, typeof(xg::guide::ImageCfg));
        auto cast = cfg.Cast<xg::guide::ImageCfg &>();
        self->_guide->ImageObject(cast);
        return self;
    };
}

- (F2Guide * (^)(NSDictionary *config))point {
    return ^id(NSDictionary *config) {
        auto cfg = F2Reflection::CreateStruct(config, typeof(xg::guide::PointCfg));
        auto cast = cfg.Cast<xg::guide::PointCfg &>();
        self->_guide->PointObject(cast);
        return self;
    };
}

@end
