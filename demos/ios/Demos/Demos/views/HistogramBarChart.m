
#import "HistogramBarChart.h"

@implementation HistogramBarChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_setionInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"x", @{@"grid": @(NO)});
    self.chart.axis(@"y", @{@"grid": @{@"type": @"dash", @"lineWidth": @(2), @"dash": @[@(6), @(2)]}});
    self.chart.scale(@"x", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.scale(@"y", @{@"nice": @(YES)});
    self.chart.interval().position(@"x*y");
    self.chart.render();
}

@end
