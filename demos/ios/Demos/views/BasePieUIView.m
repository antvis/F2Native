

#import "BasePieUIView.h"

@implementation BasePieUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_basePie" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 20, 20, 0.f).source(jsonData);
    self.chart.axis(@"percent", @{ @"line": @(NO), @"label": @(NO) });
    self.chart.axis(@"a", @{ @"line": @(NO), @"label": @(NO) });
    self.chart.coord(@{ @"type": @"polar", @"transposed": @(YES) });
    self.chart.interval().style(@{ @"lineWidth": @(1) }).position(@"a*percent").color(@"name", @[]).adjust(@"stack");
    self.chart.render();
}
@end
