//
//  F2Coordinate.h
//  F2
//
//  Created by weiqing.twq on 2021/11/13.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

//笛卡尔坐标系
/*
  |----------------------end
  |                        |
  |                        |
  |                        |
  |                        |
  start---------------------
 */

//极坐标系
    /*
    |--------------end
    |                |
    |                |
    |     center     |
    |                |
    |                |
    start--------------
 
 */

@interface F2Coordinate : NSObject

- (instancetype)initWithCoordinate:(void *)coord;

//获取极坐标系的中心点
- (CGPoint (^)(void))getCenter;

//坐标系左下角的点
- (CGPoint (^)(void))getStart;

//坐标系右上角的点
- (CGPoint (^)(void))getEnd;

//获取坐标系的宽度
- (CGFloat (^)(void))getWidth;

//获取坐标系的高度
- (CGFloat (^)(void))getHeight;

@end

NS_ASSUME_NONNULL_END
