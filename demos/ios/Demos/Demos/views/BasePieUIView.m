

#import "BasePieUIView.h"

@implementation BasePieUIView

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_basePie" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 20, 20, 0.f).source(jsonData);
    self.chart.axis(@"percent", @{@"line": @(NO), @"label": @(NO), @"grid": @(NO)});
    self.chart.axis(@"a", @{@"line": @(NO), @"label": @(NO) , @"grid": @(NO)});
    self.chart.coord(@{@"type": @"polar", @"transposed": @(YES)});
    self.chart.interval().style(@{@"lineWidth": @(1),@"stroke": @"#ffffff"}).position(@"a*percent").color(@"name", @[]).adjust(@"stack").fixedSize(1);
    self.chart.animate(@YES);
    self.chart.render();
}
@end
