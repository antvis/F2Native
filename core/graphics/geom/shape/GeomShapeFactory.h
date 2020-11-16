#ifndef XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEFACTORY_H
#define XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEFACTORY_H

#include "graphics/geom/shape/GeomShapeBase.h"
#include "graphics/shape/Group.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace xg {
class XChart;

namespace geom {
namespace shape {

class GeomShapeFactory final {
  public:
  public:
    GeomShapeFactory() {}
    ~GeomShapeFactory() { geomShapes_.clear(); }

    // geomType: line/interval/point 等
    void RegisterGeomShape(std::string geomType, std::unique_ptr<GeomShapeBase> geomShape);

    void UnregterShapeShape(std::string gemoType);

    // shapeType 为 geomType 的子类型, 例如 line 分为 line/dash/smooth
    void DrawGeomShape(XChart &chart, std::string geomType, std::string shapeType, const nlohmann::json &data, xg::shape::Group &container);

  private:
    std::unordered_map<std::string, std::unique_ptr<GeomShapeBase>> geomShapes_;
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEFACTORY_H
