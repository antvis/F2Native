#import "F2Chart.h"
#import "F2CallbackObj.h"
#import "F2Utils.h"
#import "XChart.h"
#if defined(__APPLE__)
#import <TargetConditionals.h>
#endif

@interface F2Chart ()
@property (nonatomic, assign) xg::XChart *chart;
@property (nonatomic, weak) F2CanvasView *canvasView;
@property (nonatomic, strong) RequestAnimationFrameHandle *requestAnimationFrameHandle;
@property (nonatomic, assign) BOOL isBackground;
@property (nonatomic, assign) BOOL cachedRender;
@property (nonatomic, assign) BOOL cachedRepaint;
@property (nonatomic, strong) NSMutableArray *callbackList;
@end

@implementation F2Chart

+ (F2Chart *)chart:(CGSize)size withName:(NSString *)name {
    return [[F2Chart alloc] initWithSize:size name:name];
}

- (instancetype)initWithSize:(CGSize)size name:(NSString *)name {
    if(self = [super init]) {
        _chart = new xg::XChart([F2SafeString(name) UTF8String], size.width, size.height, F2NativeScale);
        _cachedRender = NO;
        _cachedRepaint = NO;
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

- (F2Chart * (^)(F2CanvasView *canvasView))canvas {
    return ^id(F2CanvasView *canvasView) {
        self.canvasView = canvasView;
        
#if defined(TARGET_ALIPAY)
        if (canvasView.isUseCGBackend) {
            CGContextRef context = (CGContextRef)(canvasView.canvasContext.context2d);
            self.chart->SetCoreGraphicsContext(context);
        } else {
            ag::CanvasRenderingContext2D *context = (ag::CanvasRenderingContext2D *)(canvasView.canvasContext.context2d);
            self.chart->SetCanvasContext(context);
        }
#elif (TARGET_OS_MACCATALYST == 1)
        CGContextRef context = (CGContextRef)(canvasView.canvasContext.context2d);
        self.chart->SetCoreGraphicsContext(context);
#else
        CGContextRef context = (CGContextRef)(canvasView.canvasContext.context2d);
        self.chart->SetCoreGraphicsContext(context);
#endif

        self.requestAnimationFrameHandle = [RequestAnimationFrameHandle initWithF2Chart:self canvas:canvasView];
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
        self.chart->Source([F2SafeJson(json) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))scale {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Scale([F2SafeString(field) UTF8String],
                          [F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))axis {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Axis([F2SafeString(field) UTF8String],
                         [F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);

        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))legend {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Legend([F2SafeString(field) UTF8String],
                           [F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))coord {
    return ^id(NSDictionary *config) {
        self.chart->Coord([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *type, NSDictionary *config))interaction {
    return ^id(NSString *type, NSDictionary *config) {
        self.chart->Interaction([F2SafeString(type) UTF8String],
                                xg::json::ParseString([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config
                                                                                                                  host:self]]) UTF8String]));
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))tooltip {
    return ^id(NSDictionary *config) {
        self.chart->Tooltip([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
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
            self.chart->Animate([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
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

- (F2Coordinate * (^)(void))getCoord {
    return ^id() {
        return [[F2Coordinate alloc] initWithCoordinate:&self.chart->GetCoord()];
    };
}

- (F2Chart * (^)(void))render {
    return ^id() {
        if(!self.isBackground) {
            self.chart->Render();
            [self.canvasView drawFrame];
            [self.canvasView logPerformance:[NSString stringWithUTF8String:self.chart->GetChartName().c_str()]
                     renderDuration:self.chart->GetRenderDurationMM()
                        renderCount:self.chart->GetRenderCount()];
        }else {
            //标记下 待回到前台在进行绘制
            self.cachedRender = YES;
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
            self.chart->Repaint();
            [self.canvasView drawFrame];
            [self.canvasView logPerformance:[NSString stringWithUTF8String:self.chart->GetChartName().c_str()]
                     renderDuration:self.chart->GetRenderDurationMM()
                        renderCount:self.chart->GetRenderCount()];
        }else {
            self.cachedRepaint = YES;
        }
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))postTouchEvent {
    return ^id(NSDictionary *config) {
        bool changed = self.chart->OnTouchEvent([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        if(changed) {
            [self.canvasView drawFrame];
            [self.canvasView logPerformance:[NSString stringWithUTF8String:self.chart->GetChartName().c_str()]
                     renderDuration:self.chart->GetRenderDurationMM()
                        renderCount:self.chart->GetRenderCount()];
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

- (CGPoint (^)(NSDictionary *itemData))getPosition {
    return ^CGPoint(NSDictionary *itemData) {
        const xg::util::Point point =
            self.chart->GetPosition(xg::json::ParseString([F2SafeJson([F2Utils toJsonString:itemData]) UTF8String]));

        return CGPointMake(point.x / F2NativeScale, point.y / F2NativeScale);
    };
}

- (NSArray<NSDictionary *> *(^)(NSString *field))getScaleTicks {
    return ^id(NSString *field) {
        std::string info = self.chart->GetScaleTicks([F2SafeString(field) UTF8String]);
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
    
    //渲染因退到后台没有绘制的图形
    if (self.cachedRender) {
        self.cachedRender = NO;
        self.render();
    }
    
    if (self.cachedRepaint) {
        self.cachedRepaint = NO;
        self.repaint();
    }
}

- (void)bindF2CallbackObj:(F2CallbackObj *)callback {
    if(callback) {
        [self.callbackList addObject:callback];
    }
}
@end
