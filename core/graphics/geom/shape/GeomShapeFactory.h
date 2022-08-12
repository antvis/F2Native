#ifndef XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEFACTORY_H
#define XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEFACTORY_H

#include <string>
#include <unordered_map>
#include "GeomShapeBase.h"
#include "../../util/json_data.h"
#include "../../shape/Group.h"
#include "../../../nlohmann/json.hpp"

namespace xg {
class XChart;

namespace geom {
namespace shape {

class GeomShapeFactory final {
  public:
  public:
    GeomShapeFactory() {}
    ~GeomShapeFactory() { geomShapes_.clear(); }

    void Clear() { geomShapes_.clear(); }

    // geomType: line/interval/point 等
    void RegisterGeomShape(std::string geomType, std::unique_ptr<GeomShapeBase> geomShape);

    void UnregterShapeShape(std::string gemoType);

    // shapeType 为 geomType 的子类型, 例如 line 分为 line/dash/smooth
    void DrawGeomShape(XChart &chart,
                       std::string geomType,
                       std::string shapeType,
                       const XDataArray &data,
                       std::size_t start,
                       std::size_t end,
                       xg::shape::Group &container,
                       const StyleCfg &style);
    
    void DrawGeomShape(XChart &chart,
                       std::string geomType,
                       std::string shapeType,
                       const XData &data,
                       std::size_t start,
                       std::size_t end,
                       xg::shape::Group &container,
                       const StyleCfg &style);

  private:
    std::unordered_map<std::string, std::unique_ptr<GeomShapeBase>> geomShapes_;
};
} // namespace shape
} // namespace geom
} // namespace xg

#endif // XG_GRAPHICS_GEOM_SHAPE_GEOMSHAPEFACTORY_H
