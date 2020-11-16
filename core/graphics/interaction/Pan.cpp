
#include "Pan.h"
#include "graphics/XChart.h"

using namespace xg;

interaction::Pan::Pan(XChart *chart) : InteractionBase(chart) {
    this->chart_->eventController_->AddCallback("panstart", XG_MEMBER_CALLBACK_1(interaction::Pan::OnPanStart));
    this->chart_->eventController_->AddCallback("pan", XG_MEMBER_CALLBACK_1(interaction::Pan::OnPan));
    // this->chart_->eventController_->AddCallback("panend", XG_MEMBER_CALLBACK_1(interaction::Pan::onPanEnd));
}

void interaction::Pan::OnPanStart(event::Event &event) {
    this->chart_->GetLogTracer()->trace("%s direction: %s", "ingleLineChart_1#onPanStart", event.direction.data());
    this->chart_->interactionContext_->Start();
}

void interaction::Pan::OnPan(event::Event &event) {
    this->chart_->GetLogTracer()->trace("%s direction: %s deltaX: %f", "onPan", event.direction.data(), event.deltaX);
    if(event.direction == "none" || event.direction == "up" || event.direction == "down") {
        return;
    }

    double coordWidth = this->chart_->coord_->GetWidth();
    double ratio = event.deltaX / coordWidth;
    this->chart_->interactionContext_->DoMove(ratio);
}

// void interaction::Pan::onPanEnd(event::Event &event) {
//     this->chart_->GetLogTracer()->trace("%s direction: %s", "ingleLineChart_1#onPanEnd", event.direction.data());
// }
