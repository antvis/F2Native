#import "F2Chart.h"
#import "F2CallbackObj.h"
#import "F2Utils.h"
#import "XChart.h"
#if defined(TARGET_STANDALONE)
#import <gcanvas/GCanvas.hpp>
#import <gcanvas/GCanvas2dContext.h>
#endif

@interface F2Chart ()
@property (nonatomic, assign) xg::XChart *chart;
@property (nonatomic, weak) F2CanvasView *gcanvas;
@property (nonatomic, strong) RequestAnimationFrameHandle *requestAnimationFrameHandle;
@property (nonatomic, assign) BOOL isBackground;
@property (nonatomic, strong) NSMutableArray *callbackList;
@end

@implementation F2Chart

+ (F2Chart *)chart:(CGSize)size withName:(NSString *)name {
    return [[F2Chart alloc] initWithSize:size name:name];
}

- (instancetype)initWithSize:(CGSize)size name:(NSString *)name {
    if(self = [super init]) {
        _chart = new xg::XChart([XGSafeString(name) UTF8String], size.width, size.height, [UIScreen mainScreen].nativeScale);
    }
    [self addSystemNotification];
    return self;
}

- (void)dealloc {
    if(_chart) {
        delete _chart;
        _chart = nil;
    }
    [self removeSystemNotification];
}

- (F2Chart * (^)(F2CanvasView *gcanvas))canvas {
    return ^id(F2CanvasView *gcanvas) {
        self.gcanvas = gcanvas;

#if defined(TARGET_ALIPAY)
        ag::CanvasRenderingContext2D *context = (ag::CanvasRenderingContext2D *)[self.gcanvas getContext];
#else
        GCanvasContext *context = ((gcanvas::GCanvas *)[self.gcanvas gcanvas])->GetGCanvasContext();

#endif
        self.chart->SetCanvasContext(context);

        self.requestAnimationFrameHandle = [RequestAnimationFrameHandle initWithF2Chart:self canvas:gcanvas];
        self.chart->SetRequestFrameFuncId([self.requestAnimationFrameHandle.key UTF8String]);

        return self;
    };
}

- (F2Chart * (^)(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom))padding {
    return ^id(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom) {
        self.chart->Padding(left, top, right, bottom);
        return self;
    };
}

- (F2Chart * (^)(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom))margin {
    return ^id(CGFloat left, CGFloat top, CGFloat right, CGFloat bottom) {
        self.chart->Margin(left, top, right, bottom);
        return self;
    };
}

- (F2Chart * (^)(NSString *json))source {
    return ^id(NSString *json) {
        self.chart->Source([XGSafeJson(json) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))scale {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Scale([XGSafeString(field) UTF8String],
                          [XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))axis {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Axis([XGSafeString(field) UTF8String],
                         [XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);

        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))legend {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Legend([XGSafeString(field) UTF8String],
                           [XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))coord {
    return ^id(NSDictionary *config) {
        self.chart->Coord([XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *type, NSDictionary *config))interaction {
    return ^id(NSString *type, NSDictionary *config) {
        self.chart->Interaction([XGSafeString(type) UTF8String],
                                xg::json::ParseString([XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config
                                                                                                                  host:self]]) UTF8String]));
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))tooltip {
    return ^id(NSDictionary *config) {
        self.chart->Tooltip([XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

/// 配置动画功能
/// @param confg    具体字段待补充
- (F2Chart * (^)(id config))animate {
    return ^id(id config) {
        if([config isKindOfClass:[NSNumber class]]) {
            self.chart->Animate([config boolValue] ? "true" : "false");
        } else if([config isKindOfClass:[NSDictionary class]]) {
            self.chart->Animate([XGSafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        }
        return self;
    };
}

- (F2Line * (^)(void))line {
    return ^id() {
        return [[F2Line alloc] initWithGeom:&self.chart->Line() withOwner:self];
    };
}

- (F2Interval * (^)(void))interval {
    return ^id() {
        return [[F2Interval alloc] initWithGeom:&self.chart->Interval() withOwner:self];
    };
}

- (F2Area * (^)(void))area {
    return ^id() {
        return [[F2Area alloc] initWithGeom:&self.chart->Area() withOwner:self];
    };
}

- (F2Point * (^)(void))point {
    return ^id() {
        return [[F2Point alloc] initWithGeom:&self.chart->Point() withOwner:self];
    };
}

- (F2Candle * (^)(void))candle {
    return ^id() {
        return [[F2Candle alloc] initWithGeom:&self.chart->Candle() withOwner:self];
    };
}

- (F2Guide * (^)(void))guide {
    return ^id() {
        return [[F2Guide alloc] initWithGuide:&self.chart->Guide()];
    };
}

- (F2Chart * (^)(void))render {
    return ^id() {
        if(!self.isBackground) {
            NSTimeInterval start = [[NSDate date] timeIntervalSince1970] * 1000;
            self.chart->Render();
            [self.gcanvas drawFrame];
            NSString *info = self.getRenderDumpInfo();
            NSTimeInterval end = [[NSDate date] timeIntervalSince1970] * 1000;
            [self.gcanvas logPerformance:end - start withInfo:info];
        }
        return self;
    };
}

- (F2Chart * (^)(void))clear {
    return ^id() {
        self.chart->Clear();
        return self;
    };
}

- (F2Chart * (^)(void))repaint {
    return ^id() {
        if(!self.isBackground) {
            NSTimeInterval start = [[NSDate date] timeIntervalSince1970] * 1000;
            self.chart->Repaint();
            [self.gcanvas drawFrame];
            NSString *info = self.getRenderDumpInfo();
            NSTimeInterval end = [[NSDate date] timeIntervalSince1970] * 1000;
            [self.gcanvas logPerformance:end - start withInfo:info];
        }
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))postTouchEvent {
    return ^id(NSDictionary *config) {
        long start = [[NSDate date] timeIntervalSince1970] * 1000;
        bool changed = self.chart->OnTouchEvent([XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
        if(changed) {
            [self.gcanvas drawFrame];
            NSString *info = self.getRenderDumpInfo();
            NSTimeInterval end = [[NSDate date] timeIntervalSince1970] * 1000;
            [self.gcanvas logPerformance:end - start withInfo:info];
        }
        return self;
    };
}

- (NSString * (^)(void))getRenderDumpInfo {
    return ^id() {
        std::string info = self.chart->GetRenderInfo();
        return [NSString stringWithCString:info.c_str() encoding:[NSString defaultCStringEncoding]];
    };
}

- (NSArray<NSNumber *> * (^)(NSDictionary *itemData))getPosition {
    return ^id(NSDictionary *itemData) {
        const xg::util::Point point =
            self.chart->GetPosition(xg::json::ParseString([XGSafeJson([F2Utils toJsonString:itemData]) UTF8String]));

        return [NSArray arrayWithObjects:@(point.x), @(point.y), nil];
    };
}

- (NSArray<NSDictionary *> *(^)(NSString *field))getScaleTicks {
    return ^id(NSString *field) {
        std::string info = self.chart->GetScaleTicks([XGSafeString(field) UTF8String]);
        NSString *jsonStr = [NSString stringWithCString:info.c_str() encoding:[NSString defaultCStringEncoding]];
        return [F2Utils toJsonObject:jsonStr];
    };
}

- (NSMutableArray *)callbackList {
    if(!_callbackList) {
        _callbackList = [[NSMutableArray alloc] init];
    }
    return _callbackList;
}

#pragma mark Notification
- (void)addSystemNotification {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appDidEnterBackground)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appWillEnterForeground)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
}

- (void)removeSystemNotification {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillEnterForegroundNotification object:nil];
}

- (void)appDidEnterBackground {
    _isBackground = YES;
}

- (void)appWillEnterForeground {
    _isBackground = NO;
    [self render];
}

- (void)bindF2CallbackObj:(F2CallbackObj *)callback {
    if(callback) {
        [self.callbackList addObject:callback];
    }
}
@end
