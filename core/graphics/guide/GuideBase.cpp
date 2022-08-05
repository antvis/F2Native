#include "GuideBase.h"
#include "../XChart.h"
#include "../scale/Scale.h"
#include "../util/Point.h"

using namespace xg;

util::Point guide::GuideBase::GetPosition(XChart &chart, const array<string, 2> &position, const std::string &xField, const std::string &yField) {
    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = 0;
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
  

    double y = 0;
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
   
    return chart.GetCoord().ConvertPoint(util::Point(x, y));
}

util::Point guide::GuideBase::GetPosition(XChart &chart, const vector<string> &position, const std::string &xField, const std::string &yField) {
    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = 0;
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
  

    double y = 0;
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
   
    return chart.GetCoord().ConvertPoint(util::Point(x, y));
}
