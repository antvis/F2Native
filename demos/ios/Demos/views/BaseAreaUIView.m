

#import "BaseAreaUIView.h"
#import <F2/F2.h>

@implementation BaseAreaUIView

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseArea" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.clear();
    self.chart.canvas(self.canvasView).padding(20, 10, 0, 0.f).source(jsonData);
    self.chart.scale(@"tem", @{@"min": @(0)});
    self.chart.axis(@"time", @{@"grid": @(NO), @"label": @{@"textAlign": @"start"}});
    self.chart.line().position(@"time*tem").fixedSize(2).fixedShape(@"smooth");
    self.chart.area().position(@"time*tem").fixedShape(@"smooth").fixedColor([F2Utils toJsonString:@{
        @"colorStops": @[@{@"offset": @(0.f), @"color": @"red"}, @{@"offset": @(1.f), @"color": @"white"}],
        @"position": @[@(0), @(0), @(0), @(self.canvasView.bounds.size.height * [UIScreen mainScreen].scale)]
    }]);
    self.chart.point().position(@"time*tem").fixedColor(@"#000000").style(@{
        @"custom": [F2CallbackObj initWithCallback:^NSDictionary *_Nullable(NSString *_Nonnull param) {
            NSData *data = [param dataUsingEncoding:NSUTF8StringEncoding];
            NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
            if([json[@"_index"] longValue] == 3) {
                return @{@"color": @"#FFB6C1", @"size": @(0)};
            }
            return @{};
        }]
    });

    self.chart.render();
}

@end
