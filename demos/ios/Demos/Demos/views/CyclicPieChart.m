
#import "CyclicPieChart.h"

@implementation CyclicPieChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_basePie" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(0, 0, 0, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"percent", @{@"hidden": @YES});
    self.chart.axis(@"a", @{@"hidden": @YES});
    self.chart.coord(@{@"type": @"polar"});
    self.chart.interval().style(@{@"lineWidth": @(0)}).position(@"a*percent").color(@"name", @[]).adjust(@"stack");
    self.chart.animate(YES);
    self.chart.render();
}

@end
