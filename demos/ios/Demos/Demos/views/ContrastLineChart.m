
#import "ContrastLineChart.h"

@implementation ContrastLineChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_multiLines" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.scale(@"date", @{@"tickCount": @(5)});
    self.chart.axis(@"date", @{@"label": @{@"textAlign": @"center"}});
    self.chart.line().position(@"date*value").color(@"type", @[]).fixedSize(2).fixedShape(@"smooth");
    self.chart.legend(@"type", @{@"radius": @(3), @"symbol": @"square"});
    self.chart.tooltip(@{});
    self.chart.render();
}

@end
