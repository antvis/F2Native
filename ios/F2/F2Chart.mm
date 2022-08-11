#if defined(__APPLE__)
#import <TargetConditionals.h>
#endif

#import "F2Chart.h"
#import "F2Callback.h"
#import "F2Utils.h"
#import "XChart.h"
#import "F2CanvasView.h"
#import "F2Reflection.h"
#import "F2Reflection.h"

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

using namespace xg;

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

-(F2Chart * (^)(NSArray<NSDictionary *> *data))source {
   return ^id(NSArray *data) {
       NSArray *source = data;
       if([data isKindOfClass:NSString.class]) {
           source = [F2Utils toJsonArray:(NSString *)data];
       }
       __block XSourceArray list;
       [source enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
           XSourceItem item = F2Reflection::CreateaSourceItem(obj);
           list.push_back(std::move(item));
       }];
       self.chart->Source(list);
       return self;
   };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))scale {
    return ^id(NSString *field, NSDictionary *config) {
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(scale::ScaleCfg));
        auto cast = cfg.Cast<scale::ScaleCfg &>();
        self.chart->ScaleObject([F2SafeString(field) UTF8String], cast);
        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))axis {
    return ^id(NSString *field, NSDictionary *config) {
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(axis::AxisCfg));
        auto cast = cfg.Cast<axis::AxisCfg &>();
        self.chart->AxisObject([F2SafeString(field) UTF8String], cast);
        return self;
    };
}

- (F2Chart * (^)(NSString *field, NSDictionary *config))legend {
    return ^id(NSString *field, NSDictionary *config) {
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(legend::LegendCfg));
        auto cast = cfg.Cast<legend::LegendCfg &>();
        self.chart->LegendObject([F2SafeString(field) UTF8String], cast);
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))coord {
    return ^id(NSDictionary *config) {
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(xg::CoordCfg));
        auto cast = cfg.Cast<xg::CoordCfg &>();
        self.chart->CoordObject(cast);
        return self;
    };
}

- (F2Chart * (^)(NSString *type, NSDictionary *config))interaction {
    return ^id(NSString *type, NSDictionary *config) {
        NSDictionary *rest = [F2Utils resetCallbacksFromOld:config
                                                     host:self];
        if ([type isEqualToString:@"pan"]) {
            auto cfg = F2Reflection::CreateStruct(rest, typeof(interaction::PanCfg));
            auto cast = cfg.Cast<interaction::PanCfg &>();
            self.chart->Interaction([F2SafeString(type) UTF8String], cast);
            
            F2WeakSelf
            [self.canvasView addGestureListener:@"pan" callback:^(NSDictionary * _Nonnull info) {
                F2StrongSelf;
                strongSelf.postTouchEvent(info);
            }];
        } else if([type isEqualToString:@"pinch"]) {
            auto cfg = F2Reflection::CreateStruct(rest, typeof(interaction::PinchCfg));
            auto cast = cfg.Cast<interaction::PinchCfg &>();
            self.chart->Interaction([F2SafeString(type) UTF8String], cast);
            
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
        NSDictionary *reset = [F2Utils resetCallbacksFromOld:config host:self];
        auto cfg = F2Reflection::CreateStruct(reset, typeof(tooltip::ToolTipCfg));
        auto cast = cfg.Cast<tooltip::ToolTipCfg &>();
        self.chart->TooltipObject(cast);
        
        F2WeakSelf
        [self.canvasView addGestureListener:@"longPress" callback:^(NSDictionary * _Nonnull info) {
            F2StrongSelf;
            strongSelf.postTouchEvent(info);
        }];
        return self;
    };
}

- (F2Chart * (^)(bool config))animate {
    return ^id(bool config) {
        self.chart->AnimateObject({.enable = config});
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
            [self.canvasView setNeedsDisplay];
            BOOL drawSuccess = !!self.canvasView.canvasContext.bitmap;

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
            [self.canvasView setNeedsDisplay];
        } else {
            self.cachedRepaint = YES;
        }
        return self;
    };
}

- (F2Chart * (^)(NSDictionary *config))postTouchEvent {
    return ^id(NSDictionary *config) {
        __block event::Event event;
        event.eventType = [config[@"eventType"] UTF8String];
        NSArray *points = config[@"points"];
        [points enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            CGFloat x = [obj[@"x"] floatValue];
            CGFloat y = [obj[@"y"] floatValue];
            event.points.push_back(util::Point {x, y});
        }];
        bool changed = self.chart->OnTouchEvent(event);
        if(changed) {
            [self.canvasView setNeedsDisplay];
        }
        return self;
    };
}

- (CGPoint (^)(NSDictionary *itemData))getPosition {
    return ^CGPoint(NSDictionary *itemData) {
        auto item = F2Reflection::CreateaSourceItem(itemData);
        const xg::util::Point point =
            self.chart->GetPosition(item);

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

- (void)bindCallback:(F2Callback *)callback {
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

- (F2Chart * (^)(BOOL adjust))adjustRange {
    return ^id(BOOL adjust) {
        self.chart->AdjustRange(adjust);
        return self;
    };
}

- (F2Chart * (^)(CGSize size))changeSize {
    return ^id(CGSize size) {
        self.chart->ChangeSize(size.width, size.height);
        
        //生成了新的canvasContext
        [self.canvasView changeSize:size];
        
        //重新设置canvasContext
        self.chart->SetCanvasContext(self.canvasView.canvasContext.context2d);                
        return self;
    };
}

- (F2Chart * (^)(NSArray<NSDictionary *> *data))changeData {
    return ^id(NSArray *data) {
        NSArray *source = data;
        if([data isKindOfClass:NSString.class]) {
            source = [F2Utils toJsonArray:(NSString *)data];
        }
        __block XSourceArray list;
        [source enumerateObjectsUsingBlock:^(NSDictionary * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            XSourceItem item = F2Reflection::CreateaSourceItem(obj);
            list.push_back(std::move(item));
        }];
        self.chart->ChangeData(list);
        return self;
    };
}

@end
