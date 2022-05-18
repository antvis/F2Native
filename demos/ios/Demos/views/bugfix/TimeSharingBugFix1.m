//
//  TimeSharingUIView.m
//  F2Native
//
//  Created by ruize on 2022/2/7.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "TimeSharingBugFix1.h"

@interface TimeSharingBugFix1()<F2GestureDelegate>
@property (nonatomic, strong) F2Chart *priceChart;
@property (nonatomic, strong) F2Chart *subChart;
@end

@implementation TimeSharingBugFix1

-(instancetype)initWithFrame:(CGRect)frame {
    CGRect chartFrame = CGRectMake(0, 0, 168.67, 110.33);
    if (self = [super initWithFrame:chartFrame]) {}
    return self;
}

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/bugfix/mockData_timeSharingBugfix1" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.priceChart.canvas(self.canvasView).padding(0, 0, 0, 0.f).source(jsonData);
    self.priceChart.line().position(@"date*price").fixedColor(@"#E62C3B").attrs(@{@"connectNulls": @(YES)}).fixedShape(@"smooth").fixedSize(1);
    self.priceChart.area().position(@"date*price").fixedColor(@"#E62C3B18").attrs(@{@"connectNulls": @(YES)}).fixedShape(@"smooth");

    self.priceChart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1644197400000), @(1644204600000)], @[@(1644210000000), @(1644217200000)]]
    });
    self.priceChart.scale(@"price",@{@"max":@3.942, @"min":@3.51});
    self.priceChart.axis(@"price", @{@"hidden": @(YES)});
    self.priceChart.axis(@"date", @{
        @"hidden":@YES
    });
    self.priceChart.guide().line(@{@"orientation":@"horizontal" , @"position":@[@"min", @3.87], @"lineWidth":@1, @"dash":@[@4, @4], @"color":@"#E62C3B"});
    self.priceChart.tooltip(@{@"yTip":@{@"inner":@(YES)}});
    self.priceChart.render();
    
    self.subChart.canvas(self.canvasView);
    self.subChart.margin(0, 210, 0, 0);
    self.subChart.padding(15, 0, 15, 20);
    self.subChart.source(jsonData);
    self.subChart.axis(@"date", @{@"hidden": @(YES)});
    self.subChart.axis(@"volume", @{@"hidden": @(YES)});
    self.subChart.interval().position(@"date*volume");
    self.subChart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1608687000000), @(1608694200000)], @[@(1608699600000), @(1608706800000)]]
    });
    self.subChart.interaction(@"pan", @{});
    self.subChart.interaction(@"pinch", @{});
    self.subChart.tooltip(@{@"yTip":@{@"inner":@(YES)}});
    self.subChart.render();
}

-(void)dealloc {
   [self.canvasView destroy];
}

- (F2Chart *)priceChart {
   if(!_priceChart) {
       CGSize size = self.canvasView.bounds.size;
       size.height = size.height * 3 / 4;
       _priceChart = [F2Chart chart:size withName:@"TimeSharingUIView#pricechart"];
   }
   return _priceChart;
}

- (F2Chart *)subChart {
    if(!_subChart) {
        CGSize size = self.canvasView.bounds.size;
        size.height = size.height / 4;
        _subChart = [F2Chart chart:size withName:@"TimeSharingUIView#Sub"];
    }
    return _subChart;
}

-(NSString *)name {
    return @"TimeSharingUIView";
}

- (void)handleGestureInfo:(NSDictionary *)info sender:(nonnull UIGestureRecognizer *)gestureRecognizer {
    self.priceChart.postTouchEvent(info);
    self.subChart.postTouchEvent(info);
}


@end
