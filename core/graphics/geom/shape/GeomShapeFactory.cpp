#include "graphics/geom/shape/GeomShapeFactory.h"
#include "graphics/XChart.h"

using namespace xg;

void geom::shape::GeomShapeFactory::RegisterGeomShape(std::string geomType, std::unique_ptr<geom::shape::GeomShapeBase> geomShape) {
    geomShapes_[geomType] = std::move(geomShape);
}

void geom::shape::GeomShapeFactory::UnregterShapeShape(std::string geomType) { geomShapes_.erase(geomType); }

void geom::shape::GeomShapeFactory::DrawGeomShape(XChart &chart,
                                                  std::string geomType,
                                                  std::string shapeType,
                                                  const nlohmann::json &data,
                                                  std::size_t start,
                                                  std::size_t end,
                                                  xg::shape::Group &container) {
    auto got = geomShapes_.find(geomType);
    if(got == geomShapes_.end()) {
        // todo log
        return;
    }

    got->second->Draw(shapeType, chart.GetCoord(), chart.GetCanvasContext(), data, start, end, container);
}
