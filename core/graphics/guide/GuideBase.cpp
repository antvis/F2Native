#include "GuideBase.h"
#include "../XChart.h"
#include "../scale/Scale.h"
#include "../util/Point.h"

using namespace xg;

util::Point guide::GuideBase::GetPosition(XChart &chart, const nlohmann::json &position, const std::string &xField, const std::string &yField) {
    if(!position.is_array() || position.size() != 2) {
        return util::Point(0, 0);
    }

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = 0;
    if(position[0].is_string()) {
        std::string xFieldCfg = position[0];
        if(xFieldCfg == "min") {
            x = 0;
        } else if(xFieldCfg == "median") {
            x = 0.5;
        } else if(xFieldCfg == "max") {
            x = 1;
        } else {
            x = xScale.Scale(position[0]);
        }
    } else {
        x = xScale.Scale(position[0]);
    }

    double y = 0;
    if(position[1].is_string()) {
        std::string yFieldCfg = position[1];
        if(yFieldCfg == "min") {
            y = 0;
        } else if(yFieldCfg == "median") {
            y = 0.5;
        } else if(yFieldCfg == "max") {
            y = 1;
        } else {
            y = yScale.Scale(position[1]);
        }
    } else {
        y = yScale.Scale(position[1]);
    }

    return chart.GetCoord().ConvertPoint(util::Point(x, y));
}
