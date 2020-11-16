#import "F2GLESEnv.h"

static EAGLContext *_staticFirstContext;

@interface F2GLESEnv () {

    GLuint _defaultFrameBuffer;
    GLuint _defaultColorRenderBuffer;
    GLuint _depthRenderBuffer;

    // msaa的fbo和colorbuffer
    GLuint _msaaFrameBuffer;
    GLuint _msaaColorRenderBuffer;

    GLint _drawableWidth;
    GLint _drawableHeight;

    EAGLContext *context;
    id<EAGLDrawable> eaglLayer;
    CGFloat contentScale;
    BOOL isGL3;
}

@end

@implementation F2GLESEnv

- (instancetype)init:(CALayer *)layer {
    if(self = [super init]) {
        eaglLayer = (id<EAGLDrawable>)layer;
        contentScale = layer.contentsScale;
        [self createContext];
        [self setupDrawable];
    }
    return self;
}

- (void)createContext {
    _defaultFrameBuffer = 0;
    _defaultColorRenderBuffer = 0;
    _depthRenderBuffer = 0;
    _msaaFrameBuffer = 0;
    _msaaColorRenderBuffer = 0;
    isGL3 = NO;
    context = [self createGLES2Context];
}

- (void)setupDrawable {
    [self setCurrentContext];
    [self createBuffer];
}

- (void)destroyContext {
    [self setCurrentContext];
    [self destoryFrameBuffer];
    [self destorySampleBuffer];

    if([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)setCurrentContext {
    if([EAGLContext currentContext] != context) {
        [EAGLContext setCurrentContext:context];
    }
}

#pragma mark FrameBuffer
- (void)createBuffer {
    if(_defaultColorRenderBuffer == 0 && _defaultFrameBuffer == 0) {
        glGenFramebuffers(1, &_defaultFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _defaultFrameBuffer);

        glGenRenderbuffers(1, &_defaultColorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _defaultColorRenderBuffer);

        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _defaultColorRenderBuffer);

        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_drawableWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_drawableHeight);

        // mass
        glGenFramebuffers(1, &_msaaFrameBuffer);
        glGenRenderbuffers(1, &_msaaColorRenderBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _msaaFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _msaaColorRenderBuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, _drawableWidth, _drawableHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _msaaColorRenderBuffer);

        // path
        glGenRenderbuffers(1, &_depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8_OES, _drawableWidth, _drawableHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
    }
}

- (void)destoryFrameBuffer {
    if(_defaultColorRenderBuffer) {
        glDeleteRenderbuffers(1, &_defaultColorRenderBuffer);
        _defaultColorRenderBuffer = 0;
    }

    if(_defaultFrameBuffer) {
        glDeleteFramebuffers(1, &_defaultFrameBuffer);
        _defaultFrameBuffer = 0;
    }
    if(_depthRenderBuffer) {
        glDeleteRenderbuffers(1, &_depthRenderBuffer);
        _depthRenderBuffer = 0;
    }
}

- (void)destorySampleBuffer {
    if(_msaaFrameBuffer) {
        glDeleteFramebuffers(1, &_msaaFrameBuffer);
        _msaaFrameBuffer = 0;
    }

    if(_msaaColorRenderBuffer) {
        glDeleteRenderbuffers(1, &_msaaColorRenderBuffer);
        _msaaColorRenderBuffer = 0;
    }
}

- (void)swapBuffers {
    [self setCurrentContext];

    glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, _msaaFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, _defaultFrameBuffer);
    glResolveMultisampleFramebufferAPPLE();

    glBindRenderbuffer(GL_RENDERBUFFER, _defaultColorRenderBuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
    glBindFramebuffer(GL_FRAMEBUFFER, _msaaFrameBuffer);
}

- (EAGLContext *)createGLES2Context {
    EAGLContext *context = nil;
    if(!_staticFirstContext) {
        _staticFirstContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        context = _staticFirstContext;
    } else {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:_staticFirstContext.sharegroup];
    }

    return context;
}

@end
