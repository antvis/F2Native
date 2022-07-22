#include "InteractionContext.h"
#include "../XChart.h"
#include "../util/json_util.h"
#include "../../utils/xtime.h"

using namespace xg;

void xg::interaction::from_json(const nlohmann::json& j, PinchCfg& p) {
    PinchCfg d;
    p.minCount = j.value("minCount", d.minCount);
    p.maxCount = j.value("maxCount", d.maxCount);
    p.enable = true;
}

void xg::interaction::from_json(const nlohmann::json& j, PanCfg& p) {
    p.enable = true;
}


interaction::InteractionContext::InteractionContext(XChart *chart) {
    this->chart_ = chart;
    chart_->AddMonitor(ACTION_CHART_AFTER_INIT, XG_MEMBER_CALLBACK(interaction::InteractionContext::OnAfterChartInit));
}

interaction::InteractionContext::~InteractionContext() { this->chart_ = nullptr; }

void interaction::InteractionContext::OnAfterChartInit() {
    if (!pinch_.enable && !pan_.enable) {
        return;
    }
    const std::string &xField = chart_->GetXScaleField();
    auto &scale = chart_->GetScale(xField);
    this->values_ = scale.values;
    double size = fmax(values_.size(), 1.0);
    this->minScale_ = static_cast<double>(this->minCount_) / size;

    std::size_t _minCount = minCount_;
    std::size_t _maxCount = size;

    if(pinch_.enable) {
        _minCount = std::isnan(pinch_.minCount) ? _minCount : pinch_.minCount;
        _maxCount = std::isnan(pinch_.maxCount) ? _maxCount : pinch_.maxCount;
    }

    range_[0] = (scale.min + 1) / size;
    range_[1] = (scale.max + 1) / size;

    this->maxCount_ = fmin(size, _maxCount);
    this->minCount_ = fmax(minCount_, _minCount);
}

void interaction::InteractionContext::Start() {
    //    this->startRange_ = this->range_;
    const std::string &xField = chart_->GetXScaleField();
    auto &scale = chart_->GetScale(xField);
    lastTickCount_ = scale.tickCount;
    chart_->GetLogTracer()->trace("InteractionContext#Start range:{%lf, %lf} ", range_[0], range_[1]);
}

bool interaction::InteractionContext::DoMove(double deltaX, double deltaY) {
//    chart_->GetLogTracer()->trace("DoMove deltaX %lf ", deltaX);
    // long timestamp = xg::CurrentTimestampAtMM();
    double ratio = deltaX / chart_->GetCoord().GetWidth();

    double rangeStart = range_[0];
    double rangeEnd = range_[1];

    double rangeLen = rangeEnd - rangeStart;
    double rangeOffset = rangeLen * ratio;

    double newRangeStart = rangeStart - rangeOffset;
    double newRangeEnd = rangeEnd - rangeOffset;

    std::array<double, 2> newRange;
    if(newRangeStart < 0) {
        newRange = {0, rangeLen};
    } else if(newRangeEnd > 1) {
        newRange = {1 - rangeLen, 1};
    } else {
        newRange = {newRangeStart, newRangeEnd};
    }
    bool ret = this->UpdateRange(newRange);
    return ret;
}

bool interaction::InteractionContext::DoZoom(double leftScale, double rightScale, double zoom) {
    double rangeStart = range_[0];
    double rangeEnd = range_[1];

    double zoomOffset = 1 - zoom;
    double rangeLen = rangeEnd - rangeStart;
    double rangeOffset = rangeLen * zoomOffset;
    double leftOffset = rangeOffset * leftScale;
    double rightOffset = rangeOffset * rightScale;

    double newRangeStart = fmax(0, rangeStart - leftOffset);
    double newRangeEnd = fmin(1, rangeEnd + rightOffset);

    if(newRangeEnd - newRangeStart < minScale_) {
        return false;
    }

    return this->UpdateRange({newRangeStart, newRangeEnd});
}

bool interaction::InteractionContext::UpdateRange(std::array<double, 2> newRange) {
    double rangeStart = newRange[0];
    double rangeEnd = newRange[1];

    rangeStart = fmax(0, rangeStart);
    rangeEnd = fmin(1, rangeEnd);

    std::size_t valueSize = values_.size();
    std::size_t valueStart = static_cast<std::size_t>(valueSize * rangeStart);
    std::size_t valueEnd = fmin(static_cast<std::size_t>(valueSize * rangeEnd), valueSize - 1);

    // chart_->GetLogTracer()->trace("UpdateRange range: %lu", (valueEnd - valueStart));

    if((valueEnd - valueStart) < minCount_ || (valueEnd - valueStart) > maxCount_) {
        return false;
    }
    range_ = {rangeStart, rangeEnd};
    // 从原始数据里截取需要显示的数据
    auto newValue = JsonArraySlice(values_, valueStart, valueEnd);

    return this->Repaint(newValue, valueStart, valueEnd);
}

bool interaction::InteractionContext::Repaint(nlohmann::json &newValues, std::size_t valueStart, std::size_t valueEnd) {
    const std::string &xField = chart_->GetXScaleField();
    auto &scale = chart_->GetScale(xField);

    if(util::isEqualsQuick(scale.values, newValues))
        return false;

    // chart_->GetLogTracer()->trace("Repaint range: %lu", (valueEnd - valueStart));
    // TODO 平移或者缩放过程中，ticks 的变化应该由每个度量自行决定。 逻辑暂时保持 ticks 不变
    UpdateScale(xField, {{"ticks", scale.ticks}, {"domain", {valueStart, valueEnd}}});
    UpdateFollowScale(scale, newValues, valueStart, valueEnd);
    chart_->Repaint();
    return true;
}

void interaction::InteractionContext::UpdateFollowScale(scale::AbstractScale &pinchScale,
                                                        nlohmann::json &pinchValues,
                                                        std::size_t valueStart,
                                                        std::size_t valueEnd) {

    const std::string &pinchField = pinchScale.field;

    std::string followField = chart_->getYScaleFields()[0];

    // nlohmann::json pinchValuesMap;
    // for(std::size_t index = 0; index < pinchValues.size(); ++index) {
    //     nlohmann::json item = pinchValues[index];
    //     pinchValuesMap[item.dump()] = true;
    // }

    // 根据主轴的 value 值找到从轴的 value 值
    // nlohmann::json values;
    // const std::size_t dataSize = chart_->GetData().size();
    // for(std::size_t index = 0; index < dataSize; ++index) {
    //     nlohmann::json item = chart_->GetData()[index];

    //     nlohmann::json &value = item[pinchField];
    //     if(pinchValuesMap.contains(value.dump())) {
    //         nlohmann::json followValue = item[followField];
    //         values.push_back(followValue);
    //     }
    // }

//    double rangeMin = DBL_MAX, rangeMax = DBL_MIN;
//    std::for_each(chart_->geoms_.begin(), chart_->geoms_.end(), [&](auto &geom) -> void {
//        util::JsonRangeInGeomDataArray(geom->GetDataArray(), followField, valueStart, valueEnd, &rangeMin, &rangeMax);
//    });
//    UpdateScale(followField, {{"min", rangeMin}, {"max", rangeMax}, {"nice", true}});
}

void interaction::InteractionContext::UpdateScale(const std::string &field, nlohmann::json cfg) {
    auto &scale = chart_->GetScale(field);
    scale.Change(cfg);
}

void interaction::InteractionContext::UpdateTicks() {
    //     TODO 手势过程中就完成 ticks 的变化，就不需要在最后再来一次了。
    //    const std::string &xField = chart_->GetXScaleField();
    //    auto &pinchScale = chart_->GetScale(xField);
    //    pinchScale.Change({"ticks", true});
    //    chart_->Repaint();
}
