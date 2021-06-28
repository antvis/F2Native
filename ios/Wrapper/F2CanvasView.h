
#import "F2CanvasThread.h"
#import "F2GestureListener.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasView : UIView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame;

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                         andBizId:(NSString *)bizId
                        andThread:(F2CanvasThread *)thread
                         complete:(void (^)(F2CanvasView *))callback;

@property (nonatomic, weak) id<F2GestureDelegate> delegate;
@property (nonatomic, strong,readonly) F2CanvasThread *canvasThread;

///render frame
- (void)drawFrame;

/// get c++ canvas obj
- (void *)gcanvas;

/// release canvas and f2 chart resources
- (void)destroy;

///上报监控信息
- (void)logPerformance:(NSTimeInterval)duration withInfo:(NSString *)info;

@end

NS_ASSUME_NONNULL_END
