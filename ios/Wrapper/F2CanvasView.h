#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "F2CanvasThread.h"
#import "F2GestureListener.h"
#import "F2Canvas.h"


NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasView : UIView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                        andBizId:(NSString *)bizId
                        complete:(void (^)(F2CanvasView *))callback;


+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                         andBizId:(NSString *)bizId
                        andThread:(F2CanvasThread *)thread
                         complete:(void (^)(F2CanvasView *))callback;

@property (nonatomic, weak) id<F2GestureDelegate> delegate;

@property (nonatomic, strong, readonly) F2Canvas *canvas;

@property (nonatomic, strong, readonly) F2GestureListener *listener;

@property (nonatomic, strong, readonly) F2CanvasThread *canvasThread;

/// render canvas
- (void)drawFrame;

/// destory canvas release ssource
- (void)destroy;

/// get c++ canvas  context obj
- (void *)context;

@end

NS_ASSUME_NONNULL_END
