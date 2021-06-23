
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN
@class F2GestureListener;
@protocol F2GestureDelegate <NSObject>

@optional
- (BOOL)handleGestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer listener:(F2GestureListener*)listener;

- (BOOL)handleGestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
                       listener:(F2GestureListener*)listener;

@required
- (void)handleGestureInfo:(NSDictionary *)info;

@end

@interface F2GestureListener : NSObject <UIGestureRecognizerDelegate>

@property (nonatomic, weak) id<F2GestureDelegate> delegate;
@property (nonatomic, strong) UILongPressGestureRecognizer *longGesture;
@property (nonatomic, strong) UIPanGestureRecognizer *panGesture;
@property (nonatomic, strong) UIPinchGestureRecognizer *pinchGesture;
@property (nonatomic, strong) UITapGestureRecognizer *tapGesture;
@property (nonatomic, strong) UITapGestureRecognizer *doubleTapGesture;

- (instancetype)initWithView:(UIView *)view;

@end

NS_ASSUME_NONNULL_END
