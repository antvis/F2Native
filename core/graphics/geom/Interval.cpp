#include "Interval.h"
#include "../XChart.h"
#include "../scale/Scale.h"
#include "../scale/continuous/Linear.h"

using namespace xg;

void xg::util::from_json(const nlohmann::json &j, TagCfg &t) {
    if (!j.is_object()) {
        return;
    }
    TagCfg d;
    t.offset = j.value("offset", d.offset);
    t.textAlign = j.value("textAlign", d.textAlign);
    t.textBaseline = j.value("textBaseline", d.textBaseline);
    t.fill = j.value("fill", d.fill);
    t.textSize = j.value("textSize", d.textSize);
    t.hiden = false;
}

float geom::Interval::GetDefaultWidthRatio(XChart &chart) {
    if(chart.coord_->GetType() == coord::CoordType::Polar) {
        const std::string &xField = GetXScaleField();
        scale::AbstractScale &xScale = chart.GetScale(xField);
        size_t count = xScale.GetValuesSize();
        return (chart.coord_->IsTransposed() && count > 1) ? 0.75f : 1.0f;
    }
    return this->styleConfig_.widthRatio;
    // return 0.5f;
}

geom::Interval &geom::Interval::Tag(const TagCfg &cfg) {
    tagConfig_ = cfg;
    return *this;
}

unordered_map<string, double> geom::Interval::CreateShapePointsCfg(XChart &chart, XData &item, size_t index) {
    const std::string &xField = GetXScaleField();
    const std::string &yField = GetYScaleField();

    scale::AbstractScale &xScale = chart.GetScale(xField);
    scale::AbstractScale &yScale = chart.GetScale(yField);
    
    auto &data = item.data;
    double x = item.dodge.size() >= 1 ? xScale.Scale(item.dodge[0]) : xScale.Scale(data[xField]);
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
            normalizeSize *= xScale.config.HasRange() ? (xScale.config.range[1] - xScale.config.range[0]) : 1;
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
    return {{"x", x} , {"y0", y0}, {"size", normalizeSize}};
}

vector<double> geom::Interval::CreateShapePoints(XChart &chart, XData &item, size_t index) {
    const std::string &yField = GetYScaleField();
    scale::AbstractScale &yScale = chart.GetScale(yField);
    
    auto &data = item.data;
    vector<double> y;
    if (!item.adjust.empty()) {
        for(size_t i = 0; i < item.adjust.size(); i++) {
            double y_d = item.adjust[i];
            double y_s = yScale.Scale(y_d);
            y.push_back(y_s);
        }
    } else if(data[yField].GetType().IsArray()) { //区间柱状图
        vector<double> stack_item = data[yField].Cast<vector<double>>();
        for(size_t i = 0; i < stack_item.size(); i++) {
            double y_d = stack_item[i];
            double y_s = yScale.Scale(y_d);
            y.push_back(y_s);
        }

    } else {
        y.push_back(yScale.Scale(data[yField]));
    }
    return y;
}

vector<util::Point> geom::Interval::getRectPoints(const unordered_map<string, double> &cfg, const vector<double> &y) {
    double x = cfg.at("x");
    double y0 = cfg.at("y0");
    double size = cfg.at("size");

    double yMin = y0;
    double yMax;
    if(y.size() >= 2) {
        yMax = y[1];
        yMin = y[0];
    } else {
        yMax = y[0];
    }
    vector<util::Point> rst;
    double xMin = x - size / 2;
    double xMax = x + size / 2;
    rst.push_back(util::Point {xMin, yMin});
    rst.push_back(util::Point {xMin, yMax});
    rst.push_back(util::Point {xMax, yMax});
    rst.push_back(util::Point {xMax, yMin});
    return rst;
}

void geom::Interval::BeforeMapping(XChart &chart, XDataGroup &dataArray) {
    auto timestamp = xg::CurrentTimestampAtMM();
    const std::string &yField = this->GetYScaleField();
    auto &xScale = chart.GetScale(GetXScaleField());

    if(chart.GetCoord().GetType() == coord::CoordType::Polar) {
        shapeType_ = "sector";
    }

    for(std::size_t index = 0; index < dataArray.size(); ++index) {

        auto &groupData = dataArray[index];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.GetMin());
            end = fmin(end, xScale.GetMax());
        }

        for(std::size_t position = start; position <= end; ++position) {
            auto &item = groupData[position];
            
            if (!item.data.count(yField)) {
                continue;;
            }
     
            const auto &yValue = item.data[yField];
            auto cfg = CreateShapePointsCfg(chart, item, index);
            auto points = CreateShapePoints(chart, item, index);
            item.points = getRectPoints(cfg, points);

            if(item._beforeMapped) {
                continue;
            }
            
            if (tagConfig_.hiden) {
                continue;
            }

            item._tag = tagConfig_;
            if (yValue.IsFloatingNumber()) {
                item._tag.content = std::to_string(yValue.Cast<double>());
            } else if (yValue.IsNumber()) {
                item._tag.content = std::to_string(yValue.Cast<long>());
            } else if (yValue.IsString()) {
                item._tag.content = yValue.Cast<std::string>();
            } else {
                F2ASSERT(false, "interval tag has wrong type");
            }
            item._beforeMapped = true;
        }
    }
    chart.GetLogTracer()->trace("Geom#%s Beforemapping duration: %lums", type_.data(), (CurrentTimestampAtMM() - timestamp));
}

void geom::Interval::Draw(XChart &chart, const XDataArray &groupData, std::size_t start, std::size_t end) const {
    for(std::size_t i = start; i <= end; ++i) {
        const auto &item = groupData[i];
        chart.geomShapeFactory_->DrawGeomShape(chart, type_, shapeType_, item, i, i + 1, *this->container_, this->styleConfig_);
    }
}
