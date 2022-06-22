#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "F2GestureListener.h"
#import "F2CanvasContext.h"

NS_ASSUME_NONNULL_BEGIN

@interface F2CanvasView : UIView

/// 画布的上下文
@property (nonatomic, strong, readonly) F2CanvasContext *canvasContext;

/// 业务的id
@property (nonatomic, copy) NSString *bizId;

/// 初始化view
/// @param frame 视图大小
+ (instancetype)canvas:(CGRect)frame;

/// 不要使用init方法初始化
- (instancetype)init NS_UNAVAILABLE;

/// 使用 GCanvas实现的canvas渲染
/// @return YES 渲染成功 NO 渲染失败
- (BOOL)drawFrame;

///检测view是否是白屏, 方法首先会截屏，其次通过像素来分析
///@return UIImage说明是白屏 nil 说明不是白屏
- (UIImage *)detectView;

/// 转换成bitmap
/// @return UIImage
- (UIImage *)snapshot;

/// 增加手势监听
/// @param type   longpress, pinching, pan
/// @param callback callback
- (void)addGestureListener:(NSString *)type callback:(F2GestureCallback)callback;

/// 按类型删除手势
/// @param type longPress, pinching, pan
- (void)removeGestureListener:(NSString *)type;

/// 删除所有手势
- (void)removeAllGestureListeners;

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
