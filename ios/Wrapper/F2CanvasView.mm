#import "F2CanvasView.h"
#import "F2CanvasThread.h"
#import "F2Utils.h"

@interface F2CanvasView () <F2GestureDelegate>
@property (nonatomic, strong) F2Canvas *canvas;
@property (nonatomic, strong) F2GestureListener *listener;
@property (nonatomic, strong) F2CanvasThread *canvasThread;
@end

@implementation F2CanvasView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                         andBizId:(NSString *)bizId
                         complete:(void (^)(F2CanvasView *))callback {
    F2CanvasView *view = [[F2CanvasView alloc] initWithFrame:frame];
    [view createCanvascanvasWithFrame:frame andBizId:bizId complete:callback];
    return view;
}

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                         andBizId:(NSString *)bizId
                        andThread:(F2CanvasThread *)thread
                         complete:(void (^)(F2CanvasView *))callback {
    
    F2CanvasView *view = [[F2CanvasView alloc] initWithFrame:frame];
    __weak __typeof(view) weakView = view;
    if (thread) {
        view.canvasThread =thread;
        __strong __typeof(weakView) strongView = weakView;
        [view.canvasThread performBlockASyncOnThread:^{
            [strongView createCanvascanvasWithFrame:frame andBizId:bizId complete:callback];
        }];
    }else{
        [view createCanvascanvasWithFrame:frame andBizId:bizId complete:callback];
    }
    return view;
}


- (void)createCanvascanvasWithFrame:(CGRect)frame
                          andBizId:(NSString *)bizId
                          complete:(void (^)(F2CanvasView *))callback{
    self.canvas = [F2Canvas new];
    WeakSelf;
    [self.canvas createCanvascanvasWithFrame:frame andBizId:bizId complete:^(F2Canvas * _Nonnull canvas) {
        StrongSelf;
        if (!strongSelf) {
            return;
        }
        
        if([NSThread isMainThread]) {
               [strongSelf addSubview:canvas.canvasView];
           }else {
               dispatch_async(dispatch_get_main_queue(), ^{
                   [strongSelf addSubview:canvas.canvasView];
               });
           }
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
    if (self.canvasThread && [NSThread currentThread] != self.canvasThread.myThread) {
        F2Canvas *canvas = self.canvas;
        [self.canvasThread performBlockASyncOnThread:^{
            [canvas draw];
        }];
    }else{
        [self.canvas draw];
    }
}

- (void)dealloc {

}

- (void)destroy {
    if (self.canvasThread && [NSThread currentThread] != self.canvasThread.myThread) {
        F2Canvas *canvas = self.canvas;
        [self.canvasThread performBlockASyncOnThread:^{
            [canvas destroy];
        }];
    }else {
        [self.canvas destroy];
    }
    self.canvas = nil;
}

- (void *)context {
    return [self.canvas context2d];
}

- (void)handleGestureInfo:(nonnull NSDictionary *)info sender:(nonnull UIGestureRecognizer *)gestureRecognizer{
    if(info && [self.delegate respondsToSelector:@selector(handleGestureInfo:sender:)]) {
        [self.delegate handleGestureInfo:info sender:gestureRecognizer];
    }
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
