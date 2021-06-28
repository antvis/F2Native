#import "F2CanvasView.h"
#import "F2CanvasThread.h"
#import "F2GLESEnv.h"
#import "F2Utils.h"
#import <gcanvas/GCanvas.hpp>
#import <gcanvas/GCanvas2dContext.h>
#import <gcanvas/GCanvasManager.h>

@interface F2CanvasView () <F2GestureDelegate>

@property (nonatomic, assign) gcanvas::GCanvas *canvas;
@property (nonatomic, assign) GCanvasContext *context2d;
@property (nonatomic, strong) F2GLESEnv *env;
@property (nonatomic, strong) F2GestureListener *listener;
@property (nonatomic, strong) F2CanvasThread *canvasThread;

@end

@implementation F2CanvasView

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame {
    F2CanvasView *view = [[F2CanvasView alloc] initWithFrame:frame];
    view.canvasThread = [[F2CanvasThread alloc] initWithAsync:false];
    view.env = [[F2GLESEnv alloc] init:view.layer];
    CGFloat scale = UIScreen.mainScreen.nativeScale;
    CGRect bufferFrame = CGRectMake(view.frame.origin.x * scale, view.frame.origin.y * scale, view.frame.size.width * scale,
                                    view.frame.size.height * scale);
    [view createCanvas:bufferFrame];
    return view;
}

+ (F2CanvasView *)canvasWithFrame:(CGRect)frame
                         andBizId:(NSString *)bizId
                        andThread:(F2CanvasThread *)thread
                         complete:(void (^)(F2CanvasView *))callback {

    F2CanvasView *view = [[F2CanvasView alloc] initWithFrame:frame];
    if(!thread) {
        view.canvasThread = [[F2CanvasThread alloc] initWithAsync:false];
    } else {
        view.canvasThread = thread;
    }

    CGFloat scale = UIScreen.mainScreen.nativeScale;
    CGRect bufferFrame = CGRectMake(view.frame.origin.x * scale, view.frame.origin.y * scale, view.frame.size.width * scale,
                                    view.frame.size.height * scale);

    __weak __typeof(view) weakView = view;
    CALayer *layer = view.layer;
    [view
        runBlockOnExecuteThread:^{
            __strong __typeof(weakView) strongView = weakView;
            strongView.env = [[F2GLESEnv alloc] init:layer];
            [strongView createCanvas:bufferFrame];
            callback(strongView);
        }
                        isAsync:true
                      forcePost:YES];
    return view;
}

- (void)destroy {
    WeakSelf;
    [self
        runBlockOnExecuteThread:^{
            StrongSelf;
            [strongSelf destroyCanvas];
            [strongSelf.env destroyContext];
        }
                        isAsync:NO
                      forcePost:NO];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if(self = [super initWithFrame:frame]) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties =
            @{kEAGLDrawablePropertyColorFormat: kEAGLColorFormatRGBA8, kEAGLDrawablePropertyRetainedBacking: @(YES)};
        eaglLayer.contentsScale = UIScreen.mainScreen.nativeScale;
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
    config.useFbo = true;
    config.flip = false;
    config.sharedFont = false;
    config.sharedShader = false;
    gcanvas::GCanvasManager *manager = gcanvas::GCanvasManager::GetManager();
    self.canvas = manager->NewCanvas(std::to_string(canvasId++), config);
    self.canvas->CreateContext();
    self.context2d = self.canvas->GetGCanvasContext();
    self.context2d->SetFontManager(GFontManager::NewInstance());
    // gcanvas中按px计算
    self.context2d->SetDevicePixelRatio(1.0f);
    self.canvas->OnSurfaceChanged(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    self.context2d->ClearScreen();
}

- (void)destroyCanvas {
    gcanvas::GCanvasManager *manager = gcanvas::GCanvasManager::GetManager();
    manager->RemoveCanvas(self.canvas->mContextId);
    self.canvas = nil;
}

- (void *)gcanvas {
    return self.canvas;
}

- (void)drawFrame {
    WeakSelf;
    [self
        runBlockOnExecuteThread:^{
            StrongSelf;

            strongSelf.canvas->drawFrame();
            [strongSelf.env swapBuffers];
        }
                        isAsync:NO
                      forcePost:NO];
}

- (void)logPerformance:(NSTimeInterval)duration withInfo:(NSString *)info {
    WeakSelf;
    [self
        runBlockOnExecuteThread:^{
            StrongSelf;
            @try {
                NSData *jsonData = [info dataUsingEncoding:NSUTF8StringEncoding];
                NSError *err;
                NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                                    options:NSJSONReadingMutableContainers
                                                                      error:&err];
                NSString *color = @"noraml";
                if([dic[@"cmdCount"] intValue] <= 0) {
                    color = @"white";
                }
                [strongSelf logPerformance:[NSString stringWithFormat:@"%f", duration]
                                     color:color
                                  cmdCount:[[dic objectForKey:@"cmdCount"] intValue]];
            } @catch(NSException *exception) {
            }
        }
                        isAsync:YES
                      forcePost:NO];
}

- (F2CanvasThread *)canvasThread {
    return _canvasThread;
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)handleGestureInfo:(nonnull NSDictionary *)info {
    WeakSelf;
    [self
        runBlockOnExecuteThread:^{
            StrongSelf;
            if(info && [strongSelf.delegate respondsToSelector:@selector(handleGestureInfo:)]) {
                [strongSelf.delegate handleGestureInfo:info];
            }
        }
                        isAsync:NO
                      forcePost:NO];
}

- (void)logPerformance:(NSString *)duration color:(NSString *)color cmdCount:(int)cmdCount {
    NSLog(@"logPerformance: duration:%@,color:%@, cmdCoun:%d", duration, color, cmdCount);
}

- (void)runBlockOnExecuteThread:(void (^)(void))block isAsync:(BOOL)async forcePost:(BOOL)force {
    if(self.canvasThread) {
        async ? [self.canvasThread
                    runBlockASyncOnExecuteThread:^{
                        block();
                    }
                                       forcePost:force] :
                [self.canvasThread
                    runBlockSyncOnExecuteThread:^{
                        block();
                    }
                                      forcePost:force];
    } else {
        block();
    }
}

@end
