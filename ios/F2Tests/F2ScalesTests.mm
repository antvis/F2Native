//
//  F2NativeTests.m
//  F2NativeTests
//
//  Created by weiqing.twq on 2021/7/27.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "unit/scale/Linear.h"
#import "unit/scale/Category.h"
#import "unit/scale/TimeSharingLinear.h"
#import "unit/scale/ScaleController.h"
#import "unit/scale/KLineCat.h"

@interface F2ScaleTests : XCTestCase

@end

@implementation F2ScaleTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testLinear {
    XCTAssertEqual(unit::Linear::TickCountLess2(), YES);
    XCTAssertEqual(unit::Linear::Ticks(), YES);
    XCTAssertEqual(unit::Linear::TicksNice(), YES);
    XCTAssertEqual(unit::Linear::TicksNice2(), YES);
    XCTAssertEqual(unit::Linear::TicksNice3(), YES);
    XCTAssertEqual(unit::Linear::TicksNice4(), YES);
    XCTAssertEqual(unit::Linear::Scale(), YES);
    XCTAssertEqual(unit::Linear::ScaleEqualMinAndMax(), YES);
    XCTAssertEqual(unit::Linear::ScaleWithOutMinAndMAX(), YES);
    XCTAssertEqual(unit::Linear::Invert(), YES);
    XCTAssertEqual(unit::Linear::TicksCallback(), YES);
}

-(void)testCategory {
    XCTAssertEqual(unit::Category::TickCountLess2(), YES);
    XCTAssertEqual(unit::Category::ValueCountLess2(), YES);
    XCTAssertEqual(unit::Category::ValueCountZero(), YES);
    XCTAssertEqual(unit::Category::TickCountZero(), YES);
    XCTAssertEqual(unit::Category::TickCountCatEnd(), YES);
    XCTAssertEqual(unit::Category::TickCountCatEnd2(), YES);
    XCTAssertEqual(unit::Category::WithoutTickCount(), YES);
    XCTAssertEqual(unit::Category::Scale(), YES);
    XCTAssertEqual(unit::Category::ScaleEmptyValue(), YES);
    XCTAssertEqual(unit::Category::Invert(), YES);
    XCTAssertEqual(unit::Category::CustomTicks(), YES);
    XCTAssertEqual(unit::Category::TicksCallback(), YES);
}

-(void)testTimeSharingLinear {
    XCTAssertEqual(unit::TimeSharingLinear::WithoutTimeRange(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeHS(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeHSSameMiddleRange(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeHK(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeUS(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::Scale(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::Invert(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::InvalidTimeRange(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::InvalidTimeRange2(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::InvalidTimeRange3(), YES);
}

-(void)testScaleController {
    XCTAssertEqual(unit::ScaleController::MakeScaleWithEmptyData(), YES);
    XCTAssertEqual(unit::ScaleController::MakeLinearScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeTimeSharingLinearScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeCategoryScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeTimeCatScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeKLineScale(), YES);
    XCTAssertEqual(unit::ScaleController::ClearScale(), YES);
    //暂时不能根据type来创建liner
//    XCTAssertEqual(ScaleController::MakeLinearScaleByType(), YES);
    XCTAssertEqual(unit::ScaleController::MakeCategoryScaleByType(), YES);
    XCTAssertEqual(unit::ScaleController::MakeLinearScaleByArray(), YES);
    XCTAssertEqual(unit::ScaleController::MakeUnknowScale(), YES);
}

-(void)testKLineCat {
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/kline_day.json" ofType:nil];
    XCTAssertTrue(unit::KLineCat::kLineDay([path UTF8String]));
    XCTAssertTrue(unit::KLineCat::kLineDayScale([path UTF8String]));
    path = [bundle pathForResource:@"res/kline_minutes_5.json" ofType:nil];
    XCTAssertTrue(unit::KLineCat::kLineMinites5([path UTF8String]));
    path = [bundle pathForResource:@"res/kline_minutes_60.json" ofType:nil];
    XCTAssertTrue(unit::KLineCat::kLineMinites60([path UTF8String]));
}

- (void)testLinearScalePerformance {
    [self measureBlock:^{
        //性能测试设置为100000 来测试 不是性能测试的时候先设置为100 意思下
        unit::Linear::Performance(100);
    }];
}

- (void)testCategoryScalePerformance {
    [self measureBlock:^{
        //性能测试设置为100000 来测试 不是性能测试的时候先设置为0 意思下
        unit::Category::Performance(100);
    }];
}
@end
