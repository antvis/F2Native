

#import "BasePieChart.h"

@implementation BasePieChart

- (void)chartRender {    
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_basePie" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(0, 0, 0, 0.f);
    self.chart.source([F2Utils toJsonArray:jsonData]);
    self.chart.axis(@"percent", @{@"line": @(NO), @"label": @(NO), @"grid": @(NO)});
    self.chart.axis(@"a", @{@"line": @(NO), @"label": @(NO) , @"grid": @(NO)});
    self.chart.coord(@{@"type": @"polar", @"transposed": @(YES)});
    self.chart.interval().position(@"a*percent").color(@"name", @[]).adjust(@"stack").fixedSize(1).style(@{@"lineWidth": @(1),@"stroke": @"#ffffff"});
    self.chart.animate(@YES);
    self.chart.render();
}
@end
