
#include "InteractionContext.h"
#include "graphics/XChart.h"
#include "graphics/util/json_util.h"
#include "utils/xtime.h"

using namespace xg;

interaction::InteractionContext::InteractionContext(XChart *chart) {
    this->chart_ = chart;
    chart_->AddMonitor(ACTION_CHART_AFTER_INIT, XG_MEMBER_CALLBACK(interaction::InteractionContext::OnAfterChartInit));
}

interaction::InteractionContext::~InteractionContext() { this->chart_ = nullptr; }

void interaction::InteractionContext::OnAfterChartInit() {
    const std::string &xField = chart_->GetXScaleField();
    auto &scale = chart_->GetScale(xField);
    this->values_ = scale.values;

    double size = fmax(values_.size(), 1);
    this->minScale_ = this->minCount_ / size;
}

void interaction::InteractionContext::Start() {
    this->startRange_ = this->range_;
    const std::string &xField = chart_->GetXScaleField();
    auto &scale = chart_->GetScale(xField);
    lastTickCount_ = scale.tickCount;
}

void interaction::InteractionContext::DoMove(double ratio) {
    long timestamp = xg::CurrentTimestampAtMM();
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
    this->UpdateRange(newRange);

    chart_->GetLogTracer()->trace("DoMove duration: %lu-ms", (xg::CurrentTimestampAtMM() - timestamp));
}

void interaction::InteractionContext::DoZoom(double leftScale, double rightScale, double zoom) {
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
        return;
    }

    this->UpdateRange({newRangeStart, newRangeEnd});
}

void interaction::InteractionContext::UpdateRange(std::array<double, 2> newRange) {
    double rangeStart = newRange[0];
    double rangeEnd = newRange[1];

    rangeStart = fmax(0, rangeStart);
    rangeEnd = fmin(1, rangeEnd);

    range_ = {rangeStart, rangeEnd};

    std::size_t valueSize = values_.size();
    int valueStart = static_cast<int>(valueSize * rangeStart);
    int valueEnd = static_cast<int>(valueSize * rangeEnd);

    // 从原始数据里截取需要显示的数据
    auto newValue = JsonArraySlice(values_, valueStart, valueEnd);

    chart_->GetLogTracer()->trace("UpdateRange %s", newValue.dump().data());
    this->Repaint(newValue);
}

void interaction::InteractionContext::Repaint(nlohmann::json &newValues) {
    const std::string &xField = chart_->GetXScaleField();
    auto &scale = chart_->GetScale(xField);

    if(scale.values.size() == newValues.size())
        return;

    UpdateScale(xField, {/*{"ticks", true}, */ {"values", newValues}});
    UpdateFollowScale(scale, newValues);
    chart_->Repaint();
}

void interaction::InteractionContext::UpdateFollowScale(scale::AbstractScale &pinchScale, nlohmann::json &pinchValues) {

    const std::string &pinchField = pinchScale.field;

    std::string followField = chart_->getYScaleFields()[0];

    nlohmann::json pinchValuesMap;
    for(std::size_t index = 0; index < pinchValues.size(); ++index) {
        nlohmann::json item = pinchValues[index];
        pinchValuesMap[item.dump()] = true;
    }

    // 根据主轴的 value 值找到从轴的 value 值
    nlohmann::json values;
    std::size_t dataSize = chart_->GetData().size();
    for(std::size_t index = 0; index < dataSize; ++index) {
        nlohmann::json item = chart_->GetData()[index];

        nlohmann::json &value = item[pinchField];
        if(pinchValuesMap.contains(value.dump())) {
            nlohmann::json followValue = item[followField];
            values.push_back(followValue);
        }
    }

    auto range = JsonArrayRange(values);
    UpdateScale(followField, {{"min", range[0]}, {"max", range[1]}});
}

void interaction::InteractionContext::UpdateScale(const std::string &field, nlohmann::json cfg) {
    auto &scale = chart_->GetScale(field);
    scale.Change(cfg);
}

void interaction::InteractionContext::UpdateTicks() {
    // TODO tickCount 属性还有问题，待度量优化后实现
}
