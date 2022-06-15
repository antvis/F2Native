//
//  ChartTests.m
//  F2Tests
//
//  Created by weiqing.twq on 2021/12/6.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "F2.h"
#import "F2TestUtil.h"
#import "F2PixelMatch.h"
#import "../../tests/e2e/Baseline.h"
#import "../../tests/e2e/BaseInterval.h"
#import "../../tests/e2e/MarketMoving.h"
#import "../../tests/unit/XChart.h"


@interface F2ChartTests : XCTestCase

@end

@implementation F2ChartTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (NSString *)readJsonData:(NSString *)path {
    NSCAssert(path, @"readJsonData path is nil");
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *jsonPath = [bundle pathForResource:path ofType:nil];
    NSString *jsonData = [NSString stringWithContentsOfFile:jsonPath encoding:NSUTF8StringEncoding error:nil];
    return jsonData;
}


- (void)testBaseline {
    CGRect frame = CGRectMake(0, 0, 375, 280);
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/mockData_baseLine.json" ofType:nil];
    F2CanvasView *canvasView = [[F2CanvasView alloc] initWithFrame:frame];
    e2e::Baseline::Case1(frame.size.width, frame.size.height, F2NativeScale, path.UTF8String, canvasView.canvasContext.context2d);
    
    //for baseline
    //[self saveImage:canvasView.snapshot];
    
    UIImage *diff;
    BOOL match = [F2PixelMatch match:canvasView.snapshot
                           image2:[F2TestUtil readImage:@"res/e2e/baseLine.jpeg"]
                         outImage:&diff];
    
    XCTAssertTrue(match);
}

- (void)testBaseInterval {
    CGRect frame = CGRectMake(0, 0, 375, 280);
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/mockData_baseLine.json" ofType:nil];
    F2CanvasView *canvasView = [[F2CanvasView alloc] initWithFrame:frame];
    e2e::BaseInterval::Case1(frame.size.width, frame.size.height, F2NativeScale, path.UTF8String, canvasView.canvasContext.context2d);
    
    //for baseline
//    [self saveImage:canvasView.snapshot];
    
    UIImage *diff;
    BOOL match = [F2PixelMatch match:canvasView.snapshot
                              image2:[F2TestUtil readImage:@"res/e2e/baseInterval.jpeg"]
                            outImage:&diff];
    XCTAssertTrue(match);
}

- (void)testMarketMoving {
    CGRect frame = CGRectMake(0, 0, 375, 280);
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/mockData_marketTrend.json" ofType:nil];
    F2CanvasView *canvasView = [[F2CanvasView alloc] initWithFrame:frame];
    e2e::MarketMoving::Case1(frame.size.width, frame.size.height, F2NativeScale, path.UTF8String, canvasView.canvasContext.context2d);
    
    //for baseline
//    [self saveImage:canvasView.snapshot];
    
    UIImage *diff;
    BOOL match = [F2PixelMatch match:canvasView.snapshot
                              image2:[F2TestUtil readImage:@"res/e2e/marketMoving.jpeg"]
                            outImage:&diff];
    XCTAssertTrue(match);
}

- (void)testCanvasSetFrame {
    F2CanvasContext *canvasContext = [[F2CanvasContext alloc] initWithFrame:CGRectMake(0, 0, 100, 200)];
    XCTAssertTrue(100 * F2NativeScale == CGBitmapContextGetWidth(canvasContext.context2d));
    XCTAssertTrue(200 * F2NativeScale == CGBitmapContextGetHeight(canvasContext.context2d));
    
    [canvasContext setFrame:CGRectMake(0, 0, 200, 300)];
    XCTAssertTrue(200 * F2NativeScale == CGBitmapContextGetWidth(canvasContext.context2d));
    XCTAssertTrue(300 * F2NativeScale == CGBitmapContextGetHeight(canvasContext.context2d));
}

- (void)testXChart {
    XCTAssertTrue(unit::XChart::ParseConfigEmpty());
    XCTAssertTrue(unit::XChart::ParseConfigNoGeoms());
    XCTAssertTrue(unit::XChart::ParseConfigGeomObjectError());
    XCTAssertTrue(unit::XChart::ParseConfigGeomObject());
    XCTAssertTrue(unit::XChart::ParseConfigGuideObjectError());
    XCTAssertTrue(unit::XChart::ParseConfigGuideObject());
    XCTAssertTrue(unit::XChart::ParseConfigInteractionObjectError());
    XCTAssertTrue(unit::XChart::ParseConfigInteractionObject());
}


@end
