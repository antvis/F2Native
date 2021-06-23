
#ifndef XG_GRAPHICS_GEOM_SHAPE_INTERVAL_H
#define XG_GRAPHICS_GEOM_SHAPE_INTERVAL_H

#include "graphics/geom/shape/GeomShapeBase.h"
#include "graphics/global.h"
#include "graphics/shape/Rect.h"
#include "graphics/shape/Text.h"
#include <utils/common.h>

namespace xg {
namespace geom {
namespace shape {

class Interval : public GeomShapeBase {
  public:
    Interval() {}
    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const nlohmann::json &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container) override {
        if(!data.contains("_points")) {
            return;
        }

        const nlohmann::json &_points = data["_points"];
        nlohmann::json _style = data["_style"];
        if(_style.contains("custom") && _style["custom"].is_string()) {
            std::string customCallbackId = _style["custom"];
            nlohmann::json param = data;
            param["_index"] = start;
            nlohmann::json customStyle = func::InvokeFunction(customCallbackId, param);
            if(customStyle.is_object()) {
                _style.merge_patch(customStyle);
            }
        };

        std::vector<util::Point> points;
        for(std::size_t i = 0; i < _points.size(); ++i) {
            const nlohmann::json &item = _points[i];
            util::Point p = this->ParsePoint(coord, util::Point(item["x"], item["y"]));
            points.push_back(std::move(p));
        }

        util::CanvasFillStrokeStyle colorStyle = util::ColorParser(data, "_color");

        bool isFill = true;
        if(data.contains("_shape") && data["_shape"].is_string()) {
            if(data["_shape"] == "stroke") {
                isFill = false;
            }
        }

        const float lineWidth = _style["lineWidth"].get<float>() * context.GetDevicePixelRatio();
        // 扇形
        if(shapeType == "sector") {
            std::vector<util::Point> newPoints = points;
            if(coord.IsTransposed()) {
                newPoints = {points[0], points[3], points[2], points[1]};
            }
            Vector2D v = {1, 0};
            Vector2D v0 = {newPoints[0].x - coord.GetCenter().x, newPoints[0].y - coord.GetCenter().y};
            Vector2D v1 = {newPoints[1].x - coord.GetCenter().x, newPoints[1].y - coord.GetCenter().y};
            Vector2D v2 = {newPoints[2].x - coord.GetCenter().x, newPoints[2].y - coord.GetCenter().y};
            double startAngle = Vector2DUtil::AngleTo(v, v1, false);
            double endAngle = Vector2DUtil::AngleTo(v, v2, false);
            double r0 = Vector2DUtil::Length(v0);
            double r = Vector2DUtil::Length(v1);
            if(startAngle >= 1.5 * M_PI) {
                startAngle = startAngle - 2 * M_PI;
            }
            if(endAngle >= 1.5 * M_PI) {
                endAngle = endAngle - 2 * M_PI;
            }

            auto fillRect = xg::make_unique<xg::shape::Rect>(coord.GetCenter(), r, r0, startAngle, endAngle, lineWidth);
            fillRect->fillStyle_ = colorStyle;
            container.AddElement(std::move(fillRect));
            
            if(lineWidth > 0) {
                util::CanvasFillStrokeStyle strokeColor = util::CanvasFillStrokeStyle(_style["stroke"]);
                auto strokeRect = xg::make_unique<xg::shape::Rect>(coord.GetCenter(), r, r0, startAngle, endAngle, lineWidth);
                strokeRect->strokeStyle_ = strokeColor;
                container.AddElement(std::move(strokeRect));
            }
        } else {
            util::Size size(points[2].x - points[0].x, points[2].y - points[0].y);

            auto rect = xg::make_unique<xg::shape::Rect>(points[0], size);
            if(isFill) {
                rect->fillStyle_ = colorStyle;
            } else {
                rect->strokeStyle_ = colorStyle;
                rect->lineWidth_ = lineWidth;
            }
            if(_style.contains("radius")) {
                float roundings[4] = {0, 0, 0, 0};
                json::ParseRoundings(_style["radius"], &roundings[0], context.GetDevicePixelRatio());
                memcpy(rect->roundings, roundings, sizeof(float) * 4);
            }

            container.AddElement(std::move(rect));

            if(!data.contains("_tag") || !data["_tag"].is_object()) {
                return;
            }

            const nlohmann::json &tagCfg = data["_tag"];

            const std::string &content = tagCfg["content"];
            float textSize = tagCfg["textSize"].get<float>() * context.GetDevicePixelRatio();
            const std::string &textColor = tagCfg["fill"];
            int offset = tagCfg["offset"].get<float>() * context.GetDevicePixelRatio();

            util::Point tagPoint{points[1].x + size.width / 2, points[1].y + offset};

            auto tag = xg::make_unique<xg::shape::Text>(content, tagPoint, textSize, textColor, textColor);
            tag->SetTextAlign(tagCfg["textAlign"]);
            tag->SetTextBaseline(tagCfg["textBaseline"]);

            container.AddElement(std::move(tag));
        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_INTERVAL_H
