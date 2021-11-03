//
//  F2Canvas.m
//  AFWFinanceChart
//
//  Created by weiqing.twq on 2021/7/5.
//  Copyright © 2021 Alipay. All rights reserved.
//

#import "F2Canvas.h"
#import "F2Utils.h"

#if defined(PRODUCT_WALLET)
#import <AntCanvas/AntCanvas.h>
#import <APConfig/APConfig.h>
#import <APMobileFramework/DTContext.h>
#import "XGEventLogger.h"

@interface F2Canvas()
@property (nonatomic, strong) NSString *canvasBizId;
@property (nonatomic, assign) CGSize chartSize;
@property (nonatomic, strong) AntCanvas *canvas;
@end

@implementation F2Canvas

+(NSString *)backendTypeStr {
    static NSString *backendStr;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        id<APConfigService> configService = [DTContextGet() findServiceByName:@"APConfigService"];
        backendStr = [configService stringValueForKey:@"kF2NativeSkiaBackendKey"];
    });
    return backendStr;
}

- (void)createCanvascanvasWithFrame:(CGRect)frame
                          andBizId:(NSString *)bizId
                           complete:(void (^)(F2Canvas *))callback {
    self.canvasBizId = bizId;
    self.chartSize = frame.size;
    NSString *UUID = [[NSUUID UUID] UUIDString];
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    AntCanvasConfig *config = [[AntCanvasConfig alloc] init];
    config.isolateId = UUID;
    config.canvasId = UUID;
    config.canvasSize = CGSizeMake(frame.size.width *scale , frame.size.height *scale);
    config.frame = frame;
    config.isOnscreen = YES;
    config.canvasBizType = @"F2Native";
    config.canvasBizId = bizId;
    
    [XGEventLogger logGraph:bizId];
    
    if ([[F2Canvas backendTypeStr] isEqualToString:@"skia"]) {
        config.backendType = BackendTypeSkia;
    }else {
        config.backendType = BackendTypeGCanvas;
    }
    
    WeakSelf;
    [AntCanvas createCanvasWithConfig:config complete:^(AntCanvas * _Nonnull antCanvas) {
        StrongSelf;
        if (strongSelf) {
            strongSelf.canvas = antCanvas;
            callback(strongSelf);
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
    [self.canvas destroy];
}

-(void *)context2d {
    return [self.canvas getContext2D];
}

-(UIView *)canvasView {
    return [self.canvas getCanvasView];
}

- (void)logPerformance:(NSTimeInterval)duration withInfo:(NSString *)info {
    NSDictionary *dic = [F2Utils toJsonObject:info];
    NSString *color = @"noraml";
    NSInteger cmdCount = [dic integerForKey_ap:@"cmdCount"];
    [XGEventLogger logPerformance:self.canvasBizId viewSize:self.chartSize duration:[NSString stringWithFormat:@"%.2f", duration] color:color cmdCount:cmdCount];
}

@end


#else

#import <gcanvas/GCanvas.hpp>
#import <gcanvas/GCanvas2dContext.h>
#import <gcanvas/GCanvasManager.h>
#import "F2GLESEnv.h"

@interface F2GLESView : UIView

@end

@implementation F2GLESView

- (instancetype)initWithFrame:(CGRect)frame {
    if(self = [super initWithFrame:frame]) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties =
            @{kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8, kEAGLDrawablePropertyRetainedBacking: @(YES)};
        eaglLayer.contentsScale = UIScreen.mainScreen.nativeScale;
        self.userInteractionEnabled = YES;
        self.multipleTouchEnabled = YES;
    }
    return self;
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

@end

@interface F2Canvas()
@property (nonatomic, strong) UIView *canvasView;
@property (nonatomic, strong) NSString *canvasBizId;
@property (nonatomic, assign) CGSize chartSize;
@property (nonatomic, assign) gcanvas::GCanvas *gcanvas;
@property (nonatomic, assign) GCanvasContext *gcontext2d;
@property (nonatomic, strong) F2GLESEnv *env;
@end

@implementation F2Canvas

- (void)createCanvascanvasWithFrame:(CGRect)frame
                          andBizId:(NSString *)bizId
                           complete:(void (^)(F2Canvas *))callback {
    self.canvasBizId = bizId;
    self.chartSize = frame.size;
    self.canvasView = [[F2GLESView alloc] initWithFrame:frame];
    self.env = [[F2GLESEnv alloc] init:self.canvasView.layer];
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    CGRect bufferFrame = CGRectMake(self.canvasView.frame.origin.x * scale, self.canvasView.frame.origin.y * scale, self.canvasView.frame.size.width * scale, self.canvasView.frame.size.height * scale);
    [self createCanvas:bufferFrame];
 
    if(callback) {
        callback(self);
    }
}

- (void)createCanvas:(CGRect)frame {
    static NSInteger canvasId = 1;
    GCanvasConfig config;
    config.useFbo = true;
    config.flip = false;
    config.sharedFont = false;
    config.sharedShader = false;
    gcanvas::GCanvasManager *manager = gcanvas::GCanvasManager::GetManager();
    self.gcanvas = manager->NewCanvas(std::to_string(canvasId++), config);
    self.gcanvas->CreateContext();
    self.gcontext2d = self.gcanvas->GetGCanvasContext();
    self.gcontext2d->SetFontManager(GFontManager::NewInstance());
    // gcanvas中按px计算
    self.gcontext2d->SetDevicePixelRatio(1.0f);
    self.gcanvas->OnSurfaceChanged(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    self.gcontext2d->ClearScreen();
}

-(void)draw {
    self.gcanvas->drawFrame();
    [self.env swapBuffers];
}

-(void)destroy {
    gcanvas::GCanvasManager *manager = gcanvas::GCanvasManager::GetManager();
    manager->RemoveCanvas(self.gcanvas->mContextId);
    self.gcanvas = nil;
    [self.env destroyContext];
}

-(void *)context2d {
    return self.gcontext2d;
}

- (void)logPerformance:(NSTimeInterval)duration withInfo:(NSString *)info {
    NSLog(@"logPerformance render cost %f ms",duration);
}

@end

#endif


