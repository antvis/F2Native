

#import "BaseIntervalUIView.h"

@implementation BaseIntervalUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.axis(@"year", @{ @"grid": @(NO) });
    self.chart.axis(@"sales", @{ @"grid": @{@"type": @"dash", @"lineWidth": @(2), @"lineDash": @[@(6), @(2)]} });
    self.chart.scale(@"year", @{ @"range": @[@(0.1), @(0.9)] });
    self.chart.scale(@"sales", @{ @"min": @(1) });
    self.chart.interval().position(@"year*sales");
    self.chart.render();
}

@end
