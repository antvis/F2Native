//
//  ViewController.m
//  Demos
//
//  Created by weiqing.twq on 8/18/22.
//

#import "ViewController.h"
#import <F2/F2.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <Wasm3Lib/m3_config.h>
#include <Wasm3Lib/m3_api_libc.h>
#include <Wasm3Lib/m3_api_wasi.h>
#include <Wasm3Lib/m3_exception.h>
#include "wasm_canvas_api.h"


#define FATAL(msg, ...) { printf("Fatal: " msg "\n", ##__VA_ARGS__); return; }

using namespace xg::canvas;



@interface ViewController () {
    
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColor.whiteColor;
    CGFloat height = self.navigationController.navigationBar.frame.size.height + [[UIApplication sharedApplication] statusBarFrame].size.height;

    F2CanvasView *view = [F2CanvasView canvas:CGRectMake(0, height, self.view.frame.size.width, 200)];
    [self.view addSubview:view];
    view.backgroundColor = UIColor.clearColor;
    [self initwasm:view];
}

-(void)initwasm:(F2CanvasView *)view {
    CGContextRef contextRef = [view.canvasContext context2d];
    CGFloat width = view.frame.size.width ;
    CGFloat height = view.frame.size.height ;
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

@end
