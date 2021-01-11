
#import "F2GestureListener.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasView : UIView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame;

@property (nonatomic, weak) id<F2GestureDelegate> delegate;

- (void)drawFrame;

/// render canvas
- (void)display:(long)startTime  withInfo:(NSString *)info;

/// get c++ canvas obj
- (void *)gcanvas;

@end

NS_ASSUME_NONNULL_END
