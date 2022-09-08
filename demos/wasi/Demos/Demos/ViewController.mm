//
//  ViewController.m
//  Demos
//
//  Created by weiqing.twq on 8/18/22.
//

#import "ViewController.h"
#import <F2/F2.h>
#import <F2/Core.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#import "wasm3_cpp.h"
#import <Wasm3Lib/wasm3.h>
#import <Wasm3Lib/m3_config.h>
#import <Wasm3Lib/m3_api_libc.h>
#import <Wasm3Lib/m3_api_wasi.h>
#import <Wasm3Lib/m3_exception.h>

#define FATAL(msg, ...) { printf("Fatal: " msg "\n", ##__VA_ARGS__); return; }

using namespace xg::canvas;



@interface ViewController () {
    
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    F2CanvasView *view = [F2CanvasView canvas:CGRectMake(0, self.navigationController.navigationBar.bounds.size.height, self.view.frame.size.width, 200)];
    [self.view addSubview:view];
    
    view.backgroundColor = UIColor.clearColor;


    [self initwasm3:view];
}

//-(void)initwasmcpp {
//    NSString *myPath = [NSBundle.mainBundle pathForResource:@"f2" ofType:@"wasm"];
//
//    wasm3::environment env;
//    wasm3::runtime runtime = env.new_runtime(8 * 1024);
//    std::ifstream wasm_file(myPath.UTF8String, std::ios::binary | std::ios::in);
//    if (!wasm_file.is_open()) {
//        throw std::runtime_error("Failed to open wasm file");
//    }
//    wasm3::module mod = env.parse_module(wasm_file);
//    runtime.load(mod);
//
//    mod.link_libc();
//    mod.link_wasi();
//
//    mod.link("*", "clearRect", clearRect);
//
//    wasm3::function test_fn = runtime.find_function("_start");
//    test_fn.call<int>();
//}

-(void)initwasm3:(F2CanvasView *)view {
    CGContextRef contextRef = [view.canvasContext context2d];
    CGFloat width = view.frame.size.width * F2NativeScale;
    CGFloat height = view.frame.size.height * F2NativeScale;
    CoreGraphicsContext *context = new CoreGraphicsContext(contextRef, width, height, UIScreen.mainScreen.nativeScale, nullptr);
    
    auto start = xg::CurrentTimestampAtMM();
    NSString *myPath = [NSBundle.mainBundle pathForResource:@"f2" ofType:@"wasm"];
    NSData *myData = [NSData dataWithContentsOfFile:myPath];
    
    M3Result result = m3Err_none;

    printf("Loading WebAssembly...\n");

    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("m3_NewEnvironment failed");

    IM3Runtime runtime = m3_NewRuntime (env, 8 * 1024, NULL);
    if (!runtime) FATAL("m3_NewRuntime failed");

    IM3Module module;
    result = m3_ParseModule (env, &module, (const uint8_t * const)myData.bytes, (uint32_t)myData.length);
    if (result) FATAL("m3_ParseModule: %s", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("m3_LoadModule: %s", result);

    result = m3_LinkLibC (module);
    if (result) FATAL("m3_LinkLibC: %s", result);
    
    result = m3_LinkWASI (module);
    if (result) FATAL("m3_LinkWASI: %s", result);
    
    result = m3_LinkCanvas(module, context);
    if (result) FATAL("m3_LinkCanvas: %s", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "hello");
    if (result) FATAL("m3_FindFunction: %s", result);

    printf("Running CoreMark 1.0...\n");

    result = m3_CallV (f, width, height, F2NativeScale);
    if (result) FATAL("m3_Call: %s", result);

    float value = 0;
    result = m3_GetResultsV (f, &value);
    if (result) FATAL("m3_GetResults: %s", result);

    printf("Call _start function cost %lld ms\n", (xg::CurrentTimestampAtMM() - start));
}

m3ApiRawFunction(f2_clear_rect) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, width)
    m3ApiGetArg   (float, height)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->ClearRect(x, y, width, height);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_line_to) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->LineTo(x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_move_to) {
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->MoveTo(x, y);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_set_fill_style) {
    m3ApiGetArgMem   (const char *, color)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetFillStyle(color);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_set_stroke_style) {
    m3ApiGetArgMem   (const char *, color)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetStrokeStyle(color);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_set_line_width) {
    m3ApiGetArg   (float, lineWidth)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetLineWidth(lineWidth);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_global_alpha) {
    m3ApiReturnType  (float)
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    m3ApiReturn(context->GlobalAlpha());
}

m3ApiRawFunction(f2_set_font) {
    m3ApiGetArgMem   (const char *, font)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->SetFont(font);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_fill_text) {
    m3ApiGetArgMem   (const char *, font)
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, maxWith)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->FillText(font, x, y, maxWith);
    m3ApiSuccess();
}

m3ApiRawFunction(f2_stroke_text) {
    m3ApiGetArgMem   (const char *, font)
    m3ApiGetArg   (float, x)
    m3ApiGetArg   (float, y)
    m3ApiGetArg   (float, maxWith)
    
    CanvasContext* context = (CanvasContext*)(_ctx->userdata);
    context->StrokeText(font, x, y, maxWith);
    m3ApiSuccess();
}

static
M3Result SuppressLookupFailure(M3Result i_result)
{
    if (i_result == m3Err_functionLookupFailed)
        return m3Err_none;
    else
        return i_result;
}

M3Result  m3_LinkCanvas  (IM3Module module, CoreGraphicsContext *context)
{
    M3Result result = m3Err_none;

    const char* canvas = "*";

_   (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "clearRect", "v(ffff)", &f2_clear_rect, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "lineTo", "v(ff)", &f2_line_to, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "moveTo", "v(ff)", &f2_move_to, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setFillStyle", "v(*)", &f2_set_fill_style, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setStrokeStyle", "v(*)", &f2_set_stroke_style, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setLineWidth", "v(f)", &f2_set_line_width, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "setFont", "v(*)", &f2_set_font, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "globalAlpha", "f()", &f2_global_alpha, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "fillText", "v(*fff)", &f2_fill_text, context)));
    (SuppressLookupFailure (m3_LinkRawFunctionEx (module, canvas, "strokeText", "v(*fff)", &f2_stroke_text, context)));
_catch:
    return result;
}


@end
