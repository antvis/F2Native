#include "Point.h"
#include "graphics/XChart.h"

using namespace xg;

void geom::Point::BeforeMapping(XChart &chart, nlohmann::json &dataArray) {

    auto &xScale = chart.GetScale(GetXScaleField());
    for(std::size_t index = 0; index < dataArray.size(); ++index) {

        nlohmann::json &groupData = dataArray[index];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.min);
            end = fmin(end, xScale.max);
        }

        for(std::size_t position = start; position <= end; ++position) {
            nlohmann::json &item = groupData[position];
            item["_style"] = this->styleConfig_;
        }
    }
}

void geom::Point::Draw(XChart &chart, const nlohmann::json &groupData, std::size_t start, std::size_t end) const {
    for(std::size_t i = start; i <= end; ++i) {
        const nlohmann::json &item = groupData[i];
        chart.geomShapeFactory_->DrawGeomShape(chart, type_, shapeType_, item, i, i + 1, *this->container_, this->connectNulls_);
    }
}