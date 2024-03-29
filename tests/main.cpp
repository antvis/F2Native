#include <assert.h>
#include <jni.h>
#include "unit/scale/Category.h"
#include "unit/scale/Linear.h"
#include "unit/scale/ScaleController.h"
#include "unit/scale/TimeSharingLinear.h"

#define XCTAssertEqual(a, b) assert(a == b)
#define YES true
void testLinear() {
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
    // XCTAssertEqual(Linear::TicksCallback(), YES);
}

void testCategory() {
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
    // XCTAssertEqual(Category::TicksCallback(), YES);
}

void testTimeSharingLinear() {
    XCTAssertEqual(unit::TimeSharingLinear::WithoutTimeRange(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeHS(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeHSSameMiddleRange(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeHK(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::TimeRangeUS(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::Scale(), YES);
    XCTAssertEqual(unit::TimeSharingLinear::Invert(), YES);
}

void testScaleController() {
    XCTAssertEqual(unit::ScaleController::MakeScaleWithEmptyData(), YES);
    XCTAssertEqual(unit::ScaleController::MakeLinearScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeTimeSharingLinearScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeCategoryScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeTimeCatScale(), YES);
    XCTAssertEqual(unit::ScaleController::MakeKLineScale(), YES);
    XCTAssertEqual(unit::ScaleController::ClearScale(), YES);
}

//单测的入口
extern "C"
JNIEXPORT jint JNICALL
Java_com_antgroup_antv_f2_F2TestProxy_runTest(JNIEnv *env) {
    F2_LOG_I("F2Test", "Start F2 Unit Test!\n");
    testLinear();
    F2_LOG_I("F2Test", "Linear Test Passed!\n");
    testCategory();
    F2_LOG_I("F2Test", "Category Passed!\n");
    testTimeSharingLinear();
    F2_LOG_I("F2Test", "TimeSharingLinear Category Passed!\n");
    testScaleController();
    F2_LOG_I("F2Test", "ScaleController Category Passed!\n");
    F2_LOG_I("F2Test", "All Passed!\n");
    return 1;
}