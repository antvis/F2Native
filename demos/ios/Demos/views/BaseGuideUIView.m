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
            @"item": [F2CallbackObj initWithCallback:^id _Nullable(NSString *_Nonnull param) {
                NSData *data = [param dataUsingEncoding:NSUTF8StringEncoding];
                NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                NSNumber *index = [json objectForKey:@"index"];
                return @{@"textColor": index.integerValue % 2 == 0 ? @"#000000" : @"#DC143C"};
            }]
        }
    });
    self.chart.axis(@"value", @{@"grid": @{@"type": @"dash", @"dash": @[@(15), @(15), @(5), @(5)]}});
    self.chart.line().position(@"date*value").fixedSize(2).attrs(@{@"connectNulls": @(YES)});
    self.chart.tooltip(@{
        @"onPress": [F2CallbackObj initWithCallback:^id _Nullable(NSString *_Nonnull param) {
            NSArray *tips = [F2Utils toJsonObject:param];
            return tips;
        }]
    });
    
    //文字
    self.chart.guide().text(@{@"position":@[@"median", @"max"], @"content": @"AntV", @"textAlign": @"center", @"textBaseline": @"top", @"textColor": @"black"});
    
    //背景
    self.chart.guide().background(@{@"color":@"#FF00001D",@"leftBottom":@[@"min", @"min"], @"rightTop":@[@"max", @(80)]});
    self.chart.guide().background(@{@"color":@"#00FF001D",@"leftBottom":@[@"min", @(80)], @"rightTop":@[@"max", @(320)]});
    
    //线
    self.chart.guide().line(@{@"position":@[@"median", @"median"], @"lineWidth": @(2), @"orientation":@"horizontal", @"color":@"#CCCCCC", @"dash": @[@(6), @(2)] , @"top":@(NO)});
    
    //图片
    self.chart.guide().image(@{@"position":@[@"median", @"max"], @"width": @30, @"height" : @(30), @"src":@"https://gw.alipayobjects.com/zos/antfincdn/FLrTNDvlna/antv.png", @"margin": @[@15, @-10]});
    
    self.chart.animate(@(NO));
    
    self.chart.render();
}

@end
