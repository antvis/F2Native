

#import "BaseAreaUIView.h"
#import <F2/F2.h>

@implementation BaseAreaUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseArea" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.scale(@"tem", @{ @"min": @(0) });
    self.chart.axis(@"time", @{ @"grid": @(NO), @"label": @{@"textAlign": @"start"} });
    self.chart.line().position(@"time*tem").fixedSize(2).fixedShape(@"smooth");
    self.chart.area().position(@"time*tem").fixedShape(@"smooth");
    self.chart.render();
}

@end
