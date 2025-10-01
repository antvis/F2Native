#include "graphics/guide/GuideBase.h"
#include "graphics/XChart.h"
#include "graphics/scale/Scale.h"
#include "graphics/util/Point.h"
#include "graphics/geom/Geom.h"

using namespace xg;

util::Point guide::GuideBase::GetPosition(XChart &chart, const nlohmann::json &position, const std::string &xField, const std::string &yField) {
    if (!position.is_array() || position.size() != 2) {
        return util::Point(0, 0);
    }

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);
    
    int geometryIndex = 0;
    if (this->config_.contains("geometryIndex")) {
        geometryIndex = json::GetIntNumber(this->config_, "geometryIndex", 0);
        if (geometryIndex < 0 || geometryIndex >= chart.GetGeoms().size()) {
            geometryIndex = 0;
        }
    }
    
    const nlohmann::json &records = chart.GetSelectedRecordsForGeom(geometryIndex);
    
    bool hasSelectedRecord = false;
    if (records.size() > 0) {
        hasSelectedRecord = true;
        chart.GetLogTracer()->trace("#GetPosition xField:%s yField:%s record:%s", xField.data(), yField.data(), records[0].dump().c_str());
    }

    double x = 0;
    if (position[0].is_string()) {
        std::string xFieldCfg = position[0];
        if (xFieldCfg == "min") {
            x = 0;
        } else if (xFieldCfg == "median") {
            x = 0.5;
        } else if (xFieldCfg == "max") {
            x = 1;
        } else if (xFieldCfg == "selected" && hasSelectedRecord) {
            x = xScale.Scale(records[0][xField]);
        } else {
            x = xScale.Scale(position[0]);
        }
    } else {
        x = xScale.Scale(position[0]);
    }

    double y = 0;
    if (position[1].is_string()) {
        std::string yFieldCfg = position[1];
        if (yFieldCfg == "min") {
            y = 0;
        } else if (yFieldCfg == "median") {
            y = 0.5;
        } else if (yFieldCfg == "max") {
            y = 1;
        } else if (yFieldCfg == "selected" && hasSelectedRecord) {
            y = yScale.Scale(records[0][yField]);
        } else {
            y = yScale.Scale(position[1]);
        }
    } else {
        y = yScale.Scale(position[1]);
    }

    return chart.GetCoord().ConvertPoint(util::Point(x, y));
}
