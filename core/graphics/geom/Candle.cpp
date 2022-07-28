#include "Candle.h"
#include "../XChart.h"
#include "../scale/Scale.h"

using namespace xg;

unordered_map<string, double> geom::Candle::CreateShapePointsCfg(XChart &chart, XData &item, size_t index) {
    const std::string &xField = GetXScaleField();
    const std::string &yField = GetYScaleField();

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = xScale.Scale((*item.data)[xField]);
    std::size_t state = 0; // 0 - 平, 1 涨， -1 跌

    auto &yVal = (*item.data)[yField];
    if(!yVal.is_array() || yVal.empty() || yVal.size() < 4) {
        return {{"x", x}, {"y0", 0}, {"y1", 0}, {"size", 0}, {"state", 0}, {"max", 0}, {"min", 0}};
    }
    double start = yScale.Scale(yVal[0]); // 开盘
    double end = yScale.Scale(yVal[3]);   // 收盘
    double max = yScale.Scale(yVal[1]);   // 最高
    double min = yScale.Scale(yVal[2]);   // 最低

    double startOri = yVal[0];
    double endOri = yVal[3];

    if(xg::IsEqual(startOri, endOri)) {
        state = 0;
    } else if(startOri > endOri) {
        state = -1;
    } else {
        state = 1;
    }

    std::size_t count = fmax(xScale.GetValuesSize(), 1);
    double normalizeSize = 1.0 / count;

    normalizeSize *= 0.9;
    return {{"x", x}, {"y0", start}, {"y1", end}, {"size", normalizeSize}, {"state", state}, {"max", max}, {"min", min}};
}

array<util::Point, 4> geom::Candle::getRectPoints(unordered_map<string, double> &cfg) {
    double x = cfg["x"];
    double size = cfg["size"];
    const int state = cfg["state"];

    double yMax;
    double yMin;

    if(state >= 0) { // 涨, 平
        yMax = cfg["y0"];
        yMin = cfg["y1"];
    } else { // 跌
        yMax = cfg["y1"];
        yMin = cfg["y0"];
    }

    array<util::Point, 4> rst;
    double xMin = x - size / 2;
    double xMax = x + size / 2;
    rst[0] = util::Point{xMin, yMin};
    rst[1] = util::Point{xMin, yMax};
    rst[2] = util::Point{xMax, yMax};
    rst[3] = util::Point{xMax, yMin};
    return rst;
}

array<double, 2> geom::Candle::getLinePoints(unordered_map<string, double> &cfg) {
    return {cfg["max"], cfg["min"]};
}

void geom::Candle::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
    auto timestamp = xg::CurrentTimestampAtMM();
    auto &xScale = chart.GetScale(GetXScaleField());
    for(std::size_t index = 0; index < dataArray.size(); ++index) {

        auto &groupData = dataArray[index];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.GetMin());
            end = fmin(end, xScale.GetMax());
        }

        for(std::size_t position = start; position <= end; ++position) {
            auto &item = groupData[position];
            auto cfg = CreateShapePointsCfg(chart, item, index);
            item.rect = getRectPoints(cfg);
            item.line = getLinePoints(cfg);
            item.state = cfg["state"];
        }
    }
    chart.GetLogTracer()->trace("Geom#%s Beforemapping duration: %lums", type_.data(), (CurrentTimestampAtMM() - timestamp));
}

void geom::Candle::Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const {
    for(std::size_t i = start; i <= end; ++i) {
        auto &item = groupData[i];
        chart.geomShapeFactory_->DrawGeomShape(chart, type_, shapeType_, item, i, i + 1, *this->container_, this->styleConfig_);
    }
}
