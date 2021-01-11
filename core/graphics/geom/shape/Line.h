#ifndef XG_GRAPHICS_GEOM_SHAPE_LINE_H
#define XG_GRAPHICS_GEOM_SHAPE_LINE_H

#include "graphics/geom/shape/GeomShapeBase.h"
#include "graphics/global.h"
#include "graphics/shape/Polyline.h"
#include "graphics/util/Point.h"
#include <vector>

using namespace xg::util;

namespace xg {
namespace geom {
namespace shape {

class Line : public GeomShapeBase {
  public:
    Line() {}

    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const nlohmann::json &data,
              xg::shape::Group &container) override {
        this->drawLines(data, context, container);
    }

  private:
    // 还缺一个style
    void drawLines(const nlohmann::json &data, canvas::CanvasContext &canvasContext, xg::shape::Group &container) {
        size_t size = data.size();
        if(size <= 0)
            return;

        std::string color = GLOBAL_COLORS[0];
        if(size > 0 && data[0].contains("_color")) {
            color = data[0]["_color"];
        }
        float lineWidth = 1.0;
        if(size > 0 && data[0].contains("_size")) {
            lineWidth = data[0]["_size"];
        }

        string shapeType = "line";
        if(size > 0 && data[0].contains("_shape")) {
            shapeType = data[0]["_shape"];
        }
        bool smooth = shapeType == "smooth";

        if(data[0]["_y"].is_array()) {
            // stack 下的多组线
            vector<xg::util::Point> topPoints;
            vector<xg::util::Point> bottomPoints;

            for(std::size_t i = 0; i < size; i++) {
                const nlohmann::json &item = data[i];
                double x = item["_x"];
                const nlohmann::json &yVal = item["_y"];
                if(yVal.size() != 2) {
                    continue;
                }
                bottomPoints.push_back(util::Point{x, yVal[0]});
                topPoints.push_back(util::Point{x, yVal[1]});
            }

            auto topLine =
                xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), topPoints, color, "", smooth);
            if(shapeType == "dash") {
                topLine->SetDashLine(xg::GLOBAL_LINE_DASH);
            }
            container.AddElement(std::move(topLine));

            // auto bottomLine =
            //     xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), bottomPoints, color,
            //     "", smooth);
            // if(shapeType == "dash") {
            //     bottomLine->SetDashLine(xg::GLOBAL_LINE_DASH);
            // }
            // container.AddElement(std::move(bottomLine));
            return;
        }

        vector<xg::util::Point> points;

        // todo 这里有一个判断 如果线是循环的 会将第一个点复制成新点插入队尾 形成循环 目前没有这个判断 后续添加
        for(std::size_t i = 0; i < size; i++) {
            const nlohmann::json &item = data[i];
            points.push_back(util::Point(item["_x"], item["_y"]));
        }

        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), points, color, "", smooth);
        if(shapeType == "dash") {
            l->SetDashLine(xg::GLOBAL_LINE_DASH);
        }
        container.AddElement(std::move(l));
    }
};
} // namespace shape
} // namespace geom
} // namespace xg
#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
