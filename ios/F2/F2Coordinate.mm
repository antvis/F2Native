//
//  F2Coordinate.m
//  F2
//
//  Created by weiqing.twq on 2021/11/13.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "F2Coordinate.h"
#import "F2Utils.h"
#import "../../core/graphics/canvas/Coord.h"

@interface F2Coordinate () {
    xg::canvas::coord::AbstractCoord *_coord;
}

@end

@implementation F2Coordinate

- (instancetype)initWithCoordinate:(void *)coord {
    if(self = [super init]) {
        _coord = (xg::canvas::coord::AbstractCoord *)coord;
    }
    return self;
}

- (CGPoint (^)(void))getCenter {
    return ^CGPoint(void) {
        return self->_coord ? CGPointMake(self->_coord->GetCenter().x / F2NativeScale, self->_coord->GetCenter().y / F2NativeScale) : CGPointZero;
    };
}

- (CGPoint (^)(void))getStart {
    return ^CGPoint(void) {
        return self->_coord ? CGPointMake(self->_coord->GetStart().x / F2NativeScale, self->_coord->GetStart().y / F2NativeScale) : CGPointZero;
    };
}

- (CGPoint (^)(void))getEnd {
    return ^CGPoint(void) {
        return self->_coord ? CGPointMake(self->_coord->GetEnd().x / F2NativeScale, self->_coord->GetEnd().y / F2NativeScale) : CGPointZero;
    };
}

- (CGFloat (^)(void))getWidth {
    return ^CGFloat(void) {
        return self->_coord ? self->_coord->GetWidth() / F2NativeScale : 0;
    };
}

- (CGFloat (^)(void))getHeight {
    return ^CGFloat(void) {
        return self->_coord ? self->_coord->GetHeight() / F2NativeScale : 0;
    };
}

@end
