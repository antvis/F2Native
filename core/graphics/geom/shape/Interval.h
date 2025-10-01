#ifndef XG_GRAPHICS_GEOM_SHAPE_INTERVAL_H
#define XG_GRAPHICS_GEOM_SHAPE_INTERVAL_H

#include "GeomShapeBase.h"
#include "../../global.h"
#include "../../shape/Rect.h"
#include "../../shape/Text.h"
#include "../../../utils/common.h"

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
              xg::shape::Group &container,
              bool connectNulls) override {
        if(!data.contains("_points")) {
            return;
        }

        const nlohmann::json &_points = data["_points"];
        nlohmann::json _style = data["_style"];

        std::vector<util::Point> points;
        for(std::size_t i = 0; i < _points.size(); ++i) {
            const nlohmann::json &item = _points[i];
            util::Point p = this->ParsePoint(coord, util::Point(item["x"], item["y"]));
            points.push_back(std::move(p));
        }

        bool isFill = true;
        if(data.contains("_shape") && data["_shape"].is_string()) {
            if(data["_shape"] == "stroke") {
                isFill = false;
            }
        }

        float lineWidth = 0;
        if(_style.contains("lineWidth") && _style["lineWidth"].is_number()) {
            lineWidth = _style["lineWidth"].get<float>() * context.GetDevicePixelRatio();
        }
        
        float rZoomFactor = 1;
        if(_style.contains("rZoomFactor") && _style["rZoomFactor"].is_number()) {
            rZoomFactor = _style["rZoomFactor"].get<float>();
        }
        
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

            // 如果外部传入内圈半径，则以外部半径为准
            if(_style.contains("r0") && _style["r0"].is_number()) {
                r0 = _style["r0"].get<double>() * context.GetDevicePixelRatio();
            }
            // 如果外部传入外圈半径，则以外部半径为准
            if(_style.contains("r") && _style["r"].is_number()) {
                r = _style["r"].get<double>() * context.GetDevicePixelRatio();
            }
            
            auto fillRect = xg::make_unique<xg::shape::Rect>(coord.GetCenter(), r * rZoomFactor, r0, startAngle, endAngle, lineWidth);
            
            BBox bbox = BBoxUtil::GetBBoxFromPoints({coord.GetStart(), coord.GetEnd()}, .0f);
            const bool isSelected = json::GetBool(data, "_isSelected", false);
            canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(data, isSelected ? "_selectedColor" : "_color", &bbox);
            
            fillRect->SetFillStyle(colorStyle);
            container.AddElement(std::move(fillRect));

            if(lineWidth > 0) {
                canvas::CanvasFillStrokeStyle strokeColor = canvas::CanvasFillStrokeStyle(_style["stroke"]);
                auto strokeRect = xg::make_unique<xg::shape::Rect>(coord.GetCenter(), r * rZoomFactor, r0, startAngle, endAngle, lineWidth);
                strokeRect->SetStorkStyle(strokeColor);
                container.AddElement(std::move(strokeRect));
            }
        } else {
            util::Size size(points[2].x - points[0].x, points[2].y - points[0].y);

            auto rect = xg::make_unique<xg::shape::Rect>(points[0], size);
            xg::util::BBox bbox = rect->GetBBox(context);

            const bool isSelected = json::GetBool(data, "_isSelected", false);
            canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(data, isSelected ? "_selectedColor" : "_color", &bbox);

            if(isFill) {
                rect->SetFillStyle(colorStyle);
            } else {
                rect->SetStorkStyle(colorStyle);
                rect->SetLineWidth(lineWidth);
            }
            if(_style.contains("radius")) {
                float roundings[4] = {0, 0, 0, 0};
                json::ParseRoundings(_style["radius"], &roundings[0], context.GetDevicePixelRatio());
                rect->SetRoundings(roundings);
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
