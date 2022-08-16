#include "GeomShapeFactory.h"
#include "../../XChart.h"

using namespace xg;

void geom::shape::GeomShapeFactory::RegisterGeomShape(const std::string &geomType, std::unique_ptr<geom::shape::GeomShapeBase> geomShape) {
    geomShapes_[geomType] = std::move(geomShape);
}

void geom::shape::GeomShapeFactory::UnregterShapeShape(const std::string &geomType) { geomShapes_.erase(geomType); }

void geom::shape::GeomShapeFactory::DrawGeomShape(XChart &chart,
                                                  const std::string &geomType,
                                                  const std::string &shapeType,
                                                  const XDataArray &data,
                                                  std::size_t start,
                                                  std::size_t end,
                                                  xg::shape::Group &container,
                                                  const StyleCfg &style) {
    auto got = geomShapes_.find(geomType);
    if(got == geomShapes_.end()) {
        // todo log
        return;
    }

    got->second->Draw(shapeType, chart.GetCoord(), chart.GetCanvasContext(), data, start, end, container, style);
}

void geom::shape::GeomShapeFactory::DrawGeomShape(XChart &chart,
                   const std::string &geomType,
                   const std::string &shapeType,
                   const XData &data,
                   std::size_t start,
                   std::size_t end,
                   xg::shape::Group &container,
                   const StyleCfg &style) {
    auto got = geomShapes_.find(geomType);
    if(got == geomShapes_.end()) {
        // todo log
        return;
    }

    got->second->Draw(shapeType, chart.GetCoord(), chart.GetCanvasContext(), data, start, end, container, style);
}
