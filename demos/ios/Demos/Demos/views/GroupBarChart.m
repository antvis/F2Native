#import "GroupBarChart.h"

@implementation GroupBarChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_multiInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(15, 15, 15, 15.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"month", @{@"grid": @(NO)});
    self.chart.scale(@"month", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.interval().adjust(@"dodge").position(@"month*value").color(@"name", @[]);
    self.chart.tooltip(@{});
    self.chart.legend(@"name", @{@"position": @"bottom", @"align": @"center"});
    self.chart.render();
}

@end
