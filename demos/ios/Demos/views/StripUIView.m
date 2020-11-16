
#import "StripUIView.h"

@implementation StripUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseInterval" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.coord(@{ @"transposed": @(YES) });
    self.chart.scale(@"sales", @{ @"min": @(0) });
    self.chart.scale(@"year", @{ @"range": @[@(0.1), @(0.9)] });
    self.chart.interval().position(@"year*sales");
    self.chart.render();
}

@end
