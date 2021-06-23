
#import "BaseLineUIView.h"

@interface BaseLineUIView () <F2GestureDelegate>

@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *chart;
@property (nonatomic, assign) CGSize chartSize;
@end

@implementation BaseLineUIView {
    CGRect _frame;
}

- (instancetype)init {
    if(self = [super initWithFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, 280)]) {
        F2CanvasThread *cthread = [[F2CanvasThread alloc] initWithAsync:YES];
        cthread.name = [self name];
        __weak __typeof(self) weakSelf = self;
        self.chartSize = self.frame.size;
        F2CanvasView *view = [F2CanvasView canvasWithFrame:self.frame
                                                  andBizId:[self name]
                                                 andThread:nil
                                                  complete:^(F2CanvasView *view) {
                                                      __strong __typeof(weakSelf) strongSelf = weakSelf;
                                                      [strongSelf render];
                                                  }];

        self.canvasView = view;
        self.canvasView.delegate = self;
        [self addSubview:self.canvasView];
    }
    return self;
}

- (void)render {
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
    NSLog(@"== 开始 == 线程：%@", [NSThread currentThread]);
    [self chartRender];
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
    NSLog(@"== 结束 == 线程：%@ ; 耗时：%f", [NSThread currentThread], (end - start) * 1000);
}

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseLine" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.clear();
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.scale(@"date", @{@"tickCount": @(5)});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.axis(@"date", @{
        @"grid": @(NO),
        @"label": @{
            @"item": [F2CallbackObj initWithCallback:^id _Nullable(NSString *_Nonnull param) {
                NSData *data = [param dataUsingEncoding:NSUTF8StringEncoding];
                NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                NSNumber *index = [json objectForKey:@"index"];
                return @{@"textColor": index.integerValue % 2 == 0 ? @"#000000" : @"#DC143C"};
            }]
        }
    });
    self.chart.axis(@"value", @{@"grid": @{@"type": @"dash", @"dash": @[@(15), @(15), @(5), @(5)]}});
    self.chart.line().position(@"date*value").fixedSize(2);
    self.chart.tooltip(@{
        @"onPress": [F2CallbackObj initWithCallback:^id _Nullable(NSString *_Nonnull param) {
            NSArray *tips = [F2Utils toJsonObject:param];
            return tips;
        }]
    });
    self.chart.animate(@(YES));
    self.chart.interaction(@"pinch", @{});
    self.chart.interaction(@"pan", @{});
    self.chart.render();
}

- (void)dealloc {
    [self.canvasView destroy];
}

- (F2Chart *)chart {
    if(!_chart) {
        _chart = [F2Chart chart:self.chartSize withName:NSStringFromClass([self class])];
    }
    return _chart;
}

- (void)handleGestureInfo:(NSDictionary *)info {
    self.chart.postTouchEvent(info);
}

- (NSString *)name {
    return NSStringFromClass([self class]);
}

@end
