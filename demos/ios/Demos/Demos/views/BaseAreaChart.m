

#import "BaseAreaChart.h"
#import <F2/F2.h>

@implementation BaseAreaChart

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseArea" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.clear();
    self.chart.canvas(self.canvasView).padding(20, 10, 0, 0.f).source(jsonData);
    self.chart.scale(@"tem", @{@"min": @(0)});
    self.chart.axis(@"time", @{@"grid": @(NO), @"label": @{@"textAlign": @"start"}});
    self.chart.line().position(@"time*tem").fixedSize(2).fixedShape(@"smooth");
    self.chart.area().position(@"time*tem").fixedShape(@"smooth").fixedColor([F2Utils toJsonString:@{
        @"colorStops": @[@{@"offset": @(0.f), @"color": @"#ff0000"}, @{@"offset": @(1.f), @"color": @"#ffffff"}],
        @"position": @[@(0), @(0), @(0), @(self.chartSize.height * [UIScreen mainScreen].scale)]
    }]);
    self.chart.point().position(@"time*tem").fixedColor(@"#000000").style(@{
        @"custom": [F2Callback callback:^NSDictionary *_Nonnull(NSDictionary *_Nonnull param) {
            if([param[@"_index"] longValue] == 3) {
                return @{@"color": @"#FFB6C1", @"size": @(0)};
            }
            return @{};
        }]
    });
    self.chart.animate(@YES);
    self.chart.render();
}

@end
