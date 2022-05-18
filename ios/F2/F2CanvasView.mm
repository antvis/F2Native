#import "F2CanvasView.h"
#import "F2AlipayCanvasContext.h"
#import "F2CoreGraphicsCanvasContext.h"
#import "F2Utils.h"
#import "xtime.h"

#if defined(TARGET_ALIPAY)
#import "F2CSUtil.h"
#import "XGEventLogger.h"
#endif

@interface F2CanvasView () <F2GestureDelegate>
@property (nonatomic, strong) F2CanvasContext *canvasContext;
@property (nonatomic, strong) F2GestureListener *listener;
@property (nonatomic, assign, readwrite) BOOL isUseCGBackend;
@property (nonatomic, copy, readwrite) NSString *bizId;
@end

@implementation F2CanvasView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame andBizId:(NSString *)bizId complete:(void (^)(F2CanvasView *))callback {
    return [F2CanvasView canvasWithFrame:frame cgBackend:YES andBizId:bizId complete:callback];
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
                     complete:(void (^)(F2CanvasView *))callback {
    self.isUseCGBackend = isUseCGBackend;
    self.bizId = bizId;
#if defined(TARGET_ALIPAY)
    [XGEventLogger logGraph:bizId engine:self.isUseCGBackend ? @"coregraphics" : @"antg"];
    if(self.isUseCGBackend) {
        self.canvasContext = [F2CoreGraphicsCanvasContext new];
        //方便测试 后续随老canvas一起下线
        [F2CSUtil toast:@"F2Native新Canvas绘制, 只在预发弹屏"];
    } else {
        self.canvasContext = [F2AlipayCanvasContext new];
    }
#endif
#if defined(TARGET_STANDALONE)
    self.canvasContext = [F2CoreGraphicsCanvasContext new];
#endif
    F2WeakSelf;
    [self.canvasContext createContextWithFrame:frame
                                      complete:^(F2CanvasContext *_Nonnull canvas) {
                                          F2StrongSelf;
                                          if(!strongSelf) {
                                              return;
                                          }
                                          [strongSelf addSubview:canvas.contextView];
                                          if(callback) {
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

- (BOOL)drawFrame {
    return [self.canvasContext draw];
}

- (UIImage *)detectView {
#if defined(TARGET_ALIPAY)
    UIImage *image = self.snapshot;
    return [F2CSUtil isPureColor:image] ? image : nil;
#endif
    return nil;
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

- (void)handleGestureInfo:(nonnull NSDictionary *)info sender:(nonnull UIGestureRecognizer *)gestureRecognizer {
    if(info && [self.delegate respondsToSelector:@selector(handleGestureInfo:sender:)]) {
        [self.delegate handleGestureInfo:info sender:gestureRecognizer];
    }
}

- (void)logPerformance:(NSString *)chartId
        renderDuration:(NSTimeInterval)duration
         renderSuccess:(BOOL)renderSuccess
        renderCmdCount:(NSInteger)renderCmdCount
           drawSuccess:(BOOL)drawSuccess
              snapshot:(nullable UIImage *)snapshot
          snapshotCost:(NSTimeInterval)cost {
#if defined(TARGET_ALIPAY)
    [F2CSUtil logPerformance:chartId
                       bizId:self.bizId
                    viewSize:self.frame.size
                      engine:self.isUseCGBackend ? @"coregraphics" : @"antg"
              renderDuration:duration
               renderSuccess:renderSuccess
              renderCmdCount:renderCmdCount
                 drawSuccess:drawSuccess
                    snapshot:snapshot
                snapshotCost:cost];
#endif
}

- (BOOL)handleGestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer listener:(F2GestureListener *)listener {
    if([self.delegate respondsToSelector:@selector(handleGestureRecognizerShouldBegin:listener:)]) {
        return [self.delegate handleGestureRecognizerShouldBegin:gestureRecognizer listener:listener];
    }
    return YES;
}

- (BOOL)handleGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
    shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
                                              listener:(F2GestureListener *)listener {
    if([self.delegate respondsToSelector:@selector(handleGestureRecognizer:
                                             shouldRecognizeSimultaneouslyWithGestureRecognizer:listener:)]) {
        return [self.delegate handleGestureRecognizer:gestureRecognizer
            shouldRecognizeSimultaneouslyWithGestureRecognizer:otherGestureRecognizer
                                                      listener:listener];
    }
    return YES;
}
@end
