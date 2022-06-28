#ifndef XG_GRAPHICS_GEOM_SHAPE_LINE_H
#define XG_GRAPHICS_GEOM_SHAPE_LINE_H

#include <vector>
#include "GeomShapeBase.h"
#include "../../global.h"
#include "../../shape/Polyline.h"
#include "../../util/Point.h"

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
              const XDataArray &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              bool connectNulls) override {
        this->drawLines(coord, data, start, end, context, container, connectNulls);
    }

  private:
    // 还缺一个style
    void drawLines(canvas::coord::AbstractCoord &coord,
                   const XDataArray &data,
                   std::size_t start,
                   std::size_t end,
                   canvas::CanvasContext &canvasContext,
                   xg::shape::Group &container,
                   bool connectNulls) {
        size_t size = end - start + 1;
        if(size <= 0)
            return;

        std::string color = data[start]._color.empty() ? GLOBAL_COLORS[0] : data[start]._color;

        float lineWidth = std::isnan(data[start]._size) ? 1 : data[start]._size;

        string shapeType = data[start]._shape; // [line, smooth]
        bool smooth = shapeType == "smooth";

        if(!data[0]._y0.empty()) {
            // stack 下的多组线
            vector<xg::util::Point> topPoints;
            vector<xg::util::Point> bottomPoints;

            for(std::size_t i = start; i <= end; i++) {
                const auto &item = data[i];
                double x = item._x;
                auto &yVal = item._y0;
                if(yVal.size() != 2) {
                    continue;
                }
                bottomPoints.push_back(util::Point{x, yVal[0]});
                topPoints.push_back(util::Point{x, yVal[1]});
            }

            auto topLine = xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), topPoints, smooth);
            if(data[start]._style.contains("dash")) {
                topLine->SetDashLine(json::ParseDashArray(data[start]._style["dash"], canvasContext.GetDevicePixelRatio()));
            }

            topLine->SetStorkColor(color);
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
            const auto &item = data[i];
            if(connectNulls) {
                if(!std::isnan(item._x) && !std::isnan(item._y)) {
                    points.push_back(util::Point(item._x, item._y));
                }
            } else {
                points.push_back(util::Point(item._x, item._y));
            }
        }
        
        //雷达图 闭合折线
        if (coord.GetType() == CoordType::Polar) {
            points.push_back(points[0]);
        }

        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), points, smooth);
        if(data[start]._style.contains("dash")) {
            l->SetDashLine(json::ParseDashArray(data[start]._style["dash"], canvasContext.GetDevicePixelRatio()));
        }
        l->SetStorkColor(color);
        container.AddElement(std::move(l));
    }
};
} // namespace shape
} // namespace geom
} // namespace xg
#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
