//
//  MarketMovingBugFix1.m
//  F2Native
//
//  Created by weiqing.twq on 2022/2/23.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import "MarketMovingBugFix1.h"

@implementation MarketMovingBugFix1

-(instancetype)initWithFrame:(CGRect)frame {
    CGRect chartFrame = CGRectMake(0, 0, frame.size.width, 150);
    if (self = [super initWithFrame:chartFrame]) {}
    return self;
}

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/bugfix/mockData_marketMovingBugfix1" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(15, 10, 15, 0.f).source(jsonData);
    self.chart.line().position(@"date*price").fixedColor(@"#528EFF").attrs(@{@"connectNulls": @(YES)}).fixedShape(@"smooth");
    self.chart.area().position(@"date*price").fixedColor(@"#108EE9").attrs(@{@"connectNulls": @(YES)}).fixedShape(@"smooth");

    self.chart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1645147800000), @(1645155000000)], @[@(1645160400000), @(1645167600000)]]
    });
    self.chart.axis(@"price", @{@"hidden": @(YES), @"max":@(3485.8375000000005), @"min":@(3450.2333)});
    self.chart.axis(@"date", @{
        @"label": @{
            @"textColor": @"#999999",
        }
    });
    self.chart.guide().flag(@{@"content":@"基础建设",
                                   @"position": @[@1645147800000, @3450.2333],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.chart.guide().flag(@{@"content":@"数据中心",
                                   @"position": @[@1645149720000, @3450.2333],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    
    self.chart.guide().text(@{@"content":@"3450.23", @"position":@[@"min", @"min"], @"textColor": @"#999999",@"margin": @[@-5, @5], @"textSize": @12});
    self.chart.guide().text(@{@"content":@"3485.84", @"position":@[@"min", @"max"], @"textColor": @"#999999",@"margin": @[@-5, @5], @"textSize": @12});
//    self.chart.guide().flag(@{@"content":@"汽车零部件",
//                                   @"position": @[@1639102800000, @3665.2538],
//                                   @"color": @"#108EE9",
//                                   @"textColor":@"#108EE9",
//                                   @"lineWidth": @(0.5),
//                                   @"textSize": @(11),
//                                   @"backgroundColor": @"#108EE920",
//                                   @"padding": @[@5, @5, @5, @2]});
//    self.chart.guide().flag(@{@"content":@"银行",
//                                   @"position": @[@1639104900000, @3665.7622],
//                                   @"color": @"#108EE9",
//                                   @"textColor":@"#108EE9",
//                                   @"lineWidth": @(0.5),
//                                   @"textSize": @(11),
//                                   @"backgroundColor": @"#108EE920",
//                                   @"padding": @[@5, @5, @5, @2]});
//    self.chart.guide().flag(@{@"content":@"元宇宙",
//                                   @"position": @[@1639106340000, @3659.8543],
//                                   @"color": @"#108EE9",
//                                   @"textColor":@"#108EE9",
//                                   @"lineWidth": @(0.5),
//                                   @"textSize": @(11),
//                                   @"backgroundColor": @"#108EE920",
//                                   @"padding": @[@5, @5, @5, @2]});
//    self.chart.guide().flag(@{@"content":@"燃气",
//                                   @"position": @[@1639112940000, @3665.5504],
//                                   @"color": @"#108EE9",
//                                   @"textColor":@"#108EE9",
//                                   @"lineWidth": @(0.5),
//                                   @"textSize": @(11),
//                                   @"backgroundColor": @"#108EE920",
//                                   @"padding": @[@5, @5, @5, @2]});
//    self.chart.guide().flag(@{@"content":@"煤炭开采",
//                                   @"position": @[@1639114800000, @3665.3084],
//                                   @"color": @"#108EE9",
//                                   @"textColor":@"#FFFFFF",
//                                   @"lineWidth": @(0.5),
//                                   @"textSize": @(11),
//                                   @"backgroundColor": @"#108EE990",
//                                   @"padding": @[@5, @5, @5, @2]});
//    self.chart.tooltip(@{@"yTip":@{@"inner":@(YES)}});
    self.chart.render();
}

@end
