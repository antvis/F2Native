//
//  TimeSharingUIView.m
//  F2Native
//
//  Created by 青原 on 2021/6/28.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "TimeSharingChart.h"

@interface TimeSharingChart()
@property (nonatomic, strong) F2Chart *priceChart;
@property (nonatomic, strong) F2Chart *subChart;
@end

@implementation TimeSharingChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_timeSharing" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.priceChart.canvas(self.canvasView).padding(15, 10, 15, 10.f).source(jsonData);
    self.priceChart.line().position(@"date*price").fixedColor(@"#528EFF");
    self.priceChart.area().position(@"date*price").fixedColor(@"#108EE9");

    self.priceChart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1608687000000), @(1608694200000)], @[@(1608699600000), @(1608706800000)]]
    });
    self.priceChart.axis(@"price", @{@"hidden": @(YES)});
    self.priceChart.axis(@"date", @{
        @"label": @{            
            @"textColor": @"#999999",
            @"item": [F2Callback callback:^NSDictionary *_Nonnull(NSDictionary *_Nonnull param) {
               
                NSNumber *index = [param objectForKey:@"index"];
                if([index isEqual:@(0)]) {
                    return @{@"content": @"09:30", @"textAlign":@"start"};
                } else if([index isEqual:@(1)]) {
                    return @{@"content": @"11:30/13:00", @"textAlign":@"center"};
                } else if([index isEqual:@(2)]) {
                    return @{@"content": @"15:00", @"textAlign":@"end"};
                } else {
                    return @{@"content": @""};
                }
            }]
        }
    });
    
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
    self.subChart.tooltip(@{@"yTip":@{@"inner":@(YES)}});
    self.subChart.render();
    
    //多个chart需要自行监听手势，并分发
    F2WeakSelf;
    [self.canvasView addGestureListener:@"longPress" callback:^(NSDictionary * _Nonnull info) {
        F2StrongSelf;
        [strongSelf handleGestureInfo:info];
    }];
}

- (F2Chart *)priceChart {
   if(!_priceChart) {
       CGSize size = self.canvasView.bounds.size;
       size.height = size.height * 3 / 4;
       _priceChart = [F2Chart chart:size name:@"TimeSharingUIView#pricechart"];
   }
   return _priceChart;
}

- (F2Chart *)subChart {
    if(!_subChart) {
        CGSize size = self.canvasView.bounds.size;
        size.height = size.height / 4;
        _subChart = [F2Chart chart:size name:@"TimeSharingUIView#Sub"];
    }
    return _subChart;
}

-(NSString *)name {
    return @"TimeSharingUIView";
}

- (void)handleGestureInfo:(NSDictionary *)info {
    self.priceChart.postTouchEvent(info);
    self.subChart.postTouchEvent(info);
}


@end
