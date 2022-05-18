#ifndef XG_GRAPHICS_GEOM_SHAPE_POINT_H
#define XG_GRAPHICS_GEOM_SHAPE_POINT_H

#include "GeomShapeBase.h"
#include "../../func/Func.h"
#include "../../shape/Circle.h"

namespace xg {
namespace geom {
namespace shape {
class Point : public GeomShapeBase {
  public:
    Point() {}
    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const nlohmann::json &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              bool connectNulls) override {
        std::string shape = "circle";
        if(data.contains("_shape")) {
            shape = data["_shape"];
        }

        util::Point center = {data["_x"], data["_y"]};
        nlohmann::json style = data["_style"];
        if(style.contains("custom") && style["custom"].is_string()) {
            std::string customCallbackId = style["custom"];
            nlohmann::json param = data;
            param["_index"] = start;
            nlohmann::json customStyle = func::InvokeFunction(customCallbackId, param);
            if(customStyle.is_object()) {
                style.merge_patch(customStyle);
            }
        }

        std::string colorStyle = GLOBAL_COLORS[0];
        if(style.contains("color")) {
            colorStyle = style["color"];
        } else if(data.contains("_color")) {
            colorStyle = data["_color"];
        }

        if(shape == "circle") {
            float radius = 3;
            if(style.contains("size")) {
                radius = style["size"].get<float>();
            } else if(data.contains("_size")) {
                radius = data["_size"].get<float>();
            }
            radius *= context.GetDevicePixelRatio();

            auto circle = std::make_unique<xg::shape::Circle>(center, radius);
            // TODO 兼容 stroke 模式 & linear or radial 模式
            circle->SetFillColor(colorStyle);
            container.AddElement(std::move(circle));
        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
