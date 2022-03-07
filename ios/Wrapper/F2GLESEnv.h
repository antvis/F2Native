#if defined(__APPLE__)
#import <TargetConditionals.h>
#endif

#if defined(TARGET_STANDALONE) && (TARGET_OS_MACCATALYST != 1)

#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface F2GLESEnv : NSObject

- (instancetype)init:(CALayer *)layer;
- (void)destroyContext;
- (void)swapBuffers;

@end
NS_ASSUME_NONNULL_END

#endif
