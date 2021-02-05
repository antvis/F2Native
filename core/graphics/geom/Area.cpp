#include "Area.h"
#include "graphics/XChart.h"

using namespace xg;

nlohmann::json geom::Area::CreateShapePointsCfg(XChart &chart, nlohmann::json &data) {
    auto &xScale = chart.GetScale(GetXScaleField());
    auto &yScale = chart.GetScale(GetYScaleField());

    nlohmann::json &xVal = data[GetXScaleField()];
    nlohmann::json &yVal = data[GetYScaleField()];

    nlohmann::json rst;
    rst["x"] = xScale.Scale(xVal);
    if(yVal.is_array()) {
        nlohmann::json yRst;
        for(std::size_t index = 0; index < yVal.size(); ++index) {
            yRst.push_back(yScale.Scale(yVal[index]));
        }
        rst["y"] = yRst;
    } else {
        rst["y"] = yScale.Scale(yVal);
    }
    rst["y0"] = yScale.Scale(this->GetYMinValue(chart));
    return rst;
}

nlohmann::json geom::Area::GetAreaPoints(XChart &chart, nlohmann::json &data, nlohmann::json &cfg) {
    auto &x = cfg["x"];
    auto &y = cfg["y"];
    auto &y0 = cfg["y0"];

    if(!y.is_array()) {
        y = {y0, y};
    }
    return {{{"x", x}, {"y", y[0]}}, {{"x", x}, {"y", y[1]}}};
}

void geom::Area::BeforeMapping(XChart &chart, nlohmann::json &dataArray) {
    auto &xScale = chart.GetScale(GetXScaleField());

    for(std::size_t i = 0; i < dataArray.size(); ++i) {
        nlohmann::json &groupData = dataArray[i];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.min);
            end = fmin(end, xScale.max);
        }

        for(std::size_t index = start; index <= end; ++index) {
            nlohmann::json &data = groupData[index];
            nlohmann::json cfg = CreateShapePointsCfg(chart, data);
            nlohmann::json points = GetAreaPoints(chart, data, cfg);
            data["_points"] = points;
        }
        // nextPoints.
    }
}
