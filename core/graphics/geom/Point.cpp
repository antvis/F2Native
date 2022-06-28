#include "Point.h"
#include "../XChart.h"

using namespace xg;

void geom::Point::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
    auto &xScale = chart.GetScale(GetXScaleField());
    for(std::size_t index = 0; index < dataArray.size(); ++index) {

        auto &groupData = dataArray[index];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.min);
            end = fmin(end, xScale.max);
        }

        for(std::size_t position = start; position <= end; ++position) {
            auto &item = groupData[position];
            item._style = this->styleConfig_;
        }
    }
}

void geom::Point::Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const {
    for(std::size_t i = start; i <= end; ++i) {
        auto &item = groupData[i];
        chart.geomShapeFactory_->DrawGeomShape(chart, type_, shapeType_, item, i, i + 1, *this->container_, this->connectNulls_);
    }
}
