#include "Point.h"
#include "../XChart.h"

using namespace xg;

void geom::Point::Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const {
    for(std::size_t i = start; i <= end; ++i) {
        auto &item = groupData[i];
        chart.geomShapeFactory_->DrawGeomShape(chart, type_, shapeType_, item, i, i + 1, *this->container_, this->styleConfig_);
    }
}
