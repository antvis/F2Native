#ifndef XG_GRAPHICS_GEOM_SHAPE_CANDLE_H
#define XG_GRAPHICS_GEOM_SHAPE_CANDLE_H

#include "GeomShapeBase.h"
#include "../../global.h"
#include "../../shape/Line.h"
#include "../../../utils/common.h"

namespace xg {
namespace geom {
namespace shape {

class Candle : public GeomShapeBase {
  public:
    Candle() {}
    void Draw(const std::string &shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const XData &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              const StyleCfg &style) override {

        std::vector<util::Point> points;
        for(std::size_t i = 0; i < data.rect.size(); ++i) {
            util::Point p = this->ParsePoint(coord, data.rect[i]);
            points.push_back(std::move(p));
        }

        double lineYs[2] = {0};
        lineYs[0] = ParsePoint(coord, util::Point(0, data.line[0])).y;
        lineYs[1] = ParsePoint(coord, util::Point(0, data.line[1])).y;

        std::string colorStyle = style.candle[data.state + 1];

        float lineWidth = std::isnan(data._size) ? 1.0 : data._size;
        lineWidth *= context.GetDevicePixelRatio();

        util::Size size(fabs(points[2].x - points[0].x), fabs(points[2].y - points[0].y));

        if(!style.stroke.empty()) {
            auto line = xg::make_unique<xg::shape::Line>(util::Point{points[0].x + size.width / 2, lineYs[0]},
                                                         util::Point{points[0].x + size.width / 2, lineYs[1]});
            line->SetStorkColor(colorStyle);
            line->SetLineWidth(lineWidth);

            container.AddElement(std::move(line));

            auto rect = xg::make_unique<xg::shape::Rect>(points[0], size);
            rect->SetFillColor(colorStyle);
            container.AddElement(std::move(rect));
        } else {
            auto line1 = xg::make_unique<xg::shape::Line>(util::Point{points[0].x + size.width / 2, lineYs[0]},
                                                          util::Point{points[0].x + size.width / 2, points[0].y});
            line1->SetStorkColor(colorStyle);
            line1->SetLineWidth(lineWidth);
            container.AddElement(std::move(line1));

            auto line2 = xg::make_unique<xg::shape::Line>(util::Point{points[0].x + size.width / 2, lineYs[1]},
                                                          util::Point{points[0].x + size.width / 2, points[0].y + size.height});
            line2->SetStorkColor(colorStyle);
            line2->SetLineWidth(lineWidth);
            container.AddElement(std::move(line2));

            auto rect = xg::make_unique<xg::shape::Rect>(points[0], size);
            rect->SetStorkColor(colorStyle);
            rect->SetLineWidth(lineWidth);
            container.AddElement(std::move(rect));
        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_CANDLE_H
