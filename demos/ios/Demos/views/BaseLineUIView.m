
#import "BaseLineUIView.h"

@interface BaseLineUIView () <F2GestureDelegate>

@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *chart;

@end

@implementation BaseLineUIView

- (instancetype)init {
    if(self = [super initWithFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, 280)]) {
        [self chartRender];
    }
    return self;
}

- (void)chartRender {
    [self addSubview:self.canvasView];
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseLine" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.scale(@"date", @{@"tickCount": @(5)});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.axis(@"date", @{
        @"grid": @(NO),
        @"label": @{
            @"item": [F2CallbackObj initWithCallback:^NSDictionary *_Nullable(NSString *_Nonnull param) {
                NSData *data = [param dataUsingEncoding:NSUTF8StringEncoding];
                NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                NSNumber *index = [json objectForKey:@"index"];
                return @{@"textColor": index.integerValue % 2 == 0 ? @"#000000" : @"#DC143C"};
            }]
        }
    });
    self.chart.axis(@"value", @{@"grid": @{@"type": @"dash", @"dash": @[@(15), @(15), @(5), @(5)]}});
    self.chart.line().position(@"date*value").fixedSize(2);
    self.chart.tooltip(@{});
    self.chart.interaction(@"pinch", @{});
    self.chart.interaction(@"pan", @{});
    self.chart.render();
}

- (F2CanvasView *)canvasView {
    if(!_canvasView) {
        _canvasView = [F2CanvasView canvasWithFrame:self.frame];
        _canvasView.delegate = self;
    }
    return _canvasView;
}

- (F2Chart *)chart {
    if(!_chart) {
        _chart = [F2Chart chart:self.canvasView.bounds.size withName:NSStringFromClass([self class])];
    }
    return _chart;
}

- (void)handleGestureInfo:(NSDictionary *)info {
    self.chart.postTouchEvent(info);
}

@end
