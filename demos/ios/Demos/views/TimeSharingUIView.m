//
//  TimeSharingUIView.m
//  F2Native
//
//  Created by 青原 on 2021/6/28.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import "TimeSharingUIView.h"

@interface TimeSharingUIView()<F2GestureDelegate>
@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *priceChart;
@property (nonatomic, strong) F2Chart *subChart;
@end

@implementation TimeSharingUIView

-(instancetype)initWithFrame:(CGRect)frame {
    CGRect chartFrame = CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, 280);
    if (self = [super initWithFrame:chartFrame]) {
        [self createCanvasView];
        [self chartRender];
    }
    return self;
}


-(void)createCanvasView {
    F2CanvasView *view = [F2CanvasView canvasWithFrame:self.frame
                                              andBizId:self.name
                                             andThread:nil
                                              complete:nil
    ];

    self.canvasView = view;
    self.canvasView.delegate = self;
    [self addSubview:self.canvasView];
}

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_timeSharing" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.priceChart.canvas(self.canvasView).padding(15, 10, 15, 0.f).source(jsonData);
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
            @"item": [F2CallbackObj initWithCallback:^id _Nullable(NSString *_Nonnull param) {
                NSData *data = [param dataUsingEncoding:NSUTF8StringEncoding];
                NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                NSNumber *index = [json objectForKey:@"index"];
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
