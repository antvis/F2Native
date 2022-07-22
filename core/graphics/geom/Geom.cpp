#include "Geom.h"
#include "../XChart.h"
#include "../adjust/Dodge.h"
#include "../adjust/Stack.h"
#include "../global.h"
#include "../util/Point.h"
#include "../util/json_util.h"
#include "../../utils/StringUtil.h"

using namespace std;

xg::geom::AbstractGeom::~AbstractGeom() {
    container_ = nullptr;
    attrs_.clear();
}

#pragma mark public
xg::geom::AbstractGeom &xg::geom::AbstractGeom::Position(const string &field) {
    this->tracker_->trace("geom#%s  Position: %s", type_.c_str(), field.c_str());
    vector<string> fields(StringUtil::ParseFields(field));
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Position>(fields);
    attrs_[AttrType::Position] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Color(const string &field, const vector<string> &colors) {
    this->tracker_->trace("geom#%s  Color: %s colors: %lu", type_.c_str(), field.c_str(), colors.size());
    vector<string> fields(StringUtil::ParseFields(field));
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Color>(fields, colors.empty() ? COLORS : colors);
    attrs_[AttrType::Color] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Color(const string &color) {
    this->tracker_->trace("geom#%s  Color: %s ", type_.c_str(), color.c_str());
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Color>(color);
    attrs_[AttrType::Color] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Size(const string &field, const vector<float> &sizes) {
    this->tracker_->trace("geom#%s  Size: %s sizes: %lu", type_.c_str(), field.c_str(), sizes.size());
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Size>(field, sizes.empty() ? GLOBAL_SIZES : sizes);

    attrs_[AttrType::Size] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Size(const float size) {
    this->tracker_->trace("geom#%s  Size: %.2f ", type_.c_str(), size);
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Size>(size);
    attrs_[AttrType::Size] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Shape(const string &field, const vector<string> &shapes) {
    this->tracker_->trace("geom#%s  shape: %s shapes %lu ", type_.c_str(), field.c_str(), shapes.size());
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Shape>(field, shapes);
    attrs_[AttrType::Shape] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Shape(const string &shape) {
    this->tracker_->trace("geom#%s  Shape: %s ", type_.c_str(), shape.c_str());
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Shape>(shape);
    attrs_[AttrType::Shape] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Adjust(const string &adjust) {
    this->tracker_->trace("geom#%s  adjust: %s ", type_.c_str(), adjust.c_str());
    std::unique_ptr<attr::AttrBase> attr = xg::make_unique<attr::Adjust>("", adjust);
    attrs_[AttrType::Adjust] = std::move(attr);
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Style(const std::string &json) {
    return StyleObject(xg::json::ParseString(json));
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::Attrs(const std::string &attrs) {
    return AttrsObject(xg::json::ParseString(attrs));
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::StyleObject(const nlohmann::json &cfg) {
    if(cfg.is_object()) {
        this->styleConfig_.merge_patch(cfg);
    }
    return *this;
}

xg::geom::AbstractGeom &xg::geom::AbstractGeom::AttrsObject(const nlohmann::json &cfg) {
    if(cfg.is_object() && cfg.size() > 0) {
        if(cfg.contains("connectNulls") && cfg["connectNulls"].is_boolean()) {
            this->connectNulls_ = cfg["connectNulls"];
        }
    }
    return *this;
}



const unique_ptr<xg::attr::AttrBase> &xg::geom::AbstractGeom::GetColor() { return attrs_[xg::attr::AttrType::Color]; }

const std::string &xg::geom::AbstractGeom::GetXScaleField() { return attrs_[AttrType::Position]->GetFields()[0]; }

const std::string &xg::geom::AbstractGeom::GetYScaleField() { return attrs_[AttrType::Position]->GetFields()[1]; }

void xg::geom::AbstractGeom::Init(XChart *chart) {
    // InitAttrs(*chart);
    this->tracker_->trace("geom#init %s", type_.c_str());
    InitAttributes(*chart);
    ProcessData(*chart);
}

#pragma mark protected
void xg::geom::AbstractGeom::InitAttributes(XChart &chart) {
    if(this->attrs_.find(AttrType::Position) == this->attrs_.end()) {
        return;
    }

    if(this->attrs_.find(AttrType::Adjust) == this->attrs_.end()) {
        return;
    }
    std::unique_ptr<attr::AttrBase> &adjustAttr = attrs_[attr::AttrType::Adjust];
    attr::Adjust *adjust = static_cast<attr::Adjust *>(adjustAttr.get());
    if(chart.GetCoord().IsTransposed() && chart.GetCoord().GetType() == xg::canvas::coord::CoordType::Polar && adjust->GetAdjust() == "stack") {
        auto &yScale = chart.GetScale(this->GetYScaleField());
        if(yScale.values.size() > 0) {
            yScale.Change({{"nice", false}, {"min", 0}, {"max", xg::util::JsonArrayMax(yScale.values)}});
        }
    }
}

void xg::geom::AbstractGeom::ProcessData(XChart &chart) {
    auto timestamp = xg::CurrentTimestampAtMM();
    dataArray_ = GroupData(chart);

    std::unique_ptr<attr::AttrBase> &adjustAttr = attrs_[attr::AttrType::Adjust];
    attr::AttrBase *base = adjustAttr.get();
    if(base != nullptr) {
        attr::Adjust *adjust = static_cast<attr::Adjust *>(base);
        if(adjust->GetAdjust() == "dodge") {
            // numberic
            auto xField = GetXScaleField();
            auto yField = GetYScaleField();
            auto &xScale = chart.GetScale(xField);
            auto &yScale = chart.GetScale(yField);
            for(std::size_t groupIdx = 0; groupIdx < dataArray_.size(); ++groupIdx) {
                auto &group = dataArray_[groupIdx];
                for(std::size_t index = 0; index < group.size(); ++index) {
                    auto &item = group[index];
                    if(scale::IsCategory(xScale.GetType()) && (*item.data).contains(xField)) {
                        scale::Category &catScale = static_cast<scale::Category &>(xScale);
                        item.dodge.push_back( catScale.Transform((*item.data)[xField]));
                    }

                    if(scale::IsCategory(yScale.GetType()) && (*item.data).contains(yField)) {
                        scale::Category &catScale = static_cast<scale::Category &>(yScale);
                        item.dodge.push_back( catScale.Transform((*item.data)[yField]));
                    }
                }
            }
        }

        if(adjust->GetAdjust() == "stack") {
            updateStackRange(chart);
        } else if(adjust->GetAdjust() == "dodge") {
            // TODO 补充输入 marginRatio 参数
            adjust::Dodge::processDodge(GetXScaleField(), GetYScaleField(), dataArray_);
        }
    }

    // // TODO 是否需要对数据进行排序/是否需要对数据补空

    this->tracker_->trace("geom#Processdata %s -- finished %lums", type_.c_str(), (xg::CurrentTimestampAtMM() - timestamp));
}

XDataGroup xg::geom::AbstractGeom::GroupData(XChart &chart) {
    const nlohmann::json &data = chart.GetData();

    const set<string> fields(GetGroupFieldNames(chart));
    if(fields.empty()) {
        auto xField = chart.GetXScaleField();
        XDataArray ary;
        for(size_t index = 0, size = data.size(); index < size; ++index) {            
            //过滤掉没有x轴key的数据
            if(data[index].contains(xField)) {
                ary.emplace_back(XData{.data = &data[index]});
            }
        }
        return XDataGroup({std::move(ary)});
    }else {
        return util::JsonGroupByFields(data, fields);
    }
}

const set<string> xg::geom::AbstractGeom::GetGroupFieldNames(XChart &chart) {
    set<string> fieldNames;
    for(xg::attr::AttrType type : GROUP_ATTRS) {
        for(auto &it : attrs_) {
            if(it.second.get() == nullptr)
                continue;
            if(it.first == type) {
                auto &attr = it.second;
                auto &_fields = attr->GetFields();
                std::for_each(_fields.begin(), _fields.end(), [&](const string &field) {
                    if(scale::IsCategory(chart.GetScale(field).GetType())) {
                        fieldNames.insert(field);
                    }
                });
            }
        }
    }
    return fieldNames;
}

void xg::geom::AbstractGeom::Paint(XChart *chart) {
    if(dataArray_.size() <= 0) {
        this->tracker_->trace("geom#%s Paint end. dataArray is empty", type_.c_str());
        return;
    }

    this->BeforeMapping(*chart, dataArray_);

    auto timestamp = CurrentTimestampAtMM();
    auto &xScale = chart->GetScale(GetXScaleField());

    for(std::size_t i = 0; i < dataArray_.size(); ++i) {
        XDataArray &groupData = dataArray_[i];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.min);
            end = fmin(end, xScale.max);
        }

        Mapping(*chart, groupData, start, end);

        Draw(*chart, groupData, start, end);
    }

    this->tracker_->trace("geom#%s Paint finished. duration: %lums ", type_.c_str(), (CurrentTimestampAtMM() - timestamp));
}

void xg::geom::AbstractGeom::Mapping(XChart &chart, XDataArray &dataArray, std::size_t start, std::size_t end) {
    this->tracker_->trace("geom#%s start mapping, size: %lu", type_.c_str(), dataArray.size());

    for(auto &it : attrs_) {
        if(it.second.get() == nullptr) {
            continue;
        }
        AttrBase &attr = *it.second.get();
        const vector<string> &fields = attr.GetFields();
        AbstractScale &xScale = chart.GetScale(fields.size() >= 1 ? fields[0] : "");
        AbstractScale &yScale = chart.GetScale(fields.size() >= 2 ? fields[1] : "");
        attr.Mapping(dataArray, start, end, xScale, yScale, chart.GetCoord());
    }
}

void xg::geom::AbstractGeom::Draw(XChart &chart, const XDataArray &dataArray, std::size_t start, std::size_t end) const {
    chart.geomShapeFactory_->DrawGeomShape(chart, this->type_, /*subShapeType*/ "", dataArray, start, end, *container_, this->connectNulls_);
}

bool xg::geom::AbstractGeom::ContainsAttr(attr::AttrType type) {
    for(auto &it : attrs_) {
        if(it.second.get()) {
            attr::AttrBase &attr = *it.second.get();
            if(attr.GetType() == type) {
                return true;
            }
        }
    }
    return false;
}

double xg::geom::AbstractGeom::GetYMinValue(XChart &chart) {
    const std::string &yField = GetYScaleField();
    auto &yScale = chart.GetScale(yField);
    double _min = yScale.min;
    double _max = yScale.max;
    double value = _min;
    if(styleConfig_.contains("startOnZero") && styleConfig_["startOnZero"] == true) {
        if(_max <= 0 && _min <= 0) {
            value = _max;
        } else {
            value = _min >= 0 ? _min : 0;
        }
    }
    return value;
}

#pragma mark - Stack
void xg::geom::AbstractGeom::updateStackRange(XChart &chart) {
    const std::string &yField = GetYScaleField();
    const std::string &xField = GetXScaleField();
    scale::AbstractScale &yScale = chart.GetScale(yField);
    adjust::Stack::processStack(xField, yField, dataArray_);
    double min = yScale.min;
    double max = yScale.max;
    for(size_t i = 0; i < dataArray_.size(); i++) {
        auto &data = dataArray_[i];
        for(size_t j = 0; j < data.size(); j++) {
            auto &item = data[j];
            if(item.adjust.size() == 2) {
                min = fmin(min, item.adjust[0]);
                max = fmax(max, item.adjust[1]);
            }
        }
    }
    if(min < yScale.min || max > yScale.max) {
        yScale.Change({{"min", min}, {"max", max}});
    }
}

XDataArray xg::geom::AbstractGeom::GetSnapRecords(XChart *chart, util::Point point) {
    util::Point invertPoint = chart->GetCoord().InvertPoint(point);

    auto &xScale = chart->GetScale(GetXScaleField());
    //    auto &yScale = chart->GetScale(GetYScaleField());

    if(chart->GetCoord().GetType() == coord::CoordType::Polar && !chart->GetCoord().IsTransposed() &&
       invertPoint.x > (xScale.rangeMax + 1) / 2) {
        invertPoint.x = xScale.rangeMin;
    }

    nlohmann::json xValue = xScale.Invert(invertPoint.x);

    if(!scale::IsCategory(xScale.GetType())) {
        // TODO _getSnap
    }

    XDataArray tmp;

    auto &xField = GetXScaleField();
    for(std::size_t num = 0; num < dataArray_.size(); ++num) {
        auto &groupData = dataArray_[num];

        std::size_t start = 0, end = groupData.size() - 1;
        if(scale::IsCategory(xScale.GetType())) {
            start = fmax(start, xScale.min);
            end = fmin(end, xScale.max);
        }

        for(size_t index = start; index <= end; index++) {
            auto &item = groupData[index];
            if((*item.data).contains(xField) && (*item.data)[xField] == xValue) {
                tmp.push_back(item);
            }
        }
    }

    // TODO speical for pie chart.

    return tmp;
}

const XData &xg::geom::AbstractGeom::GetLastSnapRecord(XChart *chart) {
    auto &xScale = chart->GetScale(GetXScaleField());
    //说明分组了
    if (dataArray_.size() > 1) {
        return dataArray_.back().back();
    } else {
        std::size_t end = scale::IsCategory(xScale.GetType()) ? fmax(0 , xScale.max): (dataArray_.front().size() - 1);
        return dataArray_.front()[end];
    }
}

const XData &xg::geom::AbstractGeom::GetFirstSnapRecord(XChart *chart) {
    auto &xScale = chart->GetScale(GetXScaleField());
    std::size_t start = scale::IsCategory(xScale.GetType()) ? fmax(0, xScale.min) : 0;
    return dataArray_.front()[start];
}

void xg::geom::AbstractGeom::Clear() {
    if(this->container_ != nullptr) {
        this->container_->Clear();
    }
}

void xg::geom::AbstractGeom::SetAttrs(const std::string &_attrs) noexcept {
    nlohmann::json cfg = xg::json::ParseString(_attrs);
    if(cfg.is_object() && cfg.size() > 0) {
        if(cfg.contains("connectNulls") && cfg["connectNulls"].is_boolean()) {
            this->connectNulls_ = cfg["connectNulls"];
        }
    }
}
