
#import <UIKit/UIKit.h>
#import <F2/F2.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseLineUIView : UIView

@property (nonatomic, strong, readonly) F2CanvasView *canvasView;
@property (nonatomic, strong, readonly) F2Chart *chart;
@property (nonatomic, assign, readonly) CGSize chartSize;

@end

NS_ASSUME_NONNULL_END
