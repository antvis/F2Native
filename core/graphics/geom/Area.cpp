#include "Area.h"
#include "../XChart.h"

using namespace xg;

nlohmann::json geom::Area::CreateShapePointsCfg(XChart &chart, XData &data) {
    auto &xScale = chart.GetScale(GetXScaleField());
    auto &yScale = chart.GetScale(GetYScaleField());

    auto &xVal = (*data.data)[GetXScaleField()];
    auto &yVal = (*data.data)[GetYScaleField()];

    nlohmann::json rst;
    rst["x"] = xScale.Scale(xVal);
    if(!data.adjust.empty()) {
        nlohmann::json yRst;
        for(std::size_t index = 0; index < data.adjust.size(); ++index) {
            yRst.push_back(yScale.Scale(data.adjust[index]));
        }
        rst["y"] = yRst;
    } else {
        rst["y"] = yScale.Scale(yVal);
    }
    rst["y0"] = yScale.Scale(this->GetYMinValue(chart));
    return rst;
}

nlohmann::json geom::Area::GetAreaPoints(XChart &chart, XData &data, nlohmann::json &cfg) {
    auto &x = cfg["x"];
    auto &y = cfg["y"];
    auto &y0 = cfg["y0"];

    if(!y.is_array()) {
        y = {y0, y};
    }
    return {{{"x", x}, {"y", y[0]}}, {{"x", x}, {"y", y[1]}}};
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
            nlohmann::json cfg = CreateShapePointsCfg(chart, data);
            nlohmann::json points = GetAreaPoints(chart, data, cfg);
            data._points = points;
        }
        // nextPoints.
    }
}
