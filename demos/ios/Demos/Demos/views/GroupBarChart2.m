//
//  MultiIntervalUIView2.m
//  F2Native
//
//  Created by 青原 on 2020/12/22.
//  Copyright © 2020 com.alipay.xgraph. All rights reserved.
//

#import "GroupBarChart2.h"

@implementation GroupBarChart2

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_singleIntervalChart_under_zero" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"tem", @{@"grid": @{@"stroke": @"#000"}});
    self.chart.interval().position(@"time*tem").color(@"city", @[]).adjust(@"dodge");
    self.chart.scale(@"time", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.scale(@"tem", @{@"nice": @(YES)});
    self.chart.tooltip(@{});
    self.chart.render();
}

@end
