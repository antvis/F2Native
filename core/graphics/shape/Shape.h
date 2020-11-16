#ifndef XG_GRAPHICS_SHAPE_SHAPE_H
#define XG_GRAPHICS_SHAPE_SHAPE_H

#include "graphics/shape/Element.h"

namespace xg {
namespace shape {

class Shape : public Element {
    public:
    Shape();

    virtual ~Shape() override {}

    /// 获取包围盒，同一个shape只会一算一次
    /// @param context
    const BBox &GetBBox(canvas::CanvasContext &context) override;

    /// 包围盒计算，每次都会计算
    virtual BBox CalculateBox(canvas::CanvasContext &context) const override { return { 0, 0, 0, 0, 0, 0 }; }

    protected:
    virtual void DrawInner(canvas::CanvasContext &context) const override;

    /// 调用canvas生成绘制指令
    /// @param context canvas的context
    virtual void CreatePath(canvas::CanvasContext &context) const {};
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_SHAPE_H */
