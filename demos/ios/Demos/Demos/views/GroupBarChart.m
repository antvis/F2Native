#import "GroupBarChart.h"

@implementation GroupBarChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_multiInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"month", @{@"grid": @(NO)});
    self.chart.scale(@"month", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.interval().adjust(@"dodge").position(@"month*value").color(@"name", @[]);
    self.chart.tooltip(@{});
    self.chart.render();
}

@end
