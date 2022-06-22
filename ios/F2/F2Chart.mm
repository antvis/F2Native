#import "F2Chart.h"
#import "F2Callback.h"
#import "F2Utils.h"
#import "XChart.h"
#if defined(__APPLE__)
#import <TargetConditionals.h>
#endif

typedef const char *(*selector)(void *caller, const char *functionId, const char *parameter);
const char *cexecute(void *caller, const char *functionId, const char *parameter) {
    F2Chart *chart = (__bridge F2Chart *)caller;
    return [chart execute:functionId param:parameter]; }

namespace xg {
namespace func {
class IOSF2Function : public func::F2Function {
  public:
    IOSF2Function(void *_handle, selector _call) : func::F2Function(), handle_(_handle), call_(_call) {}
    
    const std::string Execute(const std::string &functionId, const std::string &param) override {
        return std::string(call_(handle_,functionId.data(),  param.data()));    
    }
    
    ~IOSF2Function() override {
        handle_ = nullptr;
        call_ = nullptr;
    }

  private:
    void *handle_;
    selector call_;
};
} // namespace func
}

@interface F2Chart ()
@property (nonatomic, weak) F2CanvasView *canvasView;
@property (nonatomic, assign) xg::XChart *chart;
@property (nonatomic, assign) xg::func::IOSF2Function *innerCallback;
@property (nonatomic, assign) BOOL isBackground;
@property (nonatomic, assign) BOOL cachedRender;
@property (nonatomic, assign) BOOL cachedRepaint;
@property (nonatomic, strong) F2RequestAnimationFrameHandle *requestAnimationFrameHandle;
@property (nonatomic, strong) NSMutableDictionary<NSString *, F2Callback *> *callbackList;
@property (nonatomic, copy) FunctionItemCallback outCallback;
@end

@implementation F2Chart

+ (F2Chart *)chart:(CGSize)size name:(NSString *)name {
    return [[F2Chart alloc] initWithSize:size name:name];
}

- (instancetype)initWithSize:(CGSize)size name:(NSString *)name {
    if(self = [super init]) {
        _chart = new xg::XChart([F2SafeString(name) UTF8String], size.width, size.height, F2NativeScale);
        
       //注册统一的回调函数
        _innerCallback = new xg::func::IOSF2Function((__bridge void *)self, cexecute);
        _chart->SetInvokeFunction(_innerCallback);
        
        //初始化回调函数容器
        _callbackList = [NSMutableDictionary dictionary];
        
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
    
    if (_innerCallback) {
        delete _innerCallback;
        _innerCallback = nil;
    }
    
    [self removeSystemNotification];
}

- (const char *)execute:(const char *)functionId param:(const char *)param {
    if (!functionId || strlen(functionId) == 0) {
        return "{}";
    }
    NSString *functionIdStr = [NSString stringWithUTF8String:functionId];
    NSString *paramStr = [NSString stringWithUTF8String:param];
    NSDictionary *paramDic = [F2Utils toJsonObject:paramStr];
    //动画的回调函数
    if ([functionIdStr isEqualToString:self.requestAnimationFrameHandle.functionId]) {
        NSDictionary *rstDic = [self.requestAnimationFrameHandle execute:paramDic];
        return rstDic ? [[F2Utils toJsonString:rstDic] UTF8String] : "{}";
    }
    //native的回调
    else {
        //根据functionId来分发
        F2Callback *callback = [self.callbackList objectForKey:functionIdStr];
        if (callback) {
            NSDictionary *rstDic = [callback execute:paramDic];
            return rstDic ? [[F2Utils toJsonString:rstDic] UTF8String] : "{}";
        }//外部的钩子
        else if (self.outCallback) {
            NSDictionary *rstDic = self.outCallback(functionIdStr, paramDic);
            return rstDic ? [[F2Utils toJsonString:rstDic] UTF8String] : "{}";
        } else {
            return "{}";
        }
    }
}

- (F2Chart * (^)(F2CanvasView *canvasView))canvas {
    return ^id(F2CanvasView *canvasView) {
        self.canvasView = canvasView;
        CGContextRef context = (CGContextRef)(canvasView.canvasContext.context2d);
        self.chart->SetCanvasContext(context);

        self.requestAnimationFrameHandle = [F2RequestAnimationFrameHandle initWithF2Chart:self canvas:canvasView];
        self.chart->SetRequestFrameFuncId([self.requestAnimationFrameHandle.functionId UTF8String]);
        
        [self.callbackList setObject:self.requestAnimationFrameHandle forKey:self.requestAnimationFrameHandle.functionId];
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

- (F2Chart * (^)(NSArray *data))source {
    return ^id(NSArray *data) {
        if ([data isKindOfClass:NSArray.class]) {
            self.chart->Source([F2SafeJson([F2Utils toJsonString:data]) UTF8String]);
        }else if([data isKindOfClass:NSString.class]) {
            NSString *dataStr = (NSString *)data;
            self.chart->Source([F2SafeJson(dataStr) UTF8String]);
        }        
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
                                [F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config
                                                                                                                  host:self]]) UTF8String]);
        if ([type isEqualToString:@"pan"]) {
            F2WeakSelf
            [self.canvasView addGestureListener:@"pan" callback:^(NSDictionary * _Nonnull info) {
                F2StrongSelf;
                strongSelf.postTouchEvent(info);
            }];
        } else if([type isEqualToString:@"pinch"]) {
            F2WeakSelf
            [self.canvasView addGestureListener:@"pinch" callback:^(NSDictionary * _Nonnull info) {
                F2StrongSelf;
                strongSelf.postTouchEvent(info);
            }];
        }
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))tooltip {
    return ^id(NSDictionary *config) {
        self.chart->Tooltip([F2SafeJson([F2Utils toJsonString:[F2Utils resetCallbacksFromOld:config host:self]]) UTF8String]);
        F2WeakSelf
        [self.canvasView addGestureListener:@"longPress" callback:^(NSDictionary * _Nonnull info) {
            F2StrongSelf;
            strongSelf.postTouchEvent(info);
        }];
        return self;
    };
}

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
            NSString *chartId = [NSString stringWithUTF8String:self.chart->GetChartId().c_str()];

            // 1.渲染是否成功
            bool renderSuccess = self.chart->Render();

            // 2.获取渲染指令的个数
            long renderCmdCount = self.chart->GetRenderCmdCount();

            // 3.上屏是否成功
            BOOL drawSuccess = [self.canvasView drawFrame];

            // 4.截bitmapview 分析
            long long start = xg::CurrentTimestampAtMM();
            UIImage *whiteView = [self.canvasView detectView];
            long long detectCost = (xg::CurrentTimestampAtMM() - start);

            long renderDuration = self.chart->GetRenderDurationMM();

            // 5.上报性能数据
            [self.canvasView logPerformance:chartId
                             renderDuration:renderDuration
                              renderSuccess:renderSuccess
                             renderCmdCount:renderCmdCount
                                drawSuccess:drawSuccess
                                   snapshot:whiteView
                               snapshotCost:detectCost];
        } else {
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
        } else {
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
        }
        return self;
    };
}

- (CGPoint (^)(NSDictionary *itemData))getPosition {
    return ^CGPoint(NSDictionary *itemData) {
        const xg::util::Point point =
            self.chart->GetPosition(xg::json::ParseString([F2SafeJson([F2Utils toJsonString:itemData]) UTF8String]));

        return CGPointMake(point.x / F2NativeScale, point.y / F2NativeScale);
    };
}

- (NSArray<NSDictionary *> * (^)(NSString *field))getScaleTicks {
    return ^id(NSString *field) {
        std::string info = self.chart->GetScaleTicks([F2SafeString(field) UTF8String]);
        NSString *jsonStr = [NSString stringWithCString:info.c_str() encoding:[NSString defaultCStringEncoding]];
        return [F2Utils toJsonObject:jsonStr];
    };
}

- (F2Chart * (^)(NSDictionary *config))config {
    return ^id(NSDictionary *config) {
        self.chart->Parse([F2SafeJson([F2Utils toJsonString:config]) UTF8String]);
        return self;
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

    //渲染因退到后台没有绘制的图形
    if(self.cachedRender) {
        self.cachedRender = NO;
        self.render();
    }

    if(self.cachedRepaint) {
        self.cachedRepaint = NO;
        self.repaint();
    }
}

- (void)bindF2CallbackObj:(F2Callback *)callback {
    if(callback) {
        [self.callbackList setObject:callback forKey:callback.functionId];
    }
}

- (F2Chart * (^)(FunctionItemCallback callback))callback {
    return ^id(FunctionItemCallback callback) {
        self.outCallback = callback;
        return self;
    };
}

- (F2Chart * (^)(BOOL adjust))adjustScale {
    return ^id(BOOL adjust) {
        self.chart->AdjustScale(adjust);
        return self;
    };
}

- (F2Chart * (^)(BOOL sync))syncYScale {
    return ^id(BOOL sync) {
        self.chart->SyncYScale(sync);
        return self;
    };
}

@end
