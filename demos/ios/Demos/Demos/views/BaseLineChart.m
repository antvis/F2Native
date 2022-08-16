
#import "BaseLineChart.h"

@interface BaseLineChart ()

@property (nonatomic, strong) F2CanvasView *canvasView;
@property (nonatomic, strong) F2Chart *chart;
@end

@implementation BaseLineChart {
    CGRect _frame;
}

- (instancetype)init {
    return [self initWithFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, 280)];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if(self = [super initWithFrame:frame]) {
        F2CanvasView *view = [[F2CanvasView alloc] initWithFrame:frame];
        view.backgroundColor = [UIColor clearColor];
        self.canvasView = view;
        [self chartRender];
        [self addSubview:self.canvasView];
    }
    return self;
}

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseLine" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    NSArray *jsonArray = [F2Utils toJsonArray:jsonData];
    self.chart.clear();
    self.chart.canvas(self.canvasView).padding(10, 10, 10, 10.f).source(jsonArray);
    self.chart.scale(@"date", @{@"tickCount": @(3)});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.axis(@"date", @{
        @"grid": @(NO),
        @"label": @{           
            @"item": [F2Callback callback:^NSDictionary *_Nonnull(NSDictionary *_Nonnull param) {
                NSInteger index = [[param objectForKey:@"index"] integerValue];
                NSArray *textAlign = @[@"start", @"center", @"end"];                
                return @{@"textColor": index % 2 == 0 ? @"#000000" : @"#DC143C", @"textAlign": textAlign[index]};
            }]
        }
    });
    self.chart.axis(@"value", @{@"grid": @{@"type": @"dash", @"dash": @[@(15), @(15), @(5), @(5)]}});
    self.chart.line().position(@"date*value").fixedSize(2);

    self.chart.tooltip(@{});
    
    self.chart.guide().background(@{@"color":@"#FF0000", @"opacity":@0.2, @"leftBottom":@[@"min", @"min"], @"rightTop":@[@"max", @(80)]});
    self.chart.guide().background(@{@"color":@"#00FF00",@"opacity":@0.2, @"leftBottom":@[@"min", @(80)], @"rightTop":@[@"max", @(320)]});
    
    self.chart.animate(YES);
    
    self.chart.render();
}

- (F2Chart *)chart {
    if(!_chart) {
        _chart = [F2Chart chart:self.canvasView.frame.size name:NSStringFromClass([self class])];
    }
    return _chart;
}

- (NSString *)name {
    return NSStringFromClass([self class]);
}

@end
