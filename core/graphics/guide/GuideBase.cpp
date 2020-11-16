#include "graphics/guide/GuideBase.h"
#include "graphics/XChart.h"
#include "graphics/scale/Scale.h"
#include "graphics/util/Point.h"

using namespace xg;

util::Point guide::GuideBase::GetPosition(XChart &chart, const std::string &xField, const std::string &yField) {
    if(!this->config_.contains("position")) {
        return util::Point(0, 0);
    }

    nlohmann::json &positionCfg = config_["position"];
    if(!positionCfg.is_array() || positionCfg.size() != 2) {
        return util::Point(0, 0);
    }

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = 0;
    if(positionCfg[0].is_string()) {
        std::string xFieldCfg = positionCfg[0];
        if(xFieldCfg == "min") {
            x = 0;
        } else if(xFieldCfg == "median") {
            x = 0.5;
        } else if(xFieldCfg == "max") {
            x = 1;
        } else {
            x = xScale.Scale(positionCfg[0]);
        }
    } else {
        x = xScale.Scale(positionCfg[0]);
    }

    double y = 0;
    if(positionCfg[1].is_string()) {
        std::string yFieldCfg = positionCfg[1];
        if(yFieldCfg == "min") {
            y = 0;
        } else if(yFieldCfg == "median") {
            y = 0.5;
        } else if(yFieldCfg == "max") {
            y = 1;
        } else {
            y = yScale.Scale(positionCfg[1]);
        }
    } else {
        y = yScale.Scale(positionCfg[1]);
    }

    return chart.GetCoord().ConvertPoint(util::Point(x, y));
}
