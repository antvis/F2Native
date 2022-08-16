#ifndef XG_GRAPHICS_ELEMENT_H
#define XG_GRAPHICS_ELEMENT_H

#include "../canvas/CanvasContext.h"
#include "../util/BBox.h"
#include "../util/Matrix.h"
#include "../util/Point.h"

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
    Element();

  public:
    static bool IsUnchanged(const Matrix &matrix);

    /// 绘制
    /// @param context canvas的context
    void Draw(canvas::CanvasContext &context) const;

    /// 矩阵操作
    inline Matrix GetMatrix() { return matrix_; }
    inline void SetMatrix(Matrix &matrix) {
        matrix_[0] = matrix[0];
        matrix_[1] = matrix[1];
        matrix_[2] = matrix[2];
        matrix_[3] = matrix[3];
        matrix_[4] = matrix[4];
        matrix_[5] = matrix[5];
    }
    virtual void Translate(float x, float y);
    virtual void Rotate(float rad);
    virtual void Scale(float sx, float sy);
    virtual void MoveTo(float x, float y);
    virtual void Apply(Vector2D *v);
    virtual void Transform(const std::vector<TransformAction> &actions);

    /// 子类处理内部绘制逻辑
    /// @param context canvas的context
    virtual void DrawInner(canvas::CanvasContext &context) const {};

    /// get属性
    inline bool HasStroke() const { return strokeStyle_.type != canvas::CanvasFillStrokeStyleType::kNone; }
    inline bool HasFill() const { return fillStyle_.type != canvas::CanvasFillStrokeStyleType::kNone; }
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

    virtual void DoClip(canvas::CanvasContext &) const {}
    
    virtual void SetFillStrokeStyle(const std::string &strokeColor, const std::string &fillColor) {
        SetStorkColor(strokeColor);
        SetFillColor(fillColor);
    }
    
    
    /// 设置fillstyle 并解析透明度
    /// @param fillColor 颜色
    virtual void SetFillColor(const std::string &fillColor) {
        if(!fillColor.empty()) {
            SetFillStyle(canvas::CanvasFillStrokeStyle(fillColor));
        }
    }
    
    
    /// 设置strokeStyle 并解析透明度
    /// @param strokeColor 颜色
    virtual void SetStorkColor(const std::string &strokeColor) {
        if(!strokeColor.empty()) {
            SetStorkStyle(canvas::CanvasFillStrokeStyle(strokeColor));
        }
    }
    
    virtual inline void SetFillStyle(const canvas::CanvasFillStrokeStyle &fillStyle) {
        fillStyle_ = fillStyle;
    }
    
    virtual inline void SetStorkStyle(const canvas::CanvasFillStrokeStyle &strokeStyle) {
        strokeStyle_ = strokeStyle;
    }
    
    virtual inline void SetStrokeOpacity(const float opacity) {
        if (!std::isnan(opacity)) {
            strokeOpacity_ = opacity;
        }
    }
    
    virtual inline void SetFillOpacity(const float opacity) {
        if (!std::isnan(opacity)) {
            fillOpacity_ = opacity;
        }
    }
    
    virtual inline void SetLineWidth(const float lineWidth) {
        lineWidth_ = lineWidth;
    }

  protected:
    util::Point point_;
    canvas::CanvasFillStrokeStyle fillStyle_;
    canvas::CanvasFillStrokeStyle strokeStyle_;
    float strokeOpacity_ = NAN;
    float fillOpacity_ = NAN;
    float lineWidth_ = NAN;

  private:
    void InitElementId();
    void InitTransform();

  protected:
    void ResetTransform(canvas::CanvasContext &context) const;
    void SetContext(canvas::CanvasContext &context) const;
    void RestoreContext(canvas::CanvasContext &context) const;
    void ResetContext(canvas::CanvasContext &context) const;

    void RemoveFromParent() {}

  protected:
    int zIndex_ = 0;
    bool visible_ = true;
    bool destroyed_ = false;
    bool isShape_ = false;
    bool isGroup_ = false;
    Matrix matrix_;
    BBox bbox_{static_cast<float>((std::nan("0")))};
    string type_ = "element";

    std::string font_ = "";
    array<float, 2> lineDash_ = {{0, 0}};
    string textAlign_ = "start";
    string textBaseline_ = "bottom";
};
} // namespace shape
} // namespace xg

#endif // XG_GRAPHICS_ELEMENT_H
