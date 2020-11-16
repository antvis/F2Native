#include "graphics/canvas/CanvasContext.h"
#include "graphics/canvas/Coord.h"
#include "graphics/shape/Group.h"
#include <math.h>
#include <nlohmann/json.hpp>
#include <utils/common.h>
#include <vector>

#ifndef XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEBASE_H
#define XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEBASE_H

using namespace std;

namespace xg {
namespace geom {
namespace shape {

class GeomShapeBase {
  public:
    GeomShapeBase() {}
    virtual ~GeomShapeBase() {}

    virtual void Draw(std::string shapeType,
                      canvas::coord::AbstractCoord &coord,
                      canvas::CanvasContext &context,
                      const nlohmann::json &data,
                      xg::shape::Group &container) = 0;

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
        for(int i = 0; i < size; i++) {
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
