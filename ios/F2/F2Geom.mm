#import "F2Geom.h"
#import "F2Callback.h"
#import "F2Chart.h"
#import "F2Utils.h"
#import "F2Reflection.h"
#import "../../core/graphics/geom/Area.h"
#import "../../core/graphics/geom/Interval.h"
#import "../../core/graphics/geom/Line.h"

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

- (F2Geom * (^)(NSDictionary *color))linearGradientColor {
    return ^id(NSDictionary *color) {
        xg::canvas::CanvasLinearGradient gradient;
        NSArray *colorStops = color[@"colorStops"];
        NSArray *position = color[@"position"];
        
        if(![colorStops isKindOfClass:NSArray.class] ||
           ![position isKindOfClass:NSArray.class]) {
            return self;
        }
        for(NSUInteger i = 0; i < colorStops.count; ++i) {
            NSDictionary *stop = colorStops[i];
            gradient.addColorStop([stop[@"offset"] floatValue], [stop[@"color"] UTF8String]);
        }

       
        if(position.count >= 4) {
            gradient.start = {[position[0] floatValue], [position[1] floatValue]};
            gradient.end = {[position[2] floatValue], [position[3] floatValue]};
        }
    
        self->_geom->Color(gradient);
        return self;
    };
}

- (F2Geom * (^)(NSDictionary *color))radialGradientColor {
    return ^id(NSDictionary *color) {
        xg::canvas::CanvasRadialGradient gradient;
        NSArray *colorStops = color[@"colorStops"];
        NSArray *position = color[@"position"];
        
        if(![colorStops isKindOfClass:NSArray.class] ||
           ![position isKindOfClass:NSArray.class]) {
            return self;
        }
        for(NSUInteger i = 0; i < colorStops.count; ++i) {
            NSDictionary *stop = colorStops[i];
            gradient.addColorStop([stop[@"offset"] floatValue], [stop[@"color"] UTF8String]);
        }

        if(position.count >= 6) {
            gradient.start = {[position[0] floatValue], [position[1] floatValue], [position[2] floatValue]};
            gradient.end = {[position[3] floatValue], [position[4] floatValue], [position[5] floatValue]};
        }
    
        self->_geom->Color(gradient);
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
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self.chart];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(xg::geom::StyleCfg));
        auto cast = cfg.Cast<xg::geom::StyleCfg &>();
        self->_geom->StyleObject(cast);
        return self;
    };
}

- (F2Geom * (^)(CGFloat opacity))fixedOpacity {
    return ^id(CGFloat opacity) {
        self->_geom->Opacity(opacity);
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
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self.chart];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(xg::util::TagCfg));
        auto cast = cfg.Cast<xg::util::TagCfg &>();
        interval->Tag(cast);
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
