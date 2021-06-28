
#import "CyclicPieUIView.h"

@implementation CyclicPieUIView

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_basePie" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.axis(@"percent", @{@"line": @(NO), @"label": @(NO)});
    self.chart.axis(@"a", @{@"line": @(NO), @"label": @(NO)});
    self.chart.coord(@{@"type": @"polar"});
    self.chart.interval().style(@{@"lineWidth": @(0)}).position(@"a*percent").color(@"name", @[]).adjust(@"stack");
    self.chart.render();
}

@end
