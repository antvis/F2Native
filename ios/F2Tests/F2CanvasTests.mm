//
//  F2NativeTests.m
//  F2NativeTests
//
//  Created by weiqing.twq on 2021/7/27.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "../../tests/unit/canvas/Polar.h"
#import "../../tests/unit/canvas/Cartesian.h"
#import "../../tests/unit/canvas/CanvasColorParser.h"
#import "../../tests/unit/canvas/CanvasFontParser.h"



@interface CanvasTests : XCTestCase

@end

@implementation CanvasTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testPolar {
    XCTAssertTrue(unit::Polar::Init());
    XCTAssertTrue(unit::Polar::ConvertPoint1());
    XCTAssertTrue(unit::Polar::ConvertPoint2());
    XCTAssertTrue(unit::Polar::Reset());
}

- (void)testCartesian {
    XCTAssertTrue(unit::Cartesian::Init());
    XCTAssertTrue(unit::Cartesian::ConvertPoint1());
    XCTAssertTrue(unit::Cartesian::ConvertPoint2());
    XCTAssertTrue(unit::Cartesian::Reset());
}

- (void)testCanvasColorParser {
    XCTAssertTrue(unit::CanvasColorParserTest::Parse());
    XCTAssertTrue(unit::CanvasColorParserTest::Error());
}

- (void)testCanvasFontStyle {
    XCTAssertTrue(unit::CanvasFontParserTest::Parse());
    XCTAssertTrue(unit::CanvasFontParserTest::Error());
}



@end
