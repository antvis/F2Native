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
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              bool connectNulls) override {
        this->drawLines(coord, data, start, end, context, container, connectNulls);
    }

  private:
    // 还缺一个style
    void drawLines(canvas::coord::AbstractCoord &coord,
                   const nlohmann::json &data,
                   std::size_t start,
                   std::size_t end,
                   canvas::CanvasContext &canvasContext,
                   xg::shape::Group &container,
                   bool connectNulls) {
        size_t size = end - start + 1;
        if(size <= 0)
            return;

        util::CanvasFillStrokeStyle colorStyle = util::ColorParser(data[start], "_color");

        float lineWidth = 1.0;
        if(data[start].contains("_size")) {
            lineWidth = data[start]["_size"];
        }

        string shapeType = "line"; // [line, smooth]
        if(data[start].contains("_shape")) {
            shapeType = data[start]["_shape"];
        }
        bool smooth = shapeType == "smooth";

        if(data[start]["_y"].is_array()) {
            // stack 下的多组线
            vector<xg::util::Point> topPoints;
            vector<xg::util::Point> bottomPoints;

            for(std::size_t i = start; i <= end; i++) {
                const nlohmann::json &item = data[i];
                double x = item["_x"];
                const nlohmann::json &yVal = item["_y"];
                if(yVal.size() != 2) {
                    continue;
                }
                bottomPoints.push_back(util::Point{x, yVal[0]});
                topPoints.push_back(util::Point{x, yVal[1]});
            }

            auto topLine = xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), topPoints, smooth);
            if(data[start].contains("_style") && data[start]["_style"].contains("dash")) {
                topLine->SetDashLine(json::ParseDashArray(data[start]["_style"]["dash"], canvasContext.GetDevicePixelRatio()));
            }

            topLine->strokeStyle_ = colorStyle;
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
        for(std::size_t i = start; i <= end; i++) {
            const nlohmann::json &item = data[i];
            if(connectNulls) {
                if(item["_x"].is_number() && !std::isnan(item["_x"]) && item["_y"].is_number() && !std::isnan(item["_y"])) {
                    points.push_back(util::Point(item["_x"], item["_y"]));
                }
            } else {
                points.push_back(util::Point(item["_x"], item["_y"]));
            }
        }

        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), points, smooth);
        if(data[start].contains("_style") && data[start]["_style"].contains("dash")) {
            l->SetDashLine(json::ParseDashArray(data[start]["_style"]["dash"], canvasContext.GetDevicePixelRatio()));
        }
        l->strokeStyle_ = colorStyle;
        container.AddElement(std::move(l));
    }
};
} // namespace shape
} // namespace geom
} // namespace xg
#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
