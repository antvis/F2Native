#import "F2Chart.h"
#import "F2Utils.h"
#import <F2/XChart.h>
#import <gcanvas/GCanvas.hpp>
#import <gcanvas/GCanvas2dContext.h>

@interface F2Chart ()
@property (nonatomic, assign) xg::XChart *chart;
@property (nonatomic, weak) F2CanvasView *gcanvas;
@property (nonatomic, assign) BOOL isBackground;
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
        GCanvasContext *context = ((gcanvas::GCanvas *)[self.gcanvas gcanvas])->GetGCanvasContext();
        self.chart->SetCanvasContext(context);
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

- (F2Chart * (^)(NSString *filed, NSDictionary *config))scale {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Scale([XGSafeString(field) UTF8String], nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        return self;
    };
}

- (F2Chart * (^)(NSString *filed, NSDictionary *config))axis {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Axis([XGSafeString(field) UTF8String], nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        return self;
    };
}

- (F2Chart * (^)(NSString *filed, NSDictionary *config))legend {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Legend([XGSafeString(field) UTF8String], nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))coord {
    return ^id(NSDictionary *config) {
        self.chart->Coord(nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        return self;
    };
}

- (F2Chart * (^)(NSString *type))interaction {
    return ^id(NSString *type) {
        self.chart->Interaction([XGSafeString(type) UTF8String]);
        return self;
    };
}

/// 开启 ToolTIp 功能
/// @param confg    具体字段待补充
- (F2Chart * (^)(NSDictionary *config))tooltip {
    return ^id(NSDictionary *config) {
        self.chart->Tooltip(nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        return self;
    };
}

- (F2Line * (^)(void))line {
    return ^id() {
        return [[F2Line alloc] initWithGeom:&self.chart->Line()];
    };
}

- (F2Interval * (^)(void))interval {
    return ^id() {
        return [[F2Interval alloc] initWithGeom:&self.chart->Interval()];
    };
}

- (F2Area * (^)(void))area {
    return ^id() {
        return [[F2Area alloc] initWithGeom:&self.chart->Area()];
    };
}

- (F2Guide * (^)(void))guide {
    return ^id() {
        return [[F2Guide alloc] initWithGuide:&self.chart->Guide()];
    };
}

- (F2Chart * (^)(void))render {
    return ^id() {
        self.chart->Render();
        if(!self.isBackground || UIApplication.sharedApplication.applicationState != UIApplicationStateBackground) {
            [self.gcanvas drawFrame];
            [self.gcanvas display];
        }
        return self;
    };
}

- (F2Chart * (^)(void))repaint {
    return ^id() {
        self.chart->Repaint();
        if(!self.isBackground || UIApplication.sharedApplication.applicationState != UIApplicationStateBackground) {
            [self.gcanvas drawFrame];
            [self.gcanvas display];
        }
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))postTouchEvent {
    return ^id(NSDictionary *config) {
        self.chart->OnTouchEvent(nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        [self.gcanvas drawFrame];
        [self.gcanvas display];
        return self;
    };
}

- (NSString * (^)(void))getRenderDumpInfo {
    return ^id() {
        std::string info = self.chart->GetRenderInfo();
        return [NSString stringWithCString:info.c_str() encoding:[NSString defaultCStringEncoding]];
    };
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

@end
