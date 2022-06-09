#include "Pan.h"
#include "../XChart.h"

using namespace xg;

interaction::Pan::Pan(XChart *chart) : InteractionBase(chart) {
    this->chart_->eventController_->AddCallback("panstart", XG_MEMBER_CALLBACK_1(interaction::Pan::OnPanStart));
    this->chart_->eventController_->AddCallback("pan", XG_MEMBER_CALLBACK_1(interaction::Pan::OnPan));
    this->chart_->eventController_->AddCallback("panend", XG_MEMBER_CALLBACK_1(interaction::Pan::onPanEnd));
}

bool interaction::Pan::OnPanStart(event::Event &event) {
    this->chart_->interactionContext_->Start();
    return false;
}

bool interaction::Pan::OnPan(event::Event &event) {
    // this->chart_->GetLogTracer()->trace("onPan type: %s direction: %s", event.eventType.data(), event.direction.data());
    if(event.direction == "none" || event.direction == "up" || event.direction == "down") {
        return false;
    }

    // double coordWidth = this->chart_->coord_->GetWidth();
    // double ratio = event.deltaX / coordWidth;
    return this->chart_->interactionContext_->DoMove(event.deltaX, event.deltaY);
}

bool interaction::Pan::onPanEnd(event::Event &event) {
    this->chart_->interactionContext_->UpdateTicks();
    return false;
}
