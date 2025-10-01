#import "F2Geom.h"
#import "F2Callback.h"
#import "F2Chart.h"
#import "F2Utils.h"
#import <graphics/geom/Area.h>
#import <graphics/geom/Interval.h>
#import <graphics/geom/Line.h>

@interface F2Geom () {
    xg::geom::AbstractGeom *_geom;
}
@property (nonatomic, weak) F2Chart *chart;
@end

@implementation F2Geom

- (instancetype)initWithGeom:(void *)geom withOwner:(id)chart {
    if(self = [super init]) {
        _geom = (xg::geom::AbstractGeom *)geom;
        _chart = chart;
    }
    return self;
}

- (void *)getGeom {
    return _geom;
}

- (F2Geom * (^)(NSString *attr))position {
    return ^id(NSString *attr) {
        self->_geom->Position([F2SafeString(attr) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSString *attr, NSArray<NSString *> *colors))color {
    return ^id(NSString *attr, NSArray *colors) {
        std::vector<std::string> c;
        for(NSString *color in colors) {
            c.emplace_back([color UTF8String]);
        }
        self->_geom->Color([F2SafeString(attr) UTF8String], c);
        return self;
    };
}

- (F2Geom * (^)(NSString *color))fixedColor {
    return ^id(NSString *color) {
        self->_geom->Color([F2SafeString(color) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSString *field, NSArray<NSNumber *> *sizes))size {
    return ^id(NSString *field, NSArray *sizes) {
        std::vector<float> s;
        for(NSNumber *size in sizes) {
            s.emplace_back([size floatValue]);
        }
        self->_geom->Size([F2SafeString(field) UTF8String], s);
        return self;
    };
}

- (F2Geom * (^)(float size))fixedSize {
    return ^id(float size) {
        self->_geom->Size(size);
        return self;
    };
}

- (F2Geom * (^)(NSString *field, NSArray<NSString *> *shapes))shape {
    return ^id(NSString *field, NSArray *shapes) {
        std::vector<std::string> s;
        for(NSString *color in shapes) {
            s.emplace_back([color UTF8String]);
        }
        self->_geom->Shape([F2SafeString(field) UTF8String], s);
        return self;
    };
}

- (F2Geom * (^)(NSString *shape))fixedShape {
    return ^id(NSString *shape) {
        self->_geom->Shape([F2SafeString(shape) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSString *adjust))adjust {
    return ^id(NSString *adjust) {
        self->_geom->Adjust([F2SafeString(adjust) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSDictionary *config))style {
    return ^id(NSDictionary *config) {
        self->_geom->Style([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self.chart]]) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSDictionary *config))attrs {
    return ^id(NSDictionary *config) {
        self->_geom->SetAttrs([F2SafeString([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

@end

@implementation F2Line
@end

@implementation F2Interval

- (F2Interval * (^)(NSDictionary *config))tag {
    return ^id(NSDictionary *config) {
        xg::geom::Interval *interval = (xg::geom::Interval *)[super getGeom];
        interval->Tag([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self.chart]]) UTF8String]);
        return self;
    };
}

@end

@implementation F2Area
@end

@implementation F2Point
@end

@implementation F2Candle
@end
