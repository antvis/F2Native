

#include "graphics/guide/GuideController.h"
#include "graphics/guide/Background.h"
#include "graphics/guide/Flag.h"
#include "graphics/guide/Line.h"
#include "graphics/guide/Text.h"
#include "graphics/util/BBox.h"
#include "graphics/util/json.h"

void xg::guide::GuideController::Render(xg::XChart &chart, canvas::CanvasContext &context) {
    std::for_each(guides.begin(), guides.end(), [&](const std::unique_ptr<xg::guide::GuideBase> &guide) -> void {
        if(guide->GetType() == "background" || !guide->isTop()) {
            guide->Render(chart, backContainer_, context, this->dangerRects);
        } else {
            guide->Render(chart, container_, context, this->dangerRects);
        }
        util::BBox bbox = guide->GetBBox();
        if(bbox.width > 0 && bbox.height > 0) {
            util::Rect rect(bbox.x, bbox.y, bbox.width, bbox.height);
            this->dangerRects.push_back(std::move(rect));
        }
    });
}

void xg::guide::GuideController::Flag(const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    if(!config.is_object())
        return;
    auto guide = xg::make_unique<xg::guide::Flag>(config);
    this->guides.push_back(std::move(guide));
}

void xg::guide::GuideController::Text(const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    if(!config.is_object())
        return;
    auto text = xg::make_unique<xg::guide::Text>(config);
    this->guides.push_back(std::move(text));
}

void xg::guide::GuideController::Line(const std::string &json) {
    nlohmann::json config = xg::json::ParseString(json);
    if(!config.is_object())
        return;
    auto line = xg::make_unique<xg::guide::Line>(config);
    this->guides.push_back(std::move(line));
}

void xg::guide::GuideController::Background(const std::string &json) {
    nlohmann::json cfg = xg::json::ParseString(json);
    if(!cfg.is_object())
        return;
    auto bg = xg::make_unique<xg::guide::Background>(cfg);
    this->guides.push_back(std::move(bg));
}
