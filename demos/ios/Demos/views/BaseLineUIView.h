
#import <UIKit/UIKit.h>
#import <F2/F2.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseLineUIView : UIView

@property (nonatomic, strong, readonly) F2CanvasView *canvasView;
@property (nonatomic, strong, readonly) F2Chart *chart;

@end

NS_ASSUME_NONNULL_END
