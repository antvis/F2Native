#ifndef XG_GRAPHICS_GEOM_SHAPE_LINE_H
#define XG_GRAPHICS_GEOM_SHAPE_LINE_H

#include <vector>
#include "GeomShapeBase.h"
#include "../../global.h"
#include "../../shape/Polyline.h"
#include "../../util/Point.h"
#include "../../shape/Circle.h"

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
    void DrawCircle(canvas::coord::AbstractCoord &coord,
                    const nlohmann::json &data,
                    std::size_t start,
                    std::size_t end,
                    canvas::CanvasContext &canvasContext,
                    xg::shape::Group &container,
                    bool connectNulls) {
      
        util::Point center(NAN, NAN);
        if(data[start]["_y"].is_array()) {
            const nlohmann::json &item = data[start];
            double x = item["_x"];
            const nlohmann::json &yVal = item["_y"];
            if(yVal.size() != 2) {
                return;;
            }
            center = util::Point{x, yVal[1]};
        } else {
            const nlohmann::json &item = data[start];
            center = util::Point{ item["_x"], item["_y"]};
        }
        
        if (std::isnan(center.x) || std::isnan(center.y)) {
            return;
        }
        
        canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(data[start], "_color");
        //使用线宽的一般作为半径来画点，避免出现第二个点的时候 有明显的大小变化的跳动感觉
        float radius = json::GetNumber(data[start], "_size", 1.0) * canvasContext.GetDevicePixelRatio() / 2.0;
        auto circle = std::make_unique<xg::shape::Circle>(center, radius);
        circle->SetFillStyle(colorStyle);
        container.AddElement(std::move(circle));
    }
    
    // 还缺一个style
    void drawLines(canvas::coord::AbstractCoord &coord,
                   const nlohmann::json &data,
                   std::size_t start,
                   std::size_t end,
                   canvas::CanvasContext &canvasContext,
                   xg::shape::Group &container,
                   bool connectNulls) {
        size_t size = end - start + 1;
        if (size <= 0) {
            return;
        } else if (size == 1) {
            DrawCircle(coord, data, start, end, canvasContext, container, connectNulls);
            return;
        }

        float lineWidth = 1.0;
        if(data[start].contains("_size")) {
            lineWidth = data[start]["_size"];
        }
        
        std::string lineCap = "butt";
        std::string lineJoin = "miter";
        if(data[start].contains("_style")) {
            if(data[start]["_style"].contains("lineCap")) {
                lineCap = json::GetString(data[start]["_style"], "lineCap", "butt");
            }
            if(data[start]["_style"].contains("lineJoin")) {
                lineJoin = json::GetString(data[start]["_style"], "lineJoin", "miter");
            }
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

            //判断是否有position
            canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(data[start], "_color");

            topLine->SetStorkStyle(colorStyle);
            topLine->SetLineCap(lineCap);
            topLine->SetLineJoin(lineJoin);
            container.AddElement(std::move(topLine));

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

        BBox bbox = BBoxUtil::GetBBoxFromPoints(points, lineWidth);
        //判断是否有position
        canvas::CanvasFillStrokeStyle colorStyle = util::ColorParser(data[start], "_color", &bbox);

        //雷达图 闭合折线
        if (coord.GetType() == CoordType::Polar) {
            points.push_back(points[0]);
        }

        auto l = xg::make_unique<xg::shape::Polyline>(lineWidth * canvasContext.GetDevicePixelRatio(), points, smooth);
        if(data[start].contains("_style") && data[start]["_style"].contains("dash")) {
            l->SetDashLine(json::ParseDashArray(data[start]["_style"]["dash"], canvasContext.GetDevicePixelRatio()));
        }
        l->SetStorkStyle(colorStyle);
        l->SetLineCap(lineCap);
        l->SetLineJoin(lineJoin);
        container.AddElement(std::move(l));
    }
};
} // namespace shape
} // namespace geom
} // namespace xg
#endif // XG_GRAPHICS_GEOM_SHAPE_LINE_H
