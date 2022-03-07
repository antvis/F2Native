//
//  F2NativeTests.m
//  F2NativeTests
//
//  Created by weiqing.twq on 2021/7/27.
//  Copyright © 2021 com.alipay.xgraph. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "../../tests/unit/scales/Linear.h"
#import "../../tests/unit/scales/Category.h"
#import "../../tests/unit/scales/TimeSharingLinear.h"
#import "../../tests/unit/scales/ScaleController.h"
#import "../../tests/unit/scales/KLineCat.h"

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
    XCTAssertEqual(Linear::TickCountLess2(), YES);
    XCTAssertEqual(Linear::Ticks(), YES);
    XCTAssertEqual(Linear::TicksNice(), YES);
    XCTAssertEqual(Linear::TicksNice2(), YES);
    XCTAssertEqual(Linear::TicksNice3(), YES);
    XCTAssertEqual(Linear::TicksNice4(), YES);
    XCTAssertEqual(Linear::Scale(), YES);
    XCTAssertEqual(Linear::ScaleEqualMinAndMax(), YES);
    XCTAssertEqual(Linear::ScaleWithOutMinAndMAX(), YES);
    XCTAssertEqual(Linear::Invert(), YES);
    XCTAssertEqual(Linear::TicksCallback(), YES);
}

-(void)testCategory {
    XCTAssertEqual(Category::TickCountLess2(), YES);
    XCTAssertEqual(Category::ValueCountLess2(), YES);
    XCTAssertEqual(Category::ValueCountZero(), YES);
    XCTAssertEqual(Category::TickCountZero(), YES);
    XCTAssertEqual(Category::TickCountCatEnd(), YES);
    XCTAssertEqual(Category::TickCountCatEnd2(), YES);
    XCTAssertEqual(Category::WithoutTickCount(), YES);
    XCTAssertEqual(Category::Scale(), YES);
    XCTAssertEqual(Category::ScaleEmptyValue(), YES);
    XCTAssertEqual(Category::Invert(), YES);
    XCTAssertEqual(Category::CustomTicks(), YES);
    XCTAssertEqual(Category::TicksCallback(), YES);
}

-(void)testTimeSharingLinear {
    XCTAssertEqual(TimeSharingLinear::WithoutTimeRange(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeHS(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeHSSameMiddleRange(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeHK(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeUS(), YES);
    XCTAssertEqual(TimeSharingLinear::Scale(), YES);
    XCTAssertEqual(TimeSharingLinear::Invert(), YES);
    XCTAssertEqual(TimeSharingLinear::InvalidTimeRange(), YES);
    XCTAssertEqual(TimeSharingLinear::InvalidTimeRange2(), YES);
    XCTAssertEqual(TimeSharingLinear::InvalidTimeRange3(), YES);
}

-(void)testScaleController {
    XCTAssertEqual(ScaleController::MakeScaleWithEmptyData(), YES);
    XCTAssertEqual(ScaleController::MakeLinearScale(), YES);
    XCTAssertEqual(ScaleController::MakeTimeSharingLinearScale(), YES);
    XCTAssertEqual(ScaleController::MakeCategoryScale(), YES);
    XCTAssertEqual(ScaleController::MakeTimeCatScale(), YES);
    XCTAssertEqual(ScaleController::MakeKLineScale(), YES);
    XCTAssertEqual(ScaleController::ClearScale(), YES);
    //暂时不能根据type来创建liner
//    XCTAssertEqual(ScaleController::MakeLinearScaleByType(), YES);
    XCTAssertEqual(ScaleController::MakeCategoryScaleByType(), YES);
    XCTAssertEqual(ScaleController::MakeLinearScaleByArray(), YES);
    XCTAssertEqual(ScaleController::MakeUnknowScale(), YES);
}

-(void)testKLineCat {
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *path = [bundle pathForResource:@"res/kline_day.json" ofType:nil];
    XCTAssertTrue(KLineCat::kLineDay([path UTF8String]));
    XCTAssertTrue(KLineCat::kLineDayScale([path UTF8String]));
    path = [bundle pathForResource:@"res/kline_minutes_5.json" ofType:nil];
    XCTAssertTrue(KLineCat::kLineMinites5([path UTF8String]));
    path = [bundle pathForResource:@"res/kline_minutes_60.json" ofType:nil];
    XCTAssertTrue(KLineCat::kLineMinites60([path UTF8String]));
}

- (void)testLinearScalePerformance {
    [self measureBlock:^{
        //性能测试设置为100000 来测试 不是性能测试的时候先设置为100 意思下
        Linear::Performance(100);
    }];
}

- (void)testCategoryScalePerformance {
    [self measureBlock:^{
        //性能测试设置为100000 来测试 不是性能测试的时候先设置为0 意思下
        Category::Performance(100);
    }];
}
@end
