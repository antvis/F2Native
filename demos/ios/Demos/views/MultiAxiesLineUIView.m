
#import "MultiAxiesLineUIView.h"

@implementation MultiAxiesLineUIView

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_multiAxiesLine" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.scale(@"date", @{@"tickCount": @(5)});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.scale(@"offset", @{@"nice": @(YES)});
    self.chart.axis(@"value", @{@"grid": @{@"type": @"dash", @"dash": @[@(15), @(15), @(5), @(5)]}});
    self.chart.axis(@"offset", @{
        @"label": @{
            @"labelOffset": @(5),
        }
    });
    self.chart.axis(@"date", @{
        @"label": @{
            @"labelOffset": @(5),
        }
    });
    self.chart.line().position(@"date*value").fixedSize(2).fixedColor(@"#1890FF");
    self.chart.line().position(@"date*offset").fixedSize(2).fixedColor(@"#2FC25B");
    self.chart.tooltip(@{});
    self.chart.interaction(@"pinch", @{});
    self.chart.interaction(@"pan", @{});
    self.chart.render();
}

@end
