#ifndef XG_GRAPHICS_SHAPE_SHAPE_H
#define XG_GRAPHICS_SHAPE_SHAPE_H

#include "Element.h"

namespace xg {
namespace shape {
class Group;
class Shape : public Element {
    friend Group;

  public:
    Shape();

    virtual ~Shape() override {}

    /// 获取包围盒，同一个shape只会一算一次
    /// @param context 上下文
    const BBox &GetBBox(canvas::CanvasContext &context) override;

    /// 包围盒计算，每次都会计算
    virtual BBox CalculateBox(canvas::CanvasContext &context) const override { return {0, 0, 0, 0, 0, 0}; }

    // 各元素独立实现，用于更新属性值
    virtual void UpdateAttribute(std::string attrName, double val) {}
    virtual void UpdateAttribute(std::string attrName, const std::string &val) {}

  protected:
    virtual void DrawInner(canvas::CanvasContext &context) const override;

    /// 调用canvas生成绘制指令
    /// @param context canvas的context
    virtual void CreatePath(canvas::CanvasContext &context) const {};
};

} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_SHAPE_H */
