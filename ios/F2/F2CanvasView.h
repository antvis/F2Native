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
/// @return YES 渲染成功 NO 渲染失败
- (BOOL)drawFrame;

///检测view是否是白屏, 方法首先会截屏，其次通过像素来分析
///@return UIImage说明是白屏 nil 说明不是白屏
- (UIImage *)detectView;

/// destory canvas release ssource
- (void)destroy;

/// 转换成bitmap
/// @return UIImage
- (UIImage *)snapshot;


/// @param chartId 图表的uuid，作为参数上报
/// @param duration 渲染时间
/// @param renderSuccess c++渲染是否成功
/// @param renderCmdCount c++侧渲染的指令
/// @param drawSuccess native侧上屏是否成功
/// @param snapshot  白屏的图片，非白屏的时候为nil
/// @param cost 截屏&分析白屏的时间消耗
- (void)logPerformance:(NSString *)chartId
        renderDuration:(NSTimeInterval)duration
         renderSuccess:(BOOL)renderSuccess
        renderCmdCount:(NSInteger)renderCmdCount
           drawSuccess:(BOOL)drawSuccess
              snapshot:(nullable UIImage *)snapshot
          snapshotCost:(NSTimeInterval)cost;
@end

NS_ASSUME_NONNULL_END
