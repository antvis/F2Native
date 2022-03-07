//
//  F2NativeTests.m
//  F2NativeTests
//
//  Created by weiqing.twq on 2021/7/27.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "unit/utils/StringUtils.h"
#import "unit/utils/XTime.h"
#import "unit/utils/json.h"
#import "unit/utils/Point.h"


@interface F2UtilsTests : XCTestCase

@end

@implementation F2UtilsTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testStringUtils {
    XCTAssertEqual(unit::StringUtils::SplitFields1(), YES);
    XCTAssertEqual(unit::StringUtils::SplitFields2(), YES);
    XCTAssertEqual(unit::StringUtils::SplitDay(), YES);
    XCTAssertEqual(unit::StringUtils::SplitMinutes(), YES);
}

-(void)testXTime {
    XCTAssertEqual(unit::XTime::Parse(), YES);
    XCTAssertEqual(unit::XTime::ParseTM(), YES);
    XCTAssertEqual(unit::XTime::ParseTimeStamp(), YES);
    XCTAssertEqual(unit::XTime::ParseTimeStampTimeZone(), YES);
}

-(void)testJson {
    XCTAssertTrue(unit::Json::ParseString());
    XCTAssertTrue(unit::Json::ParseDashArray());
    XCTAssertTrue(unit::Json::ParseRoundings());
    XCTAssertTrue(unit::Json::Get());
    XCTAssertTrue(unit::Json::GetString());
    XCTAssertTrue(unit::Json::GetNumber());
    XCTAssertTrue(unit::Json::GetArray());
    XCTAssertTrue(unit::Json::GetBool());
    XCTAssertTrue(unit::Json::GetObject());
}

-(void)testPoint {
    XCTAssertTrue(unit::PointTest::Reset());
    XCTAssertTrue(unit::PointTest::Max());
    XCTAssertTrue(unit::PointTest::Min());
    XCTAssertTrue(unit::PointTest::Add());
    XCTAssertTrue(unit::PointTest::Sub());
    XCTAssertTrue(unit::PointTest::Scale());
    XCTAssertTrue(unit::PointTest::Distance());
}

-(void)testVector2d {
    
}


@end
