#import "MarketMovingUIView.h"

@interface MarketMovingUIView()
@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *priceChart;
@property (nonatomic, strong) F2Chart *subChart;
@end

@implementation MarketMovingUIView

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_marketMoving" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970] * 1000;
    self.priceChart.canvas(self.canvasView).padding(15, 10, 15, 0.f).source(jsonData);
    self.priceChart.line().position(@"date*price").fixedColor(@"#528EFF").attrs(@{@"connectNulls": @(YES)}).fixedShape(@"smooth");
    self.priceChart.area().position(@"date*price").fixedColor(@"#108EE9").attrs(@{@"connectNulls": @(YES)}).fixedShape(@"smooth");

    self.priceChart.scale(@"date", @{
        @"type": @"timeSharing",
        @"timeRange": @[@[@(1639099800000), @(1639107000000)], @[@(1639112400000), @(1639119600000)]]
    });
    self.priceChart.axis(@"price", @{@"hidden": @(YES), @"max":@(3582.115), @"min":@(3543.2786000000006)});
    self.priceChart.axis(@"date", @{
        @"label": @{            
            @"textColor": @"#999999",            
        }
    });
    self.priceChart.guide().flag(@{@"content":@"证券",
                                   @"position": @[@1639099800000, @3656.9477],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.priceChart.guide().flag(@{@"content":@"盐湖提锂",
                                   @"position": @[@1639101240000, @3662.4675],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.priceChart.guide().flag(@{@"content":@"汽车零部件",
                                   @"position": @[@1639102800000, @3665.2538],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.priceChart.guide().flag(@{@"content":@"银行",
                                   @"position": @[@1639104900000, @3665.7622],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.priceChart.guide().flag(@{@"content":@"元宇宙",
                                   @"position": @[@1639106340000, @3659.8543],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.priceChart.guide().flag(@{@"content":@"燃气",
                                   @"position": @[@1639112940000, @3665.5504],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#108EE9",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE920",
                                   @"padding": @[@5, @5, @5, @2]});
    self.priceChart.guide().flag(@{@"content":@"煤炭开采",
                                   @"position": @[@1639114800000, @3665.3084],
                                   @"color": @"#108EE9",
                                   @"textColor":@"#FFFFFF",
                                   @"lineWidth": @(0.5),
                                   @"textSize": @(11),
                                   @"backgroundColor": @"#108EE990",
                                   @"padding": @[@5, @5, @5, @2],
                                   @"rounding":@[@3, @0, @3, @0]
                                 });
    self.priceChart.tooltip(@{@"yTip":@{@"inner":@(YES)}});
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970] * 1000;
    NSLog(@"config parse cost %fms", (end - start));
    self.priceChart.render();
}

- (F2Chart *)priceChart {
   if(!_priceChart) {
       CGSize size = self.canvasView.bounds.size;
       size.height = size.height * 3 / 4;
       _priceChart = [F2Chart chart:size withName:@"MarketMovingUIView#pricechart"];
   }
   return _priceChart;
}


-(NSString *)name {
    return @"MarketMovingUIView";
}


@end
