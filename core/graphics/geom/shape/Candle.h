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
    void Draw(std::string shapeType,
              canvas::coord::AbstractCoord &coord,
              canvas::CanvasContext &context,
              const XDataArray &data,
              std::size_t start,
              std::size_t end,
              xg::shape::Group &container,
              bool connectNulls) override {
//        if(!data.contains("_rect") || !data.contains("_line")) {
//            return;
//        }
//
//        const nlohmann::json &_rect = data["_rect"];
//        const nlohmann::json &_line = data["_line"];
//        const int state = data["_state"];
//        const nlohmann::json &style = data["_style"];
//
//        std::vector<util::Point> points;
//        for(std::size_t i = 0; i < _rect.size(); ++i) {
//            const nlohmann::json &item = _rect[i];
//            util::Point p = this->ParsePoint(coord, util::Point(item["x"], item["y"]));
//            points.push_back(std::move(p));
//        }
//
//        double lineYs[2] = {0};
//        lineYs[0] = ParsePoint(coord, util::Point(0, _line[0])).y;
//        lineYs[1] = ParsePoint(coord, util::Point(0, _line[1])).y;
//
//        nlohmann::json colors;
//        bool fill = true;
//        if(style.contains("stroke")) {
//            colors = style["stroke"];
//            fill = false;
//        } else if(style.contains("fill")) {
//            colors = style["fill"];
//        }
//
//        if(!colors.is_array() || colors.size() < 3) {
//            colors = {"#1CAA3D", "#808080", "#F4333C"};
//        }
//
//        canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(colors[state + 1]);
//
//        float lineWidth = 1.0;
//        if(data.contains("_size")) {
//            lineWidth = data["_size"];
//        }
//        lineWidth *= context.GetDevicePixelRatio();
//
//        util::Size size(fabs(points[2].x - points[0].x), fabs(points[2].y - points[0].y));
//
//        if(fill) {
//            auto line = xg::make_unique<xg::shape::Line>(util::Point{points[0].x + size.width / 2, lineYs[0]},
//                                                         util::Point{points[0].x + size.width / 2, lineYs[1]});
//            line->SetStorkStyle(colorStyle);
//            line->SetLineWidth(lineWidth);
//
//            container.AddElement(std::move(line));
//
//            auto rect = xg::make_unique<xg::shape::Rect>(points[0], size);
//            rect->SetFillStyle(colorStyle);
//            container.AddElement(std::move(rect));
//        } else {
//            auto line1 = xg::make_unique<xg::shape::Line>(util::Point{points[0].x + size.width / 2, lineYs[0]},
//                                                          util::Point{points[0].x + size.width / 2, points[0].y});
//            line1->SetStorkStyle(colorStyle);
//            line1->SetLineWidth(lineWidth);
//            container.AddElement(std::move(line1));
//
//            auto line2 = xg::make_unique<xg::shape::Line>(util::Point{points[0].x + size.width / 2, lineYs[1]},
//                                                          util::Point{points[0].x + size.width / 2, points[0].y + size.height});
//            line2->SetStorkStyle(colorStyle);
//            line2->SetLineWidth(lineWidth);
//            container.AddElement(std::move(line2));
//
//            auto rect = xg::make_unique<xg::shape::Rect>(points[0], size);
//            rect->SetStorkStyle(colorStyle);
//            rect->SetLineWidth(lineWidth);
//            container.AddElement(std::move(rect));
//        }
    }
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_CANDLE_H
