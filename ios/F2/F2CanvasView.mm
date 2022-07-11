#import "F2CanvasView.h"
#import "F2CanvasContext.h"
#import "F2Utils.h"
#import "xtime.h"

#if defined(TARGET_ALIPAY)
#import "F2CSUtil.h"
#import "XGEventLogger.h"
#endif

@interface F2CanvasView ()
@property (nonatomic, strong) F2CanvasContext *canvasContext;
@property (nonatomic, strong) F2GestureListener *listener;
@end

@implementation F2CanvasView

+ (instancetype)canvas:(CGRect)frame {
    return [[F2CanvasView alloc] initWithFrame:frame];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if(self = [super initWithFrame:frame]) {
        self.canvasContext = [[F2CanvasContext alloc] initWithFrame:frame];
        self.listener = [[F2GestureListener alloc] initWithView:self];
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    if(self.canvasContext.bitmap) {
        CGContextRef ctx = UIGraphicsGetCurrentContext();
        CGContextDrawImage(ctx, rect, self.canvasContext.bitmap);
    }
}

- (UIImage *)detectView {
#if defined(TARGET_ALIPAY)
    UIImage *image = self.snapshot;
    return [F2CSUtil isPureColor:image] ? image : nil;
#endif
    return nil;
}

- (UIImage *)snapshot {
    return [UIImage imageWithCGImage:self.canvasContext.bitmap];
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
                      engine:@"coregraphics"
              renderDuration:duration
               renderSuccess:renderSuccess
              renderCmdCount:renderCmdCount
                 drawSuccess:drawSuccess
                    snapshot:snapshot
                snapshotCost:cost];
#endif
}

- (void)addGestureListener:(NSString *)type callback:(F2GestureCallback)callback {
    if([type isEqualToString:@"longPress"]) {
        [self.listener addLongPressGesture:callback];
    } else if([type isEqualToString:@"pinch"]) {
        [self.listener addPinchGesture:callback];
    } else if([type isEqualToString:@"pan"]) {
        [self.listener addPanGesture:callback];
    }
}

- (void)removeGestureListener:(NSString *)type {
    if([type isEqualToString:@"longPress"]) {
        [self.listener removeLongPressGesture];
    } else if([type isEqualToString:@"pinch"]) {
        [self.listener removePinchGesture];
    } else if([type isEqualToString:@"pan"]) {
        [self.listener removePanGesture];
    }
}

- (void)removeAllGestureListeners {
    [self.listener removeAllGestures];
}


@end
