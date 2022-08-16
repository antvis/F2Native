//
//  RadarAreaUIView.m
//  F2Native
//
//  Created by weiqing.twq on 2021/9/16.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "RadarAreaChart.h"

@implementation RadarAreaChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_radarArea" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.clear();
    self.chart.canvas(self.canvasView).padding(0, 10, 0, 0);
    self.chart.source([F2Utils toJsonArray:jsonData]);
   
    self.chart.line().position(@"item*score").color(@"user", @[]);
    self.chart.area().position(@"item*score").color(@"user", @[]);
    self.chart.point().position(@"item*score").color(@"user", @[]);
    self.chart.coord(@{@"type": @"polar"});
    self.chart.scale(@"score", @{@"type":@"linear", @"min": @(0), @"max": @(120) , @"nice": @(NO), @"tickCount": @(4)});
    self.chart.scale(@"item", @{@"type": @"cat"});
    self.chart.axis(@"score", @{
        @"label":@{@"labelOffset":@(20)},
        @"grid":@{@"type":@"line", @"fill":@[@"#E8E8E8", @"#ffffff"], @"fillOpacity":@[@0.1, @1.0], @"stroke" :@"#E8E8E8"}});
    self.chart.axis(@"item", @{
        @"label":@{@"labelOffset":@(10)},
        @"grid":@{@"type":@"dash", @"stroke" :@"#E8E8E8", @"dash": @[@(4), @(2)]}});
    self.chart.animate(YES);
    self.chart.legend(@"user", @{@"position": @"top"});
    self.chart.render();
    
    CGPoint center = self.chart.getCoord().getCenter();
    CGPoint design = self.chart.getPosition(@{@"item":@"Design", @"score":@(70)});
    CGPoint technology = self.chart.getPosition(@{@"item":@"Technology",@"score": @(70)});
    CGPoint test = self.chart.getPosition(@{@"item":@"Test",@"score": @(70)});
    CGPoint marketing = self.chart.getPosition(@{@"item":@"Marketing",@"score": @(60)});

    NSString *textAlign = design.x > center.x ? @"start" : (design.x < center.x ? @"end" : @"center");
    NSString *textBaseline = design.y > center.y ? @"top" : (design.y < center.y ? @"bottom" : @"middle");
    self.chart.guide().text(@{@"content":@"70", @"position":@[@"Design", @(70)],@"textAlign": textAlign, @"textBaseline":textBaseline, @"margin":@[@(0), @(5)]});

    textAlign = technology.x > center.x ? @"start" : (technology.x < center.x ? @"end" : @"center");
    textBaseline = technology.y > center.y ? @"top" : (technology.y < center.y ? @"bottom" : @"middle");
    self.chart.guide().text(@{@"content":@"70", @"position":@[@"Technology", @(70)],@"textAlign": textAlign, @"textBaseline":textBaseline, @"margin":@[@(5), @(0)]});

    textAlign = test.x > center.x ? @"start" : (test.x < center.x ? @"end" : @"center");
    textBaseline = test.y > center.y ? @"top" : (test.y < center.y ? @"bottom" : @"middle");
    self.chart.guide().text(@{@"content":@"70", @"position":@[@"Test", @(70)],@"textAlign": textAlign, @"textBaseline":textBaseline, @"margin":@[@(0), @(-5)]});

    textAlign = marketing.x > center.x ? @"start" : (marketing.x < center.x ? @"end" : @"center");
    textBaseline = marketing.y > center.y ? @"top" : (marketing.y < center.y ? @"bottom" : @"middle");
    self.chart.guide().text(@{@"content":@"60", @"position":@[@"Marketing", @(60)],@"textAlign": textAlign, @"textBaseline":textBaseline, @"margin":@[@(-5), @(0)]});
    self.chart.repaint();
}

@end
