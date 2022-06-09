
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^F2GestureCallback)(NSDictionary *_Nonnull info);

@interface F2GestureListener : NSObject <UIGestureRecognizerDelegate>
@property (nonatomic, strong) UILongPressGestureRecognizer *longGesture;
@property (nonatomic, strong) UIPanGestureRecognizer *panGesture;
@property (nonatomic, strong) UIPinchGestureRecognizer *pinchGesture;
@property (nonatomic, copy) F2GestureCallback longGestureCallback;
@property (nonatomic, copy) F2GestureCallback panGestureCallback;
@property (nonatomic, copy) F2GestureCallback pinchGestureCallback;

- (instancetype)initWithView:(UIView *)view;

- (void)addLongPressGesture:(F2GestureCallback)callback;
- (void)addPinchGesture:(F2GestureCallback)callback;
- (void)addPanGesture:(F2GestureCallback)callback;

- (void)removeLongPressGesture;
- (void)removePinchGesture;
- (void)removePanGesture;

- (void)removeAllGestures;

@end

NS_ASSUME_NONNULL_END
