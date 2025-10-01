//
//  ConicPainter.m
//  AFWFinanceChart
//
//  Created by ougu on 2024/11/12.
//  Copyright © 2024 Alipay. All rights reserved.
//

#import "ConicPainter.h"

@interface Transition : NSObject

@property (nonatomic, assign) double fromLocation;
@property (nonatomic, assign) double toLocation;
@property (nonatomic, strong) UIColor *fromColor;
@property (nonatomic, strong) UIColor *toColor;

- (instancetype)initWithFromLocation:(double)fromLocation toLocation:(double)toLocation fromColor:(UIColor *)fromColor toColor:(UIColor *)toColor;
- (UIColor *)colorForPercent:(double)percent;

@end

@implementation Transition

- (instancetype)initWithFromLocation:(double)fromLocation toLocation:(double)toLocation fromColor:(UIColor *)fromColor toColor:(UIColor *)toColor {
    self = [super init];
    if (self) {
        _fromLocation = fromLocation;
        _toLocation = toLocation;
        _fromColor = fromColor;
        _toColor = toColor;
    }
    return self;
}

- (UIColor *)colorForPercent:(double)percent {
    double normalizedPercent = [self convertFromMin:_fromLocation max:_toLocation toMin:0.0 max:1.0 percent:percent];
    return [self lerpFromColor:_fromColor toColor:_toColor percent:normalizedPercent];
}

- (double)convertFromMin:(double)oldMin max:(double)oldMax toMin:(double)newMin max:(double)newMax percent:(double)value {
    double oldRange = oldMax - oldMin;
    double newRange = newMax - newMin;
    
    if (oldRange == 0.0) {
        return newMin;
    }
    
    return (((value - oldMin) * newRange) / oldRange) + newMin;
}

- (UIColor *)lerpFromColor:(UIColor *)fromColor toColor:(UIColor *)toColor percent:(double)percent {
    CGFloat fromRed, fromGreen, fromBlue, fromAlpha;
    [fromColor getRed:&fromRed green:&fromGreen blue:&fromBlue alpha:&fromAlpha];
    
    CGFloat toRed, toGreen, toBlue, toAlpha;
    [toColor getRed:&toRed green:&toGreen blue:&toBlue alpha:&toAlpha];
    
    CGFloat red = fromRed + percent * (toRed - fromRed);
    CGFloat green = fromGreen + percent * (toGreen - fromGreen);
    CGFloat blue = fromBlue + percent * (toBlue - fromBlue);
    CGFloat alpha = fromAlpha + percent * (toAlpha - fromAlpha);
    
    return [UIColor colorWithRed:red green:green blue:blue alpha:alpha];
}

@end

@interface ConicPainter()

@property (nonatomic, strong) NSArray<UIColor *> *colors;
@property (nonatomic, strong) NSArray<NSNumber *> *locations;
@property (nonatomic, assign) double startAngle;
@property (nonatomic, assign) double endAngle;
@property (nonatomic, assign) CGContextRef contextRef;

@end

@implementation ConicPainter {
    NSMutableArray *_transitions;
    double _maxAngle;
    double _maxHue;
}

- (instancetype)initWithColors:(NSArray<UIColor *> *)colors startAngle:(double)startAngle endAngle:(double)endAngle context:(CGContextRef)contextRef {
    self = [super init];
    if (self) {
        _transitions = [NSMutableArray array];
        _locations = @[];
        _colors = [colors mutableCopy];
        _startAngle = startAngle;
        _endAngle = endAngle;
        _contextRef = contextRef;
        
        _maxAngle = 2 * M_PI;
        _maxHue = 255.0;
    }
    return self;
}

- (void)drawInRect:(CGRect)rect {
    [self loadTransitions];
    
    CGPoint center = CGPointMake(CGRectGetMidX(rect), CGRectGetMidY(rect));
    double longerSide = MAX(rect.size.width, rect.size.height);
    double radius = longerSide * 2.0 / sqrt(2.0);
    double step = (M_PI / 2) / radius * 10;
    double angle = _startAngle;
    
    while (angle <= _endAngle) {
        double pointX = radius * cos(angle) + center.x;
        double pointY = radius * sin(angle) + center.y;
        CGPoint startPoint = CGPointMake(pointX, pointY);
        
        UIBezierPath *line = [UIBezierPath bezierPath];
//        CGContextMoveToPoint(self.contextRef, startPoint.x, startPoint.y);
//        CGContextAddLineToPoint(self.contextRef, center.x, center.y);
        [line moveToPoint:startPoint];
        [line addLineToPoint:center];
        
//        [[self colorForAngle:angle] setStroke];
        CGContextSetStrokeColorWithColor(self.contextRef, [self colorForAngle:angle].CGColor);
//        [line stroke];
        // 将路径添加到上下文
        CGContextAddPath(self.contextRef, line.CGPath);
            
            // 绘制路径
        CGContextStrokePath(self.contextRef);
        
        angle += step;
    }
}

- (void)loadTransitions {
    [_transitions removeAllObjects];
    NSUInteger transitionsCount = _colors.count - 1;
    
    if (transitionsCount > 0) {
        double locationStep = 1.0 / transitionsCount;
        for (NSUInteger i = 0; i < transitionsCount; i++) {
            double fromLocation, toLocation;
            UIColor *fromColor, *toColor;
            
            if (_locations.count == _colors.count) {
                fromLocation = _locations[i].doubleValue;
                toLocation = _locations[i + 1].doubleValue;
            } else {
                fromLocation = locationStep * i;
                toLocation = locationStep * (i + 1);
            }
            
            fromColor = _colors[i];
            toColor = _colors[i + 1];
            Transition *transition = [[Transition alloc] initWithFromLocation:fromLocation toLocation:toLocation fromColor:fromColor toColor:toColor];
            [_transitions addObject:transition];
        }
    }
}

- (UIColor *)colorForAngle:(double)angle {
    double percent = ((angle - _startAngle) * 1.0) / _maxAngle;
    //    double percent = [self convertFromZeroToMax:angle oldMax:MaxAngle newMax:1.0];
    Transition *transition = [self transitionForPercent:percent];
    return transition ? [transition colorForPercent:percent] : [self spectrumColorForAngle:angle];
}

- (Transition *)transitionForPercent:(double)percent {
    NSPredicate *predicate = [NSPredicate predicateWithBlock:^BOOL(Transition *transition, NSDictionary *bindings) {
        return percent >= transition.fromLocation && percent < transition.toLocation;
    }];
    
    NSArray *filtered = [_transitions filteredArrayUsingPredicate:predicate];
    
    return (filtered.count > 0) ? filtered.firstObject : (percent <= 0.5 ? _transitions.firstObject : _transitions.lastObject);
}

- (UIColor *)spectrumColorForAngle:(double)angle {
    double hue = (angle * _maxHue) / _maxAngle;
    //    double hue = [self convertFromZeroToMax:angle oldMax:MaxAngle newMax:MaxHue];
    return [UIColor colorWithHue:hue / _maxHue saturation:1.0 brightness:1.0 alpha:1.0];
}

@end
