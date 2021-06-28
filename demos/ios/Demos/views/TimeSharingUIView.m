//
//  TimeSharingUIView.m
//  F2Native
//
//  Created by 青原 on 2021/6/28.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "TimeSharingUIView.h"

@implementation TimeSharingUIView

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_timeSharing" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.line().position(@"date*price").fixedColor(@"#528EFF");
    self.chart.area().position(@"date*price").fixedColor(@"#108EE9");

    self.chart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1608687000000), @(1608694200000)], @[@(1608699600000), @(1608706800000)]]
    });
    self.chart.axis(@"price", @{@"hidden": @(YES)});
    self.chart.axis(@"date", @{
        @"label": @{
            @"labelMargin": @(20),
            @"labelOffset": @(4),
            @"textColor": @"#999999",
            @"item": [F2CallbackObj initWithCallback:^id _Nullable(NSString *_Nonnull param) {
                NSData *data = [param dataUsingEncoding:NSUTF8StringEncoding];
                NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                NSNumber *index = [json objectForKey:@"index"];
                if([index isEqual:@(0)]) {
                    return @{@"content": @"09:30"};
                } else if([index isEqual:@(1)]) {
                    return @{@"content": @"11:30/13:00"};
                } else if([index isEqual:@(2)]) {
                    return @{@"content": @"15:00"};
                } else {
                    return @{@"content": @""};
                }
            }]
        }
    });
    self.chart.render();
}

@end
