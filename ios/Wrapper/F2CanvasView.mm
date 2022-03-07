#import "F2CanvasView.h"
#import "F2Utils.h"
#import "F2CoreGraphicsCanvasContext.h"
#import "F2AlipayCanvasContext.h"

#if defined(TARGET_ALIPAY)
#import "XGEventLogger.h"
#import <APConfig/APConfigService.h>
#import <APMobileFramework/DTContext.h>
#import <AntUI/AntUI.h>
#endif

@interface F2CanvasView () <F2GestureDelegate>
@property (nonatomic, strong) F2CanvasContext *canvasContext;
@property (nonatomic, strong) F2GestureListener *listener;
@property (nonatomic, assign, readwrite) BOOL isUseCGBackend;
@property (nonatomic, copy, readwrite) NSString *bizId;
@end

@implementation F2CanvasView

//NativeCanvas上线开关
+(NSString *)backendTypeStr {
#if defined(TARGET_ALIPAY)
    static NSString *backendStr;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        id<APConfigService> configService = [DTContextGet() findServiceByName:@"APConfigService"];
        backendStr = [configService stringValueForKey:@"kF2NativeCanvasBackendKey"];
    });
    return backendStr;
#endif
    
    return @"true";
}

+(NSString *)toastStr {
#if defined(TARGET_ALIPAY)
    static NSString *toastStr;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        id<APConfigService> configService = [DTContextGet() findServiceByName:@"APConfigService"];
        toastStr = [configService stringValueForKey:@"kF2NativeCanvasBackendToast"];
    });
    return toastStr;
#endif
    
    return @"false";
}

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                         andBizId:(NSString *)bizId
                         complete:(void (^)(F2CanvasView *))callback {
    bool cgBackend = [[F2CanvasView backendTypeStr] isEqualToString:@"true"];
    return [F2CanvasView canvasWithFrame:frame cgBackend:cgBackend andBizId:bizId complete:callback];
    
}

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                        cgBackend:(BOOL)isUseCGBackend
                        andBizId:(NSString *)bizId
                         complete:(void (^)(F2CanvasView *))callback {
    F2CanvasView *view = [[F2CanvasView alloc] initWithFrame:frame];
    [view createCanvasWithFrame:frame cgBackend:isUseCGBackend andBizId:bizId complete:callback];
    return view;
}

- (void)createCanvasWithFrame:(CGRect)frame
                    cgBackend:(BOOL)isUseCGBackend
                          andBizId:(NSString *)bizId
                          complete:(void (^)(F2CanvasView *))callback{
    self.isUseCGBackend = isUseCGBackend;
    self.bizId = bizId;
#if defined(TARGET_ALIPAY)
    [XGEventLogger logGraph:bizId];
    if (self.isUseCGBackend) {
        self.canvasContext = [F2CoreGraphicsCanvasContext new];
        
        //方便测试 后续随老canvas一起下线
        if ([[F2CanvasView toastStr] isEqualToString:@"true"]) {
            [AUToast presentToastWithin:[UIApplication sharedApplication].keyWindow.rootViewController.view
                               withIcon:AUToastIconNone
                                   text:@"新Canvas绘制"
                               duration:0.4
                                 logTag:@"F2Native"];
        }
        
    }else {
        self.canvasContext = [F2AlipayCanvasContext new];
    }
#endif
#if defined(TARGET_STANDALONE)
    self.canvasContext = [F2CoreGraphicsCanvasContext new];
#endif
    F2WeakSelf;
    [self.canvasContext createContextWithFrame:frame complete:^(F2CanvasContext * _Nonnull canvas) {
        F2StrongSelf;
        if (!strongSelf) {
            return;
        }
        [strongSelf addSubview:canvas.contextView];
        if (callback) {
            callback(strongSelf);
        }
    }];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if(self = [super initWithFrame:frame]) {
        self.listener = [[F2GestureListener alloc] initWithView:self];
        self.listener.delegate = self;
        self.userInteractionEnabled = YES;
        self.multipleTouchEnabled = YES;
    }
    return self;
}

- (void)drawFrame {
    [self.canvasContext draw];
}

- (void)dealloc {
    [_canvasContext destroy];
}

- (void)destroy {
    [self.canvasContext destroy];
    self.canvasContext = nil;
}

- (UIImage *)snapshot {
    return self.canvasContext.snapshot;
}

- (void)handleGestureInfo:(nonnull NSDictionary *)info sender:(nonnull UIGestureRecognizer *)gestureRecognizer{
    if(info && [self.delegate respondsToSelector:@selector(handleGestureInfo:sender:)]) {
        [self.delegate handleGestureInfo:info sender:gestureRecognizer];
    }
}

- (void)logPerformance:(NSString *)chartName
        renderDuration:(NSTimeInterval)duration
           renderCount:(NSInteger)count {
#if defined(TARGET_ALIPAY)
    [XGEventLogger log:@"1010183" extParams:@{
        @"xg_tag" : self.bizId ? : @"",
        @"xg_duration": [NSString stringWithFormat:@"%.2f", duration],
        @"xg_index": @"1",
        @"xg_type": @"f2chart",
        @"xg_cmds": [NSString stringWithFormat:@"%ld", (long)count],
        @"xg_size": @"0",
        @"xg_width": [NSString stringWithFormat:@"%f",self.frame.size.width],
        @"xg_height": [NSString stringWithFormat:@"%f",self.frame.size.height],
        @"xg_scale": [NSString stringWithFormat:@"%f", F2NativeScale],
        @"xg_color": @"normal",
        @"xg_render": self.isUseCGBackend ? @"coregraphics" : @"antg",
        @"xg_jsengine": @"jscore"
    }];
#endif
    NSLog(@"%@#Performance renderCount %ld renderDuration %.0f ms",chartName, (long)count, duration);
}

- (BOOL)handleGestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer listener:(F2GestureListener*) listener{
    if ([self.delegate respondsToSelector:@selector(handleGestureRecognizerShouldBegin:listener:)]) {
        return [self.delegate handleGestureRecognizerShouldBegin:gestureRecognizer listener:listener];
    }
    return YES;
}


- (BOOL)handleGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
listener:(F2GestureListener *)listener {
    if ([self.delegate respondsToSelector:@selector(handleGestureRecognizer:shouldRecognizeSimultaneouslyWithGestureRecognizer:listener:)]) {
        return [self.delegate handleGestureRecognizer:gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:otherGestureRecognizer listener:listener];
    }
    return YES;
}
@end
