#ifndef XG_GRAPHICS_ELEMENT_H
#define XG_GRAPHICS_ELEMENT_H

#include "graphics/canvas/CanvasContext.h"
#include "graphics/util/BBox.h"
#include "graphics/util/Matrix.h"
#include "graphics/util/Point.h"

using namespace std;
using namespace xg::util;

namespace xg {
namespace shape {

enum class ElementType { Single, Group };

class Element {
  public:
    virtual ~Element(){};
    Element(const Element &) = delete;
    Element &operator=(const Element &) = delete;

  protected:
    /// cfg包含绘制各个shape需要的所有属性
    /// @param cfg json
    Element();

  public:
    static bool IsUnchanged(const Matrix &matrix);

    /// 绘制
    /// @param context canvas的context
    void Draw(canvas::CanvasContext &context) const;

    /// 矩阵操作
    inline Matrix &GetMatrix() { return matrix_; }
    inline void SetMatrix(Matrix &matrix) { matrix_ = matrix; }
    virtual void Translate(float x, float y);
    virtual void Rotate(float rad);
    virtual void Scale(float sx, float sy);
    virtual void MoveTo(float x, float y);
    virtual void Apply(Vector2D *v);

    /// 子类处理内部绘制逻辑
    /// @param context canvas的context
    virtual void DrawInner(canvas::CanvasContext &context) const {};

    /// get属性
    inline bool HasStroke() const { return canStroke_ && !stroke_.empty(); }
    inline bool HasFill() const { return canFill_ && !fill_.empty(); }
    inline bool IsVisible() const { return visible_; }
    inline bool IsDestroyed() const { return destroyed_; }
    inline bool IsGroup() const { return isGroup_; }
    inline bool IsShape() const { return isShape_; }
    inline int GetZIndex() const { return zIndex_; }
    virtual inline ElementType GetType() const { return ElementType::Single; }

    inline const char *getType() { return type_.c_str(); }

    virtual const BBox &GetBBox(canvas::CanvasContext &context) { return bbox_; };
    virtual BBox CalculateBox(canvas::CanvasContext &context) const { return {static_cast<float>(std::nan(0))}; }

    /// set属性
    inline void SetZIndex(int z) { zIndex_ = z; }

    /// 对element的操作
    inline Element &Show() {
        visible_ = true;
        return *this;
    }
    inline Element &Hide() {
        visible_ = false;
        return *this;
    }
    virtual void Destroy();
    virtual void Remove();

    virtual void Sort() {}

  public:
    util::Point point_;

  private:
    void InitElementId();
    void InitTransform();
    void ResetTransform(canvas::CanvasContext &context) const;
    void SetContext(canvas::CanvasContext &context) const;
    void RestoreContext(canvas::CanvasContext &context) const;
    void ResetContext(canvas::CanvasContext &context) const;

    void RemoveFromParent() {}

  protected:
    int zIndex_ = 0;
    bool visible_ = true;
    bool destroyed_ = false;
    bool canFill_ = false;
    bool canStroke_ = false;
    bool isShape_ = false;
    bool isGroup_ = false;
    Matrix matrix_;
    BBox bbox_{static_cast<float>((std::nan("0")))};
    string type_ = "element";
    string fill_ = "";
    string stroke_ = "";
    float strokeOpacity_ = std::nan("0");
    float fillOpacity_ = std::nan("0");
    float lineWidth_ = std::nan("0");
    std::string font_ = "";
    array<float, 2> lineDash_ = {{0, 0}};
    string textAlign_ = "start";
    string textBaseline_ = "bottom";
};
} // namespace shape
} // namespace xg

#endif // XG_GRAPHICS_ELEMENT_H
