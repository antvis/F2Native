//
//  KLineUIView.m
//  F2Native
//
//  Created by XiaLuo on 2021/1/14.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "KLineUIView.h"

@interface KLineUIView () <F2GestureDelegate>

@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *candleChart;
@property (nonatomic, strong) F2Chart *subChart;

@end

@implementation KLineUIView

- (instancetype)init {
    if(self = [super initWithFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, 280)]) {
        [self addSubview:self.canvasView];
        [self chartRender];
    }
    return self;
}

- (void)chartRender {

    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_klineDay" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];

    self.candleChart.canvas(self.canvasView).padding(15, 10, 15, 0.f).source(jsonData);
    self.candleChart.candle().position(@"date*values"); // .style(@{@"stroke": @[@"#1CAA3D", @"#808080", @"#F4333C"]});
    self.candleChart.line().position(@"date*m5").fixedColor(@"#A46FFE");
    self.candleChart.line().position(@"date*m10").fixedColor(@"#46ACFF");
    self.candleChart.line().position(@"date*m20").fixedColor(@"#FF8C53");
    self.candleChart.scale(@"date", @{@"type": @"kline-day", @"domain": @[@(40), @(80)]});
    self.candleChart.scale(@"values", @{@"nice": @(YES)});
    self.candleChart.scale(@"m5", @{@"assign": @"values"});
    self.candleChart.scale(@"m10", @{@"assign": @"values"});
    self.candleChart.scale(@"m20", @{@"assign": @"values"});

    self.candleChart.axis(@"m5", @{@"hidden": @(YES)});
    self.candleChart.axis(@"m10", @{@"hidden": @(YES)});
    self.candleChart.axis(@"m20", @{@"hidden": @(YES)});

    self.candleChart.interaction(@"pan", @{});
    self.candleChart.interaction(@"pinch", @{});
    self.candleChart.tooltip(@{});
    self.candleChart.render();

    self.subChart.canvas(self.canvasView);
    self.subChart.margin(0, 210, 0, 0);
    self.subChart.padding(15, 10, 15, 10);
    self.subChart.source(jsonData);
    self.subChart.axis(@"date", @{@"hidden": @(YES)});
    self.subChart.interval().position(@"date*volumn");
    self.subChart.scale(@"date", @{@"type": @"kline-day", @"domain": @[@(40), @(80)]});
    self.subChart.scale(@"volumn", @{
        @"nice": @(YES),
        @"tickCount": @(2),
        @"tick": [F2CallbackObj initWithCallback:^NSDictionary *_Nullable(NSString *_Nonnull param) {
            return @{@"content": @"1亿手"};
        }]
    });
    self.subChart.interaction(@"pan", @{});
    self.subChart.interaction(@"pinch", @{});
    self.subChart.tooltip(@{});
    self.subChart.render();
}

- (F2CanvasView *)canvasView {
    if(!_canvasView) {
        _canvasView = [F2CanvasView canvasWithFrame:self.frame andBizId:@"KLineUIView" complete:nil];
        _canvasView.delegate = self;
    }
    return _canvasView;
}

- (F2Chart *)candleChart {
    if(!_candleChart) {
        CGSize size = self.canvasView.bounds.size;
        size.height = size.height * 3 / 4;
        _candleChart = [F2Chart chart:size withName:@"KLineChart#Candle"];
    }
    return _candleChart;
}

- (F2Chart *)subChart {
    if(!_subChart) {
        CGSize size = self.canvasView.bounds.size;
        size.height = size.height / 4;
        _subChart = [F2Chart chart:size withName:@"KLineChart#Sub"];
    }
    return _subChart;
}

- (void)handleGestureInfo:(NSDictionary *)info sender:(nonnull UIGestureRecognizer *)gestureRecognizer {
    self.candleChart.postTouchEvent(info);
    self.subChart.postTouchEvent(info);
}

@end
