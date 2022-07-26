#include "Candle.h"
#include "../XChart.h"
#include "../scale/Scale.h"

using namespace xg;

nlohmann::json geom::Candle::CreateShapePointsCfg(XChart &chart, XData &item, size_t index) {

    nlohmann::json rst;
    const std::string &xField = GetXScaleField();
    const std::string &yField = GetYScaleField();

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = xScale.Scale((*item.data)[xField]);
    std::size_t state = 0; // 0 - 平, 1 涨， -1 跌

    auto &yVal = (*item.data)[yField];
    if(!yVal.is_array() || yVal.empty() || yVal.size() < 4) {
        return {{"x", x}, {"y0", 0}, {"y", 0}, {"size", 0}, {"state", 0}, {"max", 0}, {"min", 0}};
    }

    vector<double> y;
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

    y.push_back(start);
    y.push_back(end);

    std::size_t count = fmax(xScale.GetValuesSize(), 1);
    double normalizeSize = 1.0 / count;

    normalizeSize *= 0.9;

    rst["x"] = x;
    rst["y"] = y;
    rst["size"] = normalizeSize;
    rst["state"] = state;
    rst["max"] = max;
    rst["min"] = min;

    return rst;
}

nlohmann::json geom::Candle::getRectPoints(nlohmann::json &cfg) {
    double x = cfg["x"];
    auto y = cfg["y"];
    double size = cfg["size"];

    const int state = cfg["state"];

    double yMax;
    double yMin;

    if(state >= 0) { // 涨, 平
        yMax = y[0];
        yMin = y[1];
    } else { // 跌
        yMax = y[1];
        yMin = y[0];
    }

    nlohmann::json rst;
    double xMin = x - size / 2;
    double xMax = x + size / 2;
    rst.push_back({{"x", xMin}, {"y", yMin}});
    rst.push_back({{"x", xMin}, {"y", yMax}});
    rst.push_back({{"x", xMax}, {"y", yMax}});
    rst.push_back({{"x", xMax}, {"y", yMin}});
    return rst;
}

nlohmann::json geom::Candle::getLinePoints(nlohmann::json &cfg) {
    double max = cfg["max"];
    double min = cfg["min"];
    nlohmann::json rst;
    rst.push_back(max);
    rst.push_back(min);
    return rst;
}

void geom::Candle::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
    auto timestamp = xg::CurrentTimestampAtMM();
    const std::string &yField = this->GetYScaleField();
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
            nlohmann::json cfg = CreateShapePointsCfg(chart, item, index);
            nlohmann::json rect = getRectPoints(cfg);
            nlohmann::json line = getLinePoints(cfg);

            item._rect = rect;
            item._line = line;
            item._state = cfg["state"];
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
