//
//  BaseAreaUIView2.m
//  F2Native
//
//  Created by 青原 on 2020/12/23.
//  Copyright © 2020 com.alipay.xgraph. All rights reserved.
//

#import "BaseAreaUIView2.h"

@implementation BaseAreaUIView2

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_singleAreaChart_2" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0).source(jsonData);
    self.chart.axis(@"tem", @{@"grid": @{@"stroke": @"#000"}});

    self.chart.line().position(@"month*value");
    self.chart.area().position(@"month*value").fixedColor(@"#1890FF0F");

    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.animate(@YES);
    self.chart.render();
}

@end
