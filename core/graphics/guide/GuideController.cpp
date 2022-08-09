#include "GuideController.h"
#include "../util/BBox.h"
#include "../util/json.h"

void xg::guide::GuideController::Render(xg::XChart &chart, canvas::CanvasContext &context) {
    std::for_each(guides.begin(), guides.end(), [&](const std::unique_ptr<xg::guide::GuideBase> &guide) -> void {
        if(guide->GetType() == "background" || !guide->IsTop()) {
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

//void xg::guide::GuideController::Flag(const std::string &json) {
//    FlagObject(xg::json::ParseString(json));
//}

//void xg::guide::GuideController::Text(const std::string &json) {
//    TextObject(xg::json::ParseString(json));
//}

//void xg::guide::GuideController::Line(const std::string &json) {
//    LineObject(xg::json::ParseString(json));
//}

//void xg::guide::GuideController::Background(const std::string &json) {
//    BackgroundObject(xg::json::ParseString(json));
//}

//void xg::guide::GuideController::Image(const std::string &json) {
//    ImageObject(xg::json::ParseString(json));
//}

//void xg::guide::GuideController::Point(const std::string &json) {
//    PointObject(xg::json::ParseString(json));
//}

void xg::guide::GuideController::FlagObject(const FlagCfg &config) {
    auto guide = xg::make_unique<xg::guide::Flag>(config);
    this->guides.push_back(std::move(guide));
}

void xg::guide::GuideController::TextObject(const TextCfg &config) {
    logTracer_->trace("guide#text %s %s %f", config.content.c_str(), config.textColor.c_str(), config.textSize);
    
    auto text = xg::make_unique<xg::guide::Text>(config);
    this->guides.push_back(std::move(text));
}

void xg::guide::GuideController::LineObject(const LineCfg &config) {
    auto line = xg::make_unique<xg::guide::Line>(config);
    this->guides.push_back(std::move(line));
}

void xg::guide::GuideController::BackgroundObject(const BackgroundCfg &config) {
    auto bg = xg::make_unique<xg::guide::Background>(config);
    this->guides.push_back(std::move(bg));
}

void xg::guide::GuideController::ImageObject(const ImageCfg &config) {
    auto image = xg::make_unique<xg::guide::Image>(config);
    this->guides.push_back(std::move(image));
}

void xg::guide::GuideController::PointObject(const PointCfg &config) {
    auto point = xg::make_unique<xg::guide::Point>(config);
    this->guides.push_back(std::move(point));
}
