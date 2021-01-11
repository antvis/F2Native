#include "Pinch.h"
#include "graphics/XChart.h"
#include "utils/xtime.h"

using namespace xg;

interaction::Pinch::Pinch(XChart *chart) : InteractionBase(chart) {
    this->chart_->eventController_->AddCallback("pinchstart", XG_MEMBER_CALLBACK_1(interaction::Pinch::OnPinchStart));
    this->chart_->eventController_->AddCallback("pinch", XG_MEMBER_CALLBACK_1(interaction::Pinch::OnPinch));
    this->chart_->eventController_->AddCallback("pinchend", XG_MEMBER_CALLBACK_1(interaction::Pinch::onPinchEnd));
}

bool interaction::Pinch::OnPinchStart(event::Event &event) {
    chart_->interactionContext_->Start();
    return false;
}

bool interaction::Pinch::OnPinch(event::Event &event) {
    long timestamp = xg::CurrentTimestampAtMM();
    util::Point xAxis = chart_->coord_->GetXAxis();

    double coordWidth = chart_->coord_->GetWidth();

    util::Point center = event.center;

    double leftLen = fabs(center.x - xAxis.x);
    double rightLen = fabs(xAxis.x - center.x);

    double leftScale = leftLen / coordWidth;
    double rightScale = rightLen / coordWidth;

    bool ret = chart_->interactionContext_->DoZoom(leftScale, rightScale, event.zoom);
    chart_->GetLogTracer()->trace("pinch duration: %lu-ms", (xg::CurrentTimestampAtMM() - timestamp));
    return ret;
}

bool interaction::Pinch::onPinchEnd(event::Event &event) {
    chart_->interactionContext_->UpdateTicks();
    return false;
}
