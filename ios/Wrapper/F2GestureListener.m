

const double LONG_GESTURE_MINI_DURATION = 0.0f;

#import "F2GestureListener.h"
#import "F2Utils.h"

@interface F2GestureListener ()
@property (nonatomic, weak) UIView *view;
@property (nonatomic, strong) UIGestureRecognizer *currentGesture;
@property (nonatomic, copy) NSDictionary *event;
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
    [self addTapGesture];
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
    [self supplementAction:event];
}

//对齐android的长按手势
//250ms后补发一个touchmove事件,使得F2Native能判断出是长按手势
//F2 PRESS_DELAY = 250ms
-(void)supplementAction:(NSDictionary *)event {
    self.event = event;
    WeakSelf;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(250 * NSEC_PER_MSEC)), dispatch_get_main_queue(), ^{
        StrongSelf;
        NSString *eventType = [strongSelf.event objectForKey:@"eventType"];
        if ([eventType isEqualToString:@"touchstart"]) {
            NSArray *points = [strongSelf.event objectForKey:@"points"];
            [strongSelf.delegate handleGestureInfo:@{@"eventType": @"touchmove", @"points": points ? : @[]}];
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


-(void)addTapGesture {
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer  alloc] initWithTarget:self action:@selector(gestureAction:)];
    [_view addGestureRecognizer:tapGesture];
    tapGesture.delegate = self;
    self.tapGesture = tapGesture;
}

- (void)addPanningGesture {
    UIPanGestureRecognizer *panGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(gestureAction:)];
    panGesture.minimumNumberOfTouches = 1;
    panGesture.maximumNumberOfTouches = 1;
    [_view addGestureRecognizer:panGesture];
    panGesture.delegate = self;
    self.panGesture = panGesture;
}

- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer {
    if ([self.delegate respondsToSelector:@selector(handleGestureRecognizerShouldBegin:listener:)]) {
        return [self.delegate handleGestureRecognizerShouldBegin:gestureRecognizer listener:self];
    }
    return YES;
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    if ([self.delegate respondsToSelector:@selector(handleGestureRecognizer:shouldRecognizeSimultaneouslyWithGestureRecognizer:listener:)]) {
        return [self.delegate handleGestureRecognizer:gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:otherGestureRecognizer listener:self];
    }
    return YES;
}
@end
