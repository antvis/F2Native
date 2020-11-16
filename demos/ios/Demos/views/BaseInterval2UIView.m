//
//  BaseInterval2UIView.m
//  Demos
//
//  Created by XiaLuo on 2020/11/5.
//  Copyright © 2020 com.alipay.xgraph. All rights reserved.
//

#import "BaseInterval2UIView.h"

@implementation BaseInterval2UIView

- (NSString *)jsonData {
    return @"[{\"genre\":\"Sports\",\"sold\":275},{\"genre\":\"Strategy\",\"sold\":115},{\"genre\":\"Action\",\"sold\":120},{"
           @"\"genre\":\"Shooter\",\"sold\":350},{\"genre\":\"Other\",\"sold\":150}]";
}

- (void)chartRender {
    // step1
    [self addSubview:self.canvasView];
    // step3
    self.chart.canvas(self.canvasView).padding(10, 20, 10, 0.f).source([self jsonData]);
    // step4
    self.chart.interval().position(@"genre*sold").color(@"genre", @[]);
    self.chart.scale(@"sold", @{ @"min": @(0) });
    self.chart.scale(@"genre", @{ @"range": @[@(0.1), @(0.9)] });
    // step5
    self.chart.render();
}

@end
