#import "MarketMovingChart.h"

@implementation MarketMovingChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_marketMoving" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970] * 1000;
    self.chart.canvas(self.canvasView).padding(15, 10, 15, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.line().position(@"date*price").fixedColor(@"#528EFF").fixedShape(@"smooth");
    self.chart.area().position(@"date*price").fixedColor(@"#108EE9").fixedShape(@"smooth");

    self.chart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1639099800000), @(1639107000000)], @[@(1639112400000), @(1639119600000)]]
    });
    self.chart.axis(@"price", @{@"hidden": @(YES), @"max":@(3582.115), @"min":@(3543.2786000000006)});
    self.chart.axis(@"date", @{
        @"label": @{            
            @"textColor": @"#999999",            
        }
    });
    self.chart.guide().flag(@{@"content":@"证券",
                                   @"position": @[@"1639099800000", @"3656.9477"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"盐湖提锂",
                                   @"position": @[@"1639101240000", @"3662.4675"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"汽车零部件",
                                   @"position": @[@"1639102800000", @"3665.2538"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"银行",
                                   @"position": @[@"1639104900000", @"3665.7622"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"元宇宙",
                                   @"position": @[@"1639106340000", @"3659.8543"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"燃气",
                                   @"position": @[@"1639112940000", @"3665.5504"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"煤炭开采",
                                   @"position": @[@"1639114800000", @"3665.3084"],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#FFFFFF",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE990",
                                   @"padding": @[@5, @5, @5, @2],
                                   @"rounding":@[@3, @0, @3, @0]
                                 });
    self.chart.tooltip(@{@"yTip":@{@"inner":@(YES)}});
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970] * 1000;
    NSLog(@"config parse cost %fms", (end - start));
    self.chart.render();
}


@end
