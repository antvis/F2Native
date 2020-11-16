

#import "MultiAreaUIView.h"

@implementation MultiAreaUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_multiArea" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.scale(@"date", @{ @"tickCount": @(5) });
    self.chart.scale(@"value", @{ @"min": @(0) });
    self.chart.axis(@"date", @{ @"grid": @(NO), @"label": @{@"textAlign": @"start"} });
    self.chart.line().adjust(@"stack").position(@"date*value").color(@"city", @[]).fixedSize(2);
    self.chart.area().adjust(@"stack").position(@"date*value").color(@"city", @[]).fixedSize(2);
    self.chart.render();
}

@end
