//
//  F2AlipayCanvasContext.m
//  F2
//
//  Created by weiqing.twq on 2021/12/8.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#if defined(TARGET_ALIPAY)
#import "F2AlipayCanvasContext.h"
#import <AntCanvas/AntCanvas.h>
#import "XGEventLogger.h"
#import "F2Utils.h"
#import "xtime.h"

@interface F2AlipayCanvasContext()
@property (nonatomic, strong) UIView *canvasView;
@property (nonatomic, strong) AntCanvas *canvas;
@end

@implementation F2AlipayCanvasContext

- (void)createContextWithFrame:(CGRect)frame complete:(void (^)(F2CanvasContext *))callback {
    
    long long start = xg::CurrentTimestampAtMM();
    NSString *UUID = [[NSUUID UUID] UUIDString];
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    AntCanvasConfig *config = [[AntCanvasConfig alloc] init];
    config.isolateId = UUID;
    config.canvasId = UUID;
    config.canvasSize = CGSizeMake(frame.size.width *scale , frame.size.height *scale);
    config.frame = frame;
    config.isOnscreen = YES;
    config.canvasBizType = @"F2Native";
    config.canvasBizId = @"F2Native";
    config.backendType = BackendTypeSkia;    
    
    F2WeakSelf;
    [AntCanvas createCanvasWithConfig:config complete:^(AntCanvas * _Nonnull antCanvas) {
        F2StrongSelf;
        if (strongSelf) {
            NSLog(@"F2AlipayCanvasContext#Create AntGraphics Context cost %lld ms", (xg::CurrentTimestampAtMM() - start));
            strongSelf.canvas = antCanvas;
            if(callback) callback(strongSelf);
        }else {
            APLogInfo(@"F2Canvas", @"F2Canvas was destroyed");
            [antCanvas destroy];
        }
    }];
}

-(void)draw {
    if([self.canvas getCanvasElement]) {
        [self.canvas getCanvasElement]->swap();
    }
}

-(void)destroy {
    if (!self.canvas) {
        return;
    }
    [self.canvas destroy];
    self.canvas = nil;
}

-(void *)context2d {
    return (void*)[self.canvas getContext2D];
}

-(UIView *)contextView {
    return [self.canvas getCanvasView];
}

@end
#endif
