
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol F2GestureDelegate <NSObject>

@required
- (void)handleGestureInfo:(NSDictionary *)info;

@end

@interface F2GestureListener : NSObject <UIGestureRecognizerDelegate>

@property (nonatomic, weak) id<F2GestureDelegate> delegate;

- (instancetype)initWithView:(UIView *)view;

@end

NS_ASSUME_NONNULL_END
