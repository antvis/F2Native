#include "Interval.h"
#include "../XChart.h"
#include "../scale/Scale.h"
#include "../scale/continuous/Linear.h"

using namespace xg;

float geom::Interval::GetDefaultWidthRatio(XChart &chart) {
    if(chart.coord_->GetType() == coord::CoordType::Polar) {
        const std::string &xField = GetXScaleField();
        scale::AbstractScale &xScale = chart.GetScale(xField);
        size_t count = xScale.GetValuesSize();
        return (chart.coord_->IsTransposed() && count > 1) ? 0.75f : 1.0f;
    }
    return this->styleConfig_["widthRatio"];
    // return 0.5f;
}

geom::Interval &geom::Interval::Tag(const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    nlohmann::json defaultCfg = {{"offset", -5}, // 距离柱子顶部的偏移量
                                 {"textAlign", "center"},
                                 {"textBaseline", "bottom"},
                                 {"fill", "#808080"},
                                 {"textSize", DEFAULT_FONTSIZE}};

    if(config.is_object()) {
        defaultCfg.merge_patch(config);
    }
    tagConfig_ = defaultCfg;
    return *this;
}

nlohmann::json geom::Interval::CreateShapePointsCfg(XChart &chart, nlohmann::json &item, size_t index) {

    nlohmann::json rst;
    const std::string &xField = GetXScaleField();
    const std::string &yField = GetYScaleField();

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);

    double x = xScale.Scale(item[xField]);
    if(item[yField].is_array()) {
        vector<double> y;
        vector<double> stack_item = item[yField];
        for(size_t i = 0; i < stack_item.size(); i++) {
            double y_d = stack_item[i];
            double y_s = yScale.Scale(y_d);
            y.push_back(y_s);
        }
        rst["y"] = y;
    } else {
        double y = yScale.Scale(item[yField]);
        rst["y"] = y;
    }
    double y0 = yScale.Scale(this->GetYMinValue(chart));

    std::size_t count = fmax(xScale.GetValuesSize(), 1);
    double normalizeSize = 1.0f;
    //默认系数
    float widthRatio = GetDefaultWidthRatio(chart);
    if(attrs_.find(AttrType::Size) != attrs_.end()) {
        AttrBase &attr = *attrs_[AttrType::Size].get();
        const attr::Size &size = static_cast<const attr::Size &>(attr);
        widthRatio = size.GetSize(0) * chart.GetCanvasContext().GetDevicePixelRatio() / chart.GetCoord().GetWidth();
    } else {
        normalizeSize = 1.0 / count;
        if(xScale.GetType() == scale::ScaleType::Linear || xScale.GetType() == scale::ScaleType::TimeSharingLinear) {
            normalizeSize *= (xScale.rangeMax - xScale.rangeMin);
        }
    }

    normalizeSize *= widthRatio;

    attr::AttrBase *attrBase = GetAttr(attr::AttrType::Adjust).get();
    if(attrBase != nullptr) {
        attr::Adjust *adjust = static_cast<attr::Adjust *>(attrBase);
        if(adjust->GetAdjust() == "dodge") {
            double size = fmax(dataArray_.size(), 1.);
            normalizeSize = normalizeSize / size;
        }
    }

    rst["x"] = x;
    rst["y0"] = y0;
    rst["size"] = normalizeSize;
    return std::move(rst);
}

nlohmann::json geom::Interval::getRectPoints(nlohmann::json &cfg) {
    double x = cfg["x"];
    auto y = cfg["y"];
    double y0 = cfg["y0"];
    double size = cfg["size"];

    double yMin = y0;
    double yMax;
    if(y.is_array()) {
        yMax = y[1];
        yMin = y[0];
    } else {
        yMax = y;
    }
    nlohmann::json rst;
    double xMin = x - size / 2;
    double xMax = x + size / 2;
    rst.push_back({{"x", xMin}, {"y", yMin}});
    rst.push_back({{"x", xMin}, {"y", yMax}});
    rst.push_back({{"x", xMax}, {"y", yMax}});
    rst.push_back({{"x", xMax}, {"y", yMin}});
    return std::move(rst);
}

void geom::Interval::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
//    auto timestamp = xg::CurrentTimestampAtMM();
//    const std::string &yField = this->GetYScaleField();
//    auto &xScale = chart.GetScale(GetXScaleField());
//
//    if(chart.GetCoord().GetType() == coord::CoordType::Polar) {
//        shapeType_ = "sector";
//    }
//
//    for(std::size_t index = 0; index < dataArray.size(); ++index) {
//
//        nlohmann::json &groupData = dataArray[index];
//
//        std::size_t start = 0, end = groupData.size() - 1;
//        if(scale::IsCategory(xScale.GetType())) {
//            start = fmax(start, xScale.min);
//            end = fmin(end, xScale.max);
//        }
//
//        for(std::size_t position = start; position <= end; ++position) {
//            nlohmann::json &item = groupData[position];
//
//            if(!item.contains(yField)) {
//                // 无效点
//                continue;
//            }
//            nlohmann::json yValue = item[yField];
//            nlohmann::json cfg = CreateShapePointsCfg(chart, item, index);
//            nlohmann::json points = getRectPoints(cfg);
//            item["_points"] = points;
//
//            if(item.contains("_beforeMapped")) {
//                continue;
//            }
//
//            if(this->tagConfig_.is_object()) {
//                item["_tag"] = tagConfig_;
//                item["_tag"]["content"] = yValue.dump();
//            }
//
//            item["_style"] = styleConfig_;
//            item["_style"]["content"] = yValue.dump();
//            item["_beforeMapped"] = true;
//        }
//    }
//    chart.GetLogTracer()->trace("Geom#%s Beforemapping duration: %lums", type_.data(), (CurrentTimestampAtMM() - timestamp));
}

void geom::Interval::Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const {
    for(std::size_t i = start; i <= end; ++i) {
        const auto &item = groupData[i];
        chart.geomShapeFactory_->DrawGeomShape(chart, type_, shapeType_, item, i, i + 1, *this->container_, this->connectNulls_);
    }
}
