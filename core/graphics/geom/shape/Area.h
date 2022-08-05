#ifndef GRAPHICS_GEOM_SHAPE_AREA_H
#define GRAPHICS_GEOM_SHAPE_AREA_H

#include <vector>
#include "GeomShapeBase.h"
#include "../../global.h"
#include "../../shape/Area.h"

using namespace xg::util;

namespace xg {
namespace geom {
namespace shape {

class Area : public GeomShapeBase {
  public:
    Area() {}

    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const XDataArray &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              const XStyle &style) override {
        vector<xg::util::Point> topPoints;
        vector<xg::util::Point> bottomPoints;
        topPoints.reserve(end - start);
        bottomPoints.reserve(end - start);

        for(std::size_t i = start; i <= end; i++) {
            const auto &item = data[i];
            const auto &points = item.points;
            if(style.connectNulls) {
                if(!(std::isnan(points[0].x) || std::isnan(points[0].y) || std::isnan(points[1].x) || std::isnan(points[1].y))) {
                    util::Point bottom{points[0].x, points[0].y};
                    util::Point top{points[1].x, points[1].y};
                    top = coord.ConvertPoint(top);
                    bottom = coord.ConvertPoint(bottom);

                    topPoints.emplace_back(top);
                    bottomPoints.emplace_back(bottom);
                }
            } else {
                util::Point bottom{points[0].x, points[0].y};
                util::Point top{points[1].x, points[1].y};
                top = coord.ConvertPoint(top);
                bottom = coord.ConvertPoint(bottom);

                topPoints.emplace_back(top);
                bottomPoints.emplace_back(bottom);
            }
        }
        std::reverse(bottomPoints.begin(), bottomPoints.end());
        //雷达图 闭合区域
       if (coord.GetType() == CoordType::Polar) {
           topPoints.push_back(topPoints[0]);
           bottomPoints.push_back(bottomPoints[0]);
       }

        std::size_t size = end - start + 1;
        if(size > 0 && !data[start]._shape.empty()) {
            shapeType = data[start]._shape;
        }
        bool smooth = shapeType == "smooth";
        auto area = xg::make_unique<xg::shape::Area>(topPoints, bottomPoints, smooth);

        if(size <= 0) {
            area->SetFillColor(GLOBAL_COLORS[0]);
        } else {
            area->SetFillColor(data[start]._color);
        }
        container.AddElement(std::move(area));
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif
