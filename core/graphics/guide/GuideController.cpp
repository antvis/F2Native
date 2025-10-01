

#include "graphics/guide/GuideController.h"
#include "graphics/guide/Background.h"
#include "graphics/guide/Flag.h"
#include "graphics/guide/Line.h"
#include "graphics/guide/Text.h"
#include "graphics/guide/Image.h"
#include "graphics/guide/Point.h"
#include "graphics/guide/Tag.h"
#include "graphics/util/BBox.h"
#include "graphics/util/json.h"
#include "graphics/guide/RefLine.h"

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
    FlagObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::Text(const std::string &json) {
    TextObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::Line(const std::string &json) {
    LineObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::Background(const std::string &json) {
    BackgroundObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::Image(const std::string &json) {
    ImageObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::Point(const std::string &json) {
    PointObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::Tag(const std::string &json) {
    TagObject(xg::json::ParseString(json));
}

void xg::guide::GuideController::RefLine(const std::string &json) {
    RefLine(xg::json::ParseString(json));
}

void xg::guide::GuideController::FlagObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto guide = xg::make_unique<xg::guide::Flag>(config);
    this->guides.push_back(std::move(guide));
}

void xg::guide::GuideController::TextObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto text = xg::make_unique<xg::guide::Text>(config);
    this->guides.push_back(std::move(text));
}

void xg::guide::GuideController::LineObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto line = xg::make_unique<xg::guide::Line>(config);
    this->guides.push_back(std::move(line));
}

void xg::guide::GuideController::BackgroundObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto bg = xg::make_unique<xg::guide::Background>(config);
    this->guides.push_back(std::move(bg));
}

void xg::guide::GuideController::ImageObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto image = xg::make_unique<xg::guide::Image>(config);
    this->guides.push_back(std::move(image));
}

void xg::guide::GuideController::PointObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto point = xg::make_unique<xg::guide::Point>(config);
    this->guides.push_back(std::move(point));
}

void xg::guide::GuideController::TagObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto tag = xg::make_unique<xg::guide::Tag>(config);
    this->guides.push_back(std::move(tag));
}

void xg::guide::GuideController::RefLineObject(const nlohmann::json &config) {
    if(!config.is_object())
        return;
    auto tag = xg::make_unique<xg::guide::RefLine>(config);
    this->guides.push_back(std::move(tag));
}
