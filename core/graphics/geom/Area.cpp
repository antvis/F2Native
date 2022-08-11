#include "Area.h"
#include "../XChart.h"

using namespace xg;

std::vector<util::Point> geom::Area::CreateShapePointsCfg(XChart &chart, XData &data) {
    auto &xScale = chart.GetScale(GetXScaleField());
    auto &yScale = chart.GetScale(GetYScaleField());

    auto &xVal = data.data[GetXScaleField()];
    auto &yVal = data.data[GetYScaleField()];


    auto x = xScale.Scale(xVal);
    auto y0 = yScale.Scale(this->GetYMinValue(chart));
    vector<double> yRst;
    if(!data.adjust.empty()) {
        for(std::size_t index = 0; index < data.adjust.size(); ++index) {
            yRst.push_back(yScale.Scale(data.adjust[index]));
        }
    } else {
        auto y = yScale.Scale(yVal);
        yRst = {y0, y};
    }

    return {util::Point{x, yRst[0]}, util::Point{x, yRst[1]}};
}

void geom::Area::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
    auto &xScale = chart.GetScale(GetXScaleField());

    for(std::size_t i = 0; i < dataArray.size(); ++i) {
        auto &groupData = dataArray[i];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.GetMin());
            end = fmin(end, xScale.GetMax());
        }

        for(std::size_t index = start; index <= end; ++index) {
            auto &data = groupData[index];
            data.points = CreateShapePointsCfg(chart, data);
        }
        // nextPoints.
    }
}
