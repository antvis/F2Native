#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "F2GestureListener.h"
#import "F2CanvasContext.h"

NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasView : UIView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                        andBizId:(NSString *)bizId
                        complete:(void (^)(F2CanvasView *))callback;

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                        cgBackend:(BOOL)isUseCGBackend
                        andBizId:(NSString *)bizId
                        complete:(void (^)(F2CanvasView *))callback;


@property (nonatomic, weak) id<F2GestureDelegate> delegate;

@property (nonatomic, strong, readonly) F2CanvasContext *canvasContext;

@property (nonatomic, strong, readonly) F2GestureListener *listener;

@property (nonatomic, copy, readonly) NSString *bizId;

@property (nonatomic, assign, readonly) BOOL isUseCGBackend;

/// 使用 GCanvas实现的canvas渲染
- (void)drawFrame;

/// destory canvas release ssource
- (void)destroy;

/// 转换成bitmap
- (UIImage *)snapshot;

- (void)logPerformance:(NSString *)chartName
        renderDuration:(NSTimeInterval)duration
           renderCount:(NSInteger)count;
@end

NS_ASSUME_NONNULL_END
