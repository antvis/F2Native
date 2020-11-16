#ifndef XG_GRAPHICS_SHAPE_GROUP_H
#define XG_GRAPHICS_SHAPE_GROUP_H

#include "graphics/shape/Element.h"

using namespace std;

namespace xg {
namespace shape {

class Group : public Element {
  public:
    Group() = default;

    virtual const BBox &GetBBox(canvas::CanvasContext &context) override;

    void AddElement(std::unique_ptr<Element> &&e) { children_.push_back(std::move(e)); }
    void RemoveElement(std::unique_ptr<Element> &e) { std::remove(children_.begin(), children_.end(), e); }

    inline ElementType GetType() const override { return ElementType::Group; }

    // const Element* FindElementById(uint64_t elementId);

    /// 清理children_
    virtual void Destroy() override;

    void Sort() override {
        std::sort(std::begin(children_), std::end(children_),
                  [](const std::unique_ptr<Element> &e1, const std::unique_ptr<Element> &e2) -> bool {
                      return e1->GetZIndex() < e2->GetZIndex();
                  });

        std::for_each(children_.begin(), children_.end(), [&](const std::unique_ptr<Element> &e) -> void { e->Sort(); });
    }

    void Clear() { children_.clear(); }

    std::size_t Size() { return children_.size(); }

  protected:
    virtual void DrawInner(canvas::CanvasContext &context) const override;

  protected:
    vector<std::unique_ptr<Element>> children_;
};
} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_GROUP_H */
