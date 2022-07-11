#import "F2GestureListener.h"
#import "F2Utils.h"

const double LONG_GESTURE_MINI_DURATION = 0.25f;

@interface F2GestureListener ()
@property (nonatomic, weak) UIView *view;
@property (nonatomic, strong) UIGestureRecognizer *currentGesture;
@property (nonatomic, copy) NSDictionary *event;
@end

@implementation F2GestureListener

- (instancetype)initWithView:(UIView *)view {
    if(self = [super init]) {
        _view = view;
        _view.userInteractionEnabled = YES;
        _view.multipleTouchEnabled = YES;
    }
    return self;
}

- (void)gestureAction:(id)sender {
    NSString *eventType = [self eventType:sender];
    if(!eventType) {
        return;
    }
    CGFloat scale = [UIScreen mainScreen].nativeScale;
    UIGestureRecognizer *gesture = (UIGestureRecognizer *)sender;
    NSArray *points = nil;
    if(gesture.numberOfTouches == 2) {
        CGPoint locationOne = [gesture locationOfTouch:0 inView:_view];
        NSDictionary *pointOne = @{ @"x": @(locationOne.x * scale), @"y": @(locationOne.y * scale) };
        CGPoint locationTwo = [gesture locationOfTouch:1 inView:_view];
        NSDictionary *pointTwo = @{ @"x": @(locationTwo.x * scale), @"y": @(locationTwo.y * scale) };
        points = @[pointOne, pointTwo];
    } else {
        CGPoint location = [gesture locationInView:_view];
        NSDictionary *point = @{ @"x": @(location.x * scale), @"y": @(location.y * scale) };
        points = @[point];
    }
    NSDictionary *event = @{ @"eventType": eventType, @"points": points };
    if([sender isKindOfClass:UILongPressGestureRecognizer.class]) {
        F2SafeBlockRun(self.longGestureCallback, event);
    } else if([sender isKindOfClass:UIPanGestureRecognizer.class]) {
        F2SafeBlockRun(self.panGestureCallback, event);
    } else if([sender isKindOfClass:UIPinchGestureRecognizer.class]) {
        F2SafeBlockRun(self.pinchGestureCallback, event);
    }
    [self supplementAction:event sender:sender];
}

//对齐android的长按手势
//250ms后补发一个touchmove事件,使得F2Native能判断出是长按手势
//F2 PRESS_DELAY = 250ms
-(void)supplementAction:(NSDictionary *)event sender:(id)sender  {
    self.event = event;
    F2WeakSelf;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(250 * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
        F2StrongSelf;
        NSString *eventType = [strongSelf.event objectForKey:@"eventType"];
        if ([eventType isEqualToString:@"touchstart"]) {
            NSArray *points = [strongSelf.event objectForKey:@"points"];
            F2SafeBlockRun(strongSelf.longGestureCallback, @{@"eventType": @"touchmove", @"points": points ? : @[]});
        }
    });
}

- (NSString *)eventType:(id)sender {
    UIGestureRecognizer *gesture = (UIGestureRecognizer *)sender;
    // once
    if ([gesture isKindOfClass:[UITapGestureRecognizer class]]) {
        return @"tap";
    }
    // continue
    switch(gesture.state) {
        case UIGestureRecognizerStateBegan:
            if(self.currentGesture) {
                return nil;
            }
            self.currentGesture = sender;
            return @"touchstart";
        case UIGestureRecognizerStateChanged:
            return @"touchmove";
        case UIGestureRecognizerStateEnded:
            self.currentGesture = nil;
            return @"touchend";
        case UIGestureRecognizerStateCancelled:
            self.currentGesture = nil;
            return @"touchcancel";
        default:
            return nil;
            break;
    }
}

#pragma mark - initGesture

- (void)addLongPressGesture:(F2GestureCallback)callback {
    UILongPressGestureRecognizer *longGesture =
        [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    longGesture.minimumPressDuration = LONG_GESTURE_MINI_DURATION;
    [_view addGestureRecognizer:longGesture];
    longGesture.delegate = self;
    self.longGesture = longGesture;
    self.longGestureCallback = callback;
}

- (void)addPinchGesture:(F2GestureCallback)callback {
    UIPinchGestureRecognizer *pinchGesture =
        [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    [_view addGestureRecognizer:pinchGesture];
    pinchGesture.delegate = self;
    self.pinchGesture = pinchGesture;
    self.pinchGestureCallback = callback;
}

- (void)addPanGesture:(F2GestureCallback)callback {
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    panGesture.minimumNumberOfTouches = 1;
    panGesture.maximumNumberOfTouches = 1;
    [_view addGestureRecognizer:panGesture];
    panGesture.delegate = self;
    self.panGesture = panGesture;
    self.panGestureCallback = callback;
}

- (void)removeLongPressGesture {
    [_view removeGestureRecognizer:self.longGesture];
}

- (void)removePinchGesture {
    [_view removeGestureRecognizer:self.pinchGesture];
}

- (void)removePanGesture {
    [_view removeGestureRecognizer:self.panGesture];
}

- (void)removeAllGestures {
    for(UIGestureRecognizer *gesture in _view.gestureRecognizers) {
        [_view removeGestureRecognizer:gesture];
    }
}
@end
