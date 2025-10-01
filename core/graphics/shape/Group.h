#ifndef XG_GRAPHICS_SHAPE_GROUP_H
#define XG_GRAPHICS_SHAPE_GROUP_H

#include "Shape.h"
#include <unordered_map>

using namespace std;

namespace xg {
namespace shape {

class Group : public Element {
  public:
    Group() = default;

    virtual const BBox &GetBBox(canvas::CanvasContext &context) override;

    void AddElement(std::unique_ptr<Element> &&e) { children_.push_back(std::move(e)); }
    void RemoveElement(std::unique_ptr<Element> &e) { std::remove(children_.begin(), children_.end(), e); }
    
    virtual void UpdateAttribute(std::string attrName, double val) override;

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

    void Translate(float x, float y) override;
    void Rotate(float rad) override;
    void Scale(float sx, float sy) override;
    void MoveTo(float x, float y) override;
    void Apply(Vector2D *v) override;
    void Alpha(float alpha) override;

    Group *AddGroup();

    void DoClip(canvas::CanvasContext &) const override;
    void DoClipByIdentifier(canvas::CanvasContext &, string identifier) const override;
    bool NeedShapeClip(canvas::CanvasContext &, const std::function<void(string)>& callback) const override;

  protected:
    virtual void DrawInner(canvas::CanvasContext &context) const override;

  public:
    vector<std::unique_ptr<Element>> children_;

    std::unique_ptr<Shape> clip_ = nullptr;
    
    std::unordered_map<std::string, std::unique_ptr<Shape>> shapeAnimationDictionary;
    vector<std::string> endAnimationElement;
};
} // namespace shape
} // namespace xg

#endif /* XG_GRAPHICS_SHAPE_GROUP_H */
