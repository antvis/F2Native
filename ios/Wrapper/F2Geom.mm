#import "F2Geom.h"
#import "F2Utils.h"
#import <graphics/geom/Area.h>
#import <graphics/geom/Interval.h>
#import <graphics/geom/Line.h>

@interface F2Geom () {
    xg::geom::AbstractGeom *_geom;
}
@end

@implementation F2Geom

- (instancetype)initWithGeom:(void *)geom {
    if(self = [super init]) {
        _geom = (xg::geom::AbstractGeom *)geom;
    }
    return self;
}

- (void *)getGeom {
    return _geom;
}

- (F2Geom * (^)(NSString *attr))position {
    return ^id(NSString *attr) {
        self->_geom->Position([XGSafeString(attr) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSString *attr, NSArray<NSString *> *colors))color {
    return ^id(NSString *attr, NSArray *colors) {
        std::vector<std::string> c;
        for(NSString *color in colors) {
            c.emplace_back([color UTF8String]);
        }
        self->_geom->Color([XGSafeString(attr) UTF8String], c);
        return self;
    };
}

- (F2Geom * (^)(NSString *color))fixedColor {
    return ^id(NSString *color) {
        self->_geom->Color([XGSafeString(color) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSString *field, NSArray<NSNumber *> *sizes))size {
    return ^id(NSString *field, NSArray *sizes) {
        std::vector<float> s;
        for(NSNumber *size in sizes) {
            s.emplace_back([size floatValue]);
        }
        self->_geom->Size([XGSafeString(field) UTF8String], s);
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
        self->_geom->Shape([XGSafeString(field) UTF8String], s);
        return self;
    };
}

- (F2Geom * (^)(NSString *shape))fixedShape {
    return ^id(NSString *shape) {
        self->_geom->Shape([XGSafeString(shape) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSString *adjust))adjust {
    return ^id(NSString *adjust) {
        self->_geom->Adjust([XGSafeString(adjust) UTF8String]);
        return self;
    };
}

- (F2Geom * (^)(NSDictionary *config))style {
    return ^id(NSDictionary *config) {
        self->_geom->Style([XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
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
        interval->Tag([XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

@end

@implementation F2Area
@end

@implementation F2Point
@end
