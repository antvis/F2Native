#import "BaseBarChart.h"

@implementation BaseBarChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 10.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"year", @{@"grid": @(NO)});
    self.chart.axis(@"sales", @{@"grid": @{@"type": @"dash", @"lineWidth": @(2), @"dash": @[@(6), @(2)]}});
    self.chart.scale(@"year", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.scale(@"sales", @{@"nice": @(YES)});
    self.chart.interval().position(@"year*sales").style(@{
        @"rounding": @[@(10), @(10), @(0), @(0)] // [tl, tr, bl, br]
    });
    self.chart.tooltip(@{});
    self.chart.animate(YES);
    self.chart.render();
}

@end
