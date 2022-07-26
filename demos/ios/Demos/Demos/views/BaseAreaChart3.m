//
//  BaseAreaUIView3.m
//  F2Native
//
//  Created by weiqing.twq on 2021/4/27.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "BaseAreaChart3.h"

@implementation BaseAreaChart3

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_singleAreaChart_2" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0).source(jsonData);
    self.chart.axis(@"tem", @{@"grid": @{@"stroke": @"#000"}});

    self.chart.line().position(@"month*value");
    self.chart.area().position(@"month*value").fixedColor(@"#1890FF0F").style(@{@"startOnZero":@(YES)});

    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.render();
}

@end
