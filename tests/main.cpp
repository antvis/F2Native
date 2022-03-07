#include "./scales/Category.h"
#include "./scales/Linear.h"
#include "./scales/ScaleController.h"
#include "./scales/TimeSharingLinear.h"
#include <assert.h>

#define XCTAssertEqual(a, b) assert(a == b)
#define YES true
void testLinear() {
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
    // XCTAssertEqual(Linear::TicksCallback(), YES);
}

void testCategory() {
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
    // XCTAssertEqual(Category::TicksCallback(), YES);
}

void testTimeSharingLinear() {
    XCTAssertEqual(TimeSharingLinear::WithoutTimeRange(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeHS(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeHSSameMiddleRange(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeHK(), YES);
    XCTAssertEqual(TimeSharingLinear::TimeRangeUS(), YES);
    XCTAssertEqual(TimeSharingLinear::Scale(), YES);
    XCTAssertEqual(TimeSharingLinear::Invert(), YES);
}

void testScaleController() {
    XCTAssertEqual(ScaleController::MakeScaleWithEmptyData(), YES);
    XCTAssertEqual(ScaleController::MakeLinearScale(), YES);
    XCTAssertEqual(ScaleController::MakeTimeSharingLinearScale(), YES);
    XCTAssertEqual(ScaleController::MakeCategoryScale(), YES);
    XCTAssertEqual(ScaleController::MakeTimeCatScale(), YES);
    XCTAssertEqual(ScaleController::MakeKLineScale(), YES);
    XCTAssertEqual(ScaleController::ClearScale(), YES);
}

int main() {
    printf("Start F2 Unit Test!\n");
    testLinear();
    printf("Linear Test Passed!\n");
    testCategory();
    printf("Linear Category Passed!\n");
    testTimeSharingLinear();
    printf("TimeSharingLinear Category Passed!\n");
    testScaleController();
    printf("ScaleController Category Passed!\n");
    printf("All Passed!\n");
    return 1;
}
