//
//  ChartBridgeTests.m
//  F2Tests
//
//  Created by weiqing.twq on 2022/6/7.
//  Copyright © 2022 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "F2TestUtil.h"
#import "F2PixelMatch.h"
#import "F2CanvasContext.h"
#import "../../tests/unit/bridge/ChartBridge.h"


@interface F2ChartBridgeTests : XCTestCase

@end

@implementation F2ChartBridgeTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testBridge {
    XCTAssertTrue(unit::ChartBridge::Init());
    XCTAssertTrue(unit::ChartBridge::Render());
}

- (void)testBridgeImage {
    F2CanvasContext *cotenxt = [[F2CanvasContext alloc] initWithFrame:CGRectMake(0, 0, 300, 150)];
    
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/chart_bridge.json" ofType:nil];
    BOOL ret = unit::ChartBridge::Render(path.UTF8String, cotenxt.context2d, 300, 150, cotenxt.nativeScale);
//    [F2TestUtil saveImage:cotenxt.snapshot imageName:@"chartBridge.jpeg"];
    
    UIImage *diff;
    BOOL match = [F2PixelMatch match:cotenxt.snapshot
                              image2:[F2TestUtil readImage:@"res/e2e/chartBridge.jpeg"]
                            outImage:&diff];
    
    XCTAssertTrue(ret);
    XCTAssertTrue(match);
}


@end
