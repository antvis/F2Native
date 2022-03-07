//
//  ChartTests.m
//  F2Tests
//
//  Created by weiqing.twq on 2021/12/6.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "F2.h"
#import "F2PixelMatch.h"
#import "e2e/Baseline.h"
#import "e2e/BaseInterval.h"


@interface F2ChartTests : XCTestCase

@end

@implementation F2ChartTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

- (NSString *)readJsonData:(NSString *)path {
    NSCAssert(path, @"readJsonData path is nil");
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *jsonPath = [bundle pathForResource:path ofType:nil];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    return jsonData;
}

- (BOOL)saveImage:(UIImage *)image {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES);
    NSString *filePath = [[paths objectAtIndex:0]stringByAppendingPathComponent:
                          [NSString stringWithFormat:@"demo.jpeg"]];  // 保存文件的名称
    NSLog(@"saveImage: %@", filePath);
    BOOL result =[UIImageJPEGRepresentation(image, 1) writeToFile:filePath  atomically:YES]; // 保存成功会返回YES
    NSCAssert(result, @"saveImage failure");
    return result;
}

- (UIImage *)readImage:(NSString *)imageName {
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *jsonPath = [bundle pathForResource:imageName ofType:nil];
    return [[UIImage alloc] initWithContentsOfFile:jsonPath];
}

- (void)testBaseline {
    CGRect frame = CGRectMake(0, 0, 375, 280);
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/mockData_baseLine.json" ofType:nil];
    F2CanvasView *canvasView = [F2CanvasView canvasWithFrame:frame andBizId:@"" complete:nil];
    e2e::Baseline::Case1(frame.size.width, frame.size.height, F2NativeScale, path.UTF8String, canvasView.canvasContext.context2d);
    
    //for baseline
    //[self saveImage:canvasView.snapshot];
    
    UIImage *diff;
    BOOL match = [F2PixelMatch match:canvasView.snapshot
                           image2:[self readImage:@"res/e2e/baseLine.jpeg"]
                         outImage:&diff];
    
    XCTAssertTrue(match);
}

- (void)testBaseInterval {
    CGRect frame = CGRectMake(0, 0, 375, 280);
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/mockData_baseLine.json" ofType:nil];
    F2CanvasView *canvasView = [F2CanvasView canvasWithFrame:frame andBizId:@"" complete:nil];
    e2e::BaseInterval::Case1(frame.size.width, frame.size.height, F2NativeScale, path.UTF8String, canvasView.canvasContext.context2d);
    
    //for baseline
//    [self saveImage:canvasView.snapshot];
    
    UIImage *diff;
    BOOL match = [F2PixelMatch match:canvasView.snapshot
                              image2:[self readImage:@"res/e2e/baseInterval.jpeg"]
                            outImage:&diff];
    XCTAssertTrue(match);
}


@end
