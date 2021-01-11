//
//  BasePointUIView.m
//  F2Native
//
//  Created by 青原 on 2021/1/4.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "BasePointUIView.h"

@implementation BasePointUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_singlePointChart" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    //    self.chart.scale(@"y", @{@"min": @(0)});
    //    self.chart.axis(@"x", @{@"grid": @(NO), @"label": @{@"textAlign": @"start"}});

    self.chart.point().position(@"x*y").size(@"z", @[@(3), @(10)]).fixedShape(@"circle");
    self.chart.render();
}

@end
