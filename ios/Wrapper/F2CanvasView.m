#import "F2CanvasView.h"
#import "F2GLESEnv.h"
#import <gcanvas/GCanvas.hpp>
#import <gcanvas/GCanvas2dContext.h>
#import <gcanvas/GCanvasManager.h>

@interface F2CanvasView () <F2GestureDelegate>

@property (nonatomic, assign) gcanvas::GCanvas *canvas;
@property (nonatomic, assign) GCanvasContext *context2d;
@property (nonatomic, strong) F2GLESEnv *env;
@property (nonatomic, strong) F2GestureListener *listener;

@end

@implementation F2CanvasView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame {
    return [[F2CanvasView alloc] initWithFrame:frame];
}

- (void)dealloc {
    [self destroy];
}

- (void)destroy {
    [self destroyCanvas];
    [self.env destroyContext];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if(self = [super initWithFrame:frame]) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties =
            @{ kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8,
               kEAGLDrawablePropertyRetainedBacking: @(YES) };
        eaglLayer.contentsScale = UIScreen.mainScreen.nativeScale;
        CGFloat scale = UIScreen.mainScreen.nativeScale;
        CGRect bufferFrame =
            CGRectMake(frame.origin.x * scale, frame.origin.y * scale, frame.size.width * scale, frame.size.height * scale);
        _env = [[F2GLESEnv alloc] init:self.layer];
        [self createCanvas:bufferFrame];
        self.listener = [[F2GestureListener alloc] initWithView:self];
        self.listener.delegate = self;
        self.userInteractionEnabled = YES;
        self.multipleTouchEnabled = YES;
    }
    return self;
}

- (void)createCanvas:(CGRect)frame {
    static NSInteger canvasId = 1;
    GCanvasConfig config;
    gcanvas::GCanvasManager *manager = gcanvas::GCanvasManager::GetManager();
    self.canvas = manager->NewCanvas(std::to_string(canvasId++), config);
    self.canvas->CreateContext();
    self.context2d = self.canvas->GetGCanvasContext();
    // gcanvas中按px计算
    self.context2d->SetDevicePixelRatio(1.0f);
    self.canvas->OnSurfaceChanged(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    self.context2d->ClearScreen();
}

- (void)destroyCanvas {
    gcanvas::GCanvasManager *manager = gcanvas::GCanvasManager::GetManager();
    manager->RemoveCanvas(self.canvas->mContextId);
}

- (void *)gcanvas {
    return self.canvas;
}

- (void)drawFrame {
    self.canvas->drawFrame();
}

- (void)display {
    [self.env swapBuffers];
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)handleGestureInfo:(nonnull NSDictionary *)info {
    if(info && [self.delegate respondsToSelector:@selector(handleGestureInfo:)]) {
        [self.delegate handleGestureInfo:info];
    }
}

@end
