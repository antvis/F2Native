#import "F2Chart.h"
#import "F2CallbackObj.h"
#import "F2Utils.h"
#import "XChart.h"
#if defined(TARGET_STANDALONE)
#import <gcanvas/GCanvas.hpp>
#import <gcanvas/GCanvas2dContext.h>
#endif
#import "F2CallbackObj.h"

@interface F2Chart ()
@property (nonatomic, assign) xg::XChart *chart;
@property (nonatomic, weak) F2CanvasView *gcanvas;
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
        self.chart->Scale([XGSafeString(field) UTF8String], [XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *filed, NSDictionary *config))axis {
    return ^id(NSString *field, NSDictionary *config) {
        config = [self updateItemAttr:config WithName:@"label"];
        self.chart->Axis([XGSafeString(field) UTF8String], [XGSafeJson([F2Utils toJsonString:config]) UTF8String]);

        return self;
    };
}

- (F2Chart * (^)(NSString *filed, NSDictionary *config))legend {
    return ^id(NSString *field, NSDictionary *config) {
        self.chart->Legend([XGSafeString(field) UTF8String], [XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))coord {
    return ^id(NSDictionary *config) {
        self.chart->Coord([XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
    };
}

- (F2Chart * (^)(NSString *type, NSDictionary *config))interaction {
    return ^id(NSString *type, NSDictionary *config) {
        self.chart->Interaction([XGSafeString(type) UTF8String],
                                nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:config]) UTF8String]));
        return self;
    };
}

/// 开启 ToolTIp 功能
/// @param confg    具体字段待补充
- (F2Chart * (^)(NSDictionary *config))tooltip {
    return ^id(NSDictionary *config) {
        self.chart->Tooltip([XGSafeJson([F2Utils toJsonString:config]) UTF8String]);
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

- (F2Point * (^)(void))point {
    return ^id() {
        return [[F2Point alloc] initWithGeom:&self.chart->Point()];
    };
}

- (F2Guide * (^)(void))guide {
    return ^id() {
        return [[F2Guide alloc] initWithGuide:&self.chart->Guide()];
    };
}

- (F2Chart * (^)(void))render {
    return ^id() {
        long start = [[NSDate date] timeIntervalSince1970] * 1000;
        if(!self.isBackground) {
            self.chart->Render();
            [self.gcanvas drawFrame];
            NSString *info = self.getRenderDumpInfo();
            [self.gcanvas display:start withInfo:info];
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
        long start = [[NSDate date] timeIntervalSince1970] * 1000;
        if(!self.isBackground) {
            self.chart->Repaint();
            [self.gcanvas drawFrame];
            NSString *info = self.getRenderDumpInfo();
            [self.gcanvas display:start withInfo:info];
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
            [self.gcanvas display:start withInfo:info];
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
            self.chart->GetPosition(nlohmann::json::parse([XGSafeJson([F2Utils toJsonString:itemData]) UTF8String]));
        return [NSArray arrayWithObjects:@(point.x), @(point.y), nil];
    };
}

#pragma mark private
- (NSDictionary *)updateItemAttr:(NSDictionary *)config WithName:(NSString *)name {
    if(name && [config objectForKey:name]) {

        NSDictionary *attr = [config objectForKey:name];
        if([attr isKindOfClass:[NSDictionary class]] && [attr objectForKey:@"item"] &&
           [[attr objectForKey:@"item"] isKindOfClass:[F2CallbackObj class]]) {
            NSMutableDictionary *mutableConfig = [NSMutableDictionary dictionaryWithDictionary:attr];
            F2CallbackObj *callbackObj = (F2CallbackObj *)[attr objectForKey:@"item"];
            [mutableConfig setObject:callbackObj.key forKey:@"item"];
            [self.callbackList addObject:callbackObj];
            NSDictionary *attrConf = [mutableConfig copy];
            NSMutableDictionary *m_dic = [NSMutableDictionary dictionaryWithDictionary:config];
            [m_dic setObject:attrConf forKey:name];
            return [m_dic copy];
        }
    }
    return config;
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

@end
