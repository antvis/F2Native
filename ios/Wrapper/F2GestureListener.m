

const double LONG_GESTURE_MINI_DURATION = 0.25f;

#import "F2GestureListener.h"

@interface F2GestureListener ()

@property (nonatomic, strong) UILongPressGestureRecognizer *longGesture;
@property (nonatomic, strong) UIPanGestureRecognizer *panGesture;
@property (nonatomic, strong) UIPinchGestureRecognizer *pinchGesture;
@property (nonatomic, strong) UITapGestureRecognizer *tapGesture;
@property (nonatomic, strong) UITapGestureRecognizer *doubleTapGesture;
@property (nonatomic, weak) UIView *view;
@property (nonatomic, strong) UIGestureRecognizer *currentGesture;

@end

@implementation F2GestureListener

- (instancetype)initWithView:(UIView *)view {
    if(self = [super init]) {
        _view = view;
        [self initGesture];
    }
    return self;
}

- (void)initGesture {
    [self addLongPressGesture];
    [self addPinchingGesture];
    [self addPanningGesture];
}

- (void)gestureAction:(id)sender {
    NSString *eventType = [self eventType:sender];
    if(!eventType) {
        return;
    }
    CGFloat scale = [UIScreen mainScreen].scale;
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
    if([self.delegate respondsToSelector:@selector(handleGestureInfo:)]) {
        [self.delegate handleGestureInfo:event];
    }
}

- (NSString *)eventType:(id)sender {
    UIGestureRecognizer *gesture = (UIGestureRecognizer *)sender;
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

- (void)addLongPressGesture {
    UILongPressGestureRecognizer *longGesture =
        [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    longGesture.minimumPressDuration = LONG_GESTURE_MINI_DURATION;
    [_view addGestureRecognizer:longGesture];
    longGesture.delegate = self;
    self.longGesture = longGesture;
}

- (void)addPinchingGesture {
    UIPinchGestureRecognizer *pinchGesture =
        [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    [_view addGestureRecognizer:pinchGesture];
    pinchGesture.delegate = self;
    self.pinchGesture = pinchGesture;
}

- (void)addPanningGesture {
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    panGesture.minimumNumberOfTouches = 1;
    panGesture.maximumNumberOfTouches = 1;
    [_view addGestureRecognizer:panGesture];
    panGesture.delegate = self;
    self.panGesture = panGesture;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
    shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}

@end
