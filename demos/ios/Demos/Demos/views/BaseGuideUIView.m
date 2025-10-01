#import "BaseGuideUIView.h"

@implementation BaseGuideUIView;

- (void)chartRender {
    NSString *jsonPath = [[NSBundle mainBundle] pathForResource:@"Res/mockData_baseLine" ofType:@"json"];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    self.chart.clear();
    self.chart.canvas(self.canvasView).padding(20, 10, 20, 0.f).source(jsonData);
    self.chart.scale(@"date", @{@"tickCount": @(3)});
    self.chart.scale(@"value", @{@"nice": @(YES)});
    self.chart.axis(@"date", @{
        @"grid": @(NO),
        @"label": @{
            @"labelOffset": @(5),
            @"textAlign": @[@"start", @"center", @"end"],
            @"item": [F2Callback callback:^NSDictionary *_Nonnull(NSDictionary *_Nonnull param) {
                NSNumber *index = [param objectForKey:@"index"];
                return @{@"textColor": index.integerValue % 2 == 0 ? @"#000000" : @"#DC143C"};
            }]
        }
    });
    self.chart.axis(@"value", @{@"grid": @{@"type": @"dash", @"dash": @[@(15), @(15), @(5), @(5)]}});
    self.chart.line().position(@"date*value").fixedSize(2).attrs(@{@"connectNulls": @(YES)});
    self.chart.tooltip(@{
        @"onPress": [F2Callback callback:^NSDictionary *_Nonnull(NSDictionary *_Nonnull param) {
            //可以修改param中的参数，然后按原来的格式返回回去
            NSArray *tips = [param objectForKey:@"tooltip"];
            NSLog(@"tips %lu", (unsigned long)tips.count);
            return param;
        }]
    });
    //文字
    self.chart.guide().text(@{@"position":@[@"min", @"max"],
                              @"content": @"测试",
                              @"textAlign": @"center",
                              @"font": @{
                                  @"fontWeight":@"Bold",
                                  @"fontFamily":@"Helvetica-Bold"
                              },
                              @"textBaseline": @"top",
                              @"textColor": @"black"});

    //文字
    self.chart.guide().text(@{@"position":@[@"median", @"max"],
                              @"content": @"测试",
                              @"textAlign": @"center",
                              @"font": @{
                                  @"fontWeight":@"normal",
                                  @"fontFamily":@"PingFangSC-Medium"
                              },
                              @"textBaseline": @"top",
                              @"textColor": @"black"});
    //文字
    self.chart.guide().text(@{@"position":@[@"max", @"max"],
                              @"content": @"测试",
                              @"textAlign": @"center",
                              @"font": @{
                                  @"fontWeight":@"800",
                                  @"fontFamily":@"PingFangSC-Regular"
                              },
                              @"textBaseline": @"top",
                              @"textColor": @"black"});
    
    //背景
    self.chart.guide().background(@{@"color":@"#FF00001D",@"leftBottom":@[@"min", @"min"], @"rightTop":@[@"max", @(80)]});
    self.chart.guide().background(@{@"color":@"#00FF001D",@"leftBottom":@[@"min", @(80)], @"rightTop":@[@"max", @(320)]});
    
    //线
    self.chart.guide().line(@{@"position":@[@"median", @"median"], @"lineWidth": @(2), @"orientation":@"horizontal", @"color":@"#CCCCCC", @"dash": @[@(6), @(2)] , @"top":@(NO)});
    //线
    self.chart.guide().line(@{@"position":@[@"median", @"median"], @"lineWidth": @(2), @"orientation":@"vertical", @"color":@"#CCCCCC", @"dash": @[@(6), @(2)] , @"top":@(NO)});
    
    //图片
    self.chart.guide().image(@{@"position":@[@"2017-06-20", @206], @"width": @10, @"height" : @10, @"src":@"https://gw.alipayobjects.com/zos/antfincdn/FLrTNDvlna/antv.png", @"margin": @[@0, @0]});

    self.chart.animate(@(NO));
    
    self.chart.render();
}

@end
