#import "MultiIntervalUIView3.h"

@implementation MultiIntervalUIView3

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_multiInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.axis(@"month", @{@"grid": @(NO)});
    self.chart.scale(@"month", @{@"range": @[@(0.1), @(0.9)]});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.interval().adjust(@"stack").position(@"month*value").color(@"name", @[]);
    self.chart.render();
}

@end
