#ifndef XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEBASE_H
#define XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEBASE_H

#include <math.h>
#include <vector>
#include "../../canvas/CanvasContext.h"
#include "../../canvas/Coord.h"
#include "../../shape/Group.h"
#include "../../util/json_data.h"
#include "../../../nlohmann/json.hpp"
#include "../../../utils/common.h"

using namespace std;

namespace xg {
namespace geom {
namespace shape {

class GeomShapeBase {
  public:
    GeomShapeBase() {}
    virtual ~GeomShapeBase() {}

    //绘制一组数据 比如线
    virtual void Draw(std::string shapeType,
                      canvas::coord::AbstractCoord &coord,
                      canvas::CanvasContext &context,
                      const XDataArray &data,
                      std::size_t start,
                      std::size_t end,
                      xg::shape::Group &container,
                      bool connectNulls) {};
    
    //绘制一个数据 比如柱
    virtual void Draw(std::string shapeType,
                      canvas::coord::AbstractCoord &coord,
                      canvas::CanvasContext &context,
                      const XData &data,
                      std::size_t start,
                      std::size_t end,
                      xg::shape::Group &container,
                      bool connectNulls) {};

    // // line暂未用到
    util::Point ParsePoint(canvas::coord::AbstractCoord &coord, const util::Point &point) {
        if(coord.GetType() == canvas::coord::CoordType::Polar) {
            util::Point p = util::Point(point);
            if(xg::IsZero(p.x - 1.)) {
                p.x = 0.9999999;
            }
            if(xg::IsZero(point.y - 1.)) {
                p.y = 0.9999999;
            }
            return coord.ConvertPoint(p);
        }
        return coord.ConvertPoint(point);
    }

    vector<util::Point> ParsePoints(canvas::coord::AbstractCoord &coord, const vector<util::Point> &points) {
        if(points.size() == 0) {
            return points;
        }
        vector<util::Point> rst;
        size_t size = points.size();
        for(std::size_t i = 0; i < size; i++) {
            const util::Point &point = points.at(i);
            rst.push_back(ParsePoint(coord, point));
        }
        return rst;
    }
};
} // namespace shape
} // namespace geom
} // namespace xg
#endif // XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEBASE_H
