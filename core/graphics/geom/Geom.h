#ifndef GRAPHICS_GEOM_GEOM_H
#define GRAPHICS_GEOM_GEOM_H

#include <map>
#include <set>
#include "attr/AttrBase.h"
#include "../animate/GeomAnimate.h"
#include "../canvas/Container.h"
#include "../canvas/Coord.h"
#include "../shape/Group.h"
#include "../util/json_data.h"
#include "../../utils/Tracer.h"


#define GROUP_ATTRS                                                                                                            \
    vector<xg::attr::AttrType> { xg::attr::AttrType::Color, xg::attr::AttrType::Size, xg::attr::AttrType::Shape }
#define COLORS                                                                                                                 \
    std::vector<string> { "#1890FF", "#2FC25B", "#FACC14", "#223273", "#8543E0", "#13C2C2", "#3436C7", "#F04864" }
#define FIELD_ORIGIN "_origin"
#define FIELD_ORIGIN_Y "_originY"

using namespace xg::scale;
using namespace xg::attr;
using namespace xg::canvas;
using namespace xg::shape;
using namespace xg::canvas::coord;
using namespace std;

namespace xg {

class XChart;

namespace geom {

struct StyleCfg {
    //虚线
    std::vector<float> dash;
    
    //面积图是否从0开始
    bool startOnZero = false;
    
    //线宽
    float lineWidth = 1.0f;

    //描边颜色
    string stroke = "#ffffff";
    //填充颜色
    string fill = "";
    
    //candle的填充色
    std::vector<std::string> candle = {"#1CAA3D", "#808080", "#F4333C"};
    
    //柱子占坐标系的比值
    float widthRatio = 0.5f;
    
    //两点之间与null数据 是否链接
    bool connectNulls = true;
    
    //圆角
    std::vector<float> rounding = {0, 0, 0, 0};
    
    //圆半径
    float size = NAN;
    
    //回调方法 未实现
    std::string custom;
    
#if !defined(__EMSCRIPTEN__)
    BEGIN_TYPE(StyleCfg)
        FIELDS(FIELD(&StyleCfg::dash),
               FIELD(&StyleCfg::startOnZero),
               FIELD(&StyleCfg::lineWidth),
               FIELD(&StyleCfg::stroke),
               FIELD(&StyleCfg::fill),
               FIELD(&StyleCfg::candle),
               FIELD(&StyleCfg::widthRatio),
               FIELD(&StyleCfg::connectNulls),
               FIELD(&StyleCfg::rounding),
               FIELD(&StyleCfg::size),
               FIELD(&StyleCfg::custom))
        CTORS(DEFAULT_CTOR(StyleCfg))
    END_TYPE
#endif
};


struct GeomCfg {
    struct ColorCfg {
        string field;
        vector<string> colors;
        
#if !defined(__EMSCRIPTEN__)
   BEGIN_TYPE(ColorCfg)
       FIELDS(FIELD(&ColorCfg::field),
              FIELD(&ColorCfg::colors))
       CTORS(DEFAULT_CTOR(ColorCfg))
   END_TYPE
#endif
    };
    
    struct SizeCfg {
        string field;
        vector<float> sizes;
        
#if !defined(__EMSCRIPTEN__)
   BEGIN_TYPE(SizeCfg)
       FIELDS(FIELD(&SizeCfg::field),
              FIELD(&SizeCfg::sizes))
       CTORS(DEFAULT_CTOR(SizeCfg))
   END_TYPE
#endif
    };
    
    struct ShapeCfg {
        string field;
        vector<string> shapes;
        
#if !defined(__EMSCRIPTEN__)
   BEGIN_TYPE(ShapeCfg)
       FIELDS(FIELD(&ShapeCfg::field),
              FIELD(&ShapeCfg::shapes))
       CTORS(DEFAULT_CTOR(ShapeCfg))
   END_TYPE
#endif
    };
    
    string type, position;
    string fixedColor;
    ColorCfg color;
    float fixedSize;
    SizeCfg size;
    string fixedShape;
    ShapeCfg shape;
    string adjust;
    StyleCfg style;
    float opacity;
    
#if !defined(__EMSCRIPTEN__)
   BEGIN_TYPE(GeomCfg)
       FIELDS(FIELD(&GeomCfg::type),
              FIELD(&GeomCfg::position),
              FIELD(&GeomCfg::color),
              FIELD(&GeomCfg::fixedColor),
              FIELD(&GeomCfg::size),
              FIELD(&GeomCfg::fixedSize),
              FIELD(&GeomCfg::shape),
              FIELD(&GeomCfg::fixedShape),
              FIELD(&GeomCfg::adjust),
              FIELD(&GeomCfg::style),
              FIELD(&GeomCfg::opacity))
       CTORS(DEFAULT_CTOR(GeomCfg))
   END_TYPE
#endif
};

class AbstractGeom {
    friend animate::GeomAnimate;

  public:
    virtual ~AbstractGeom();
    // emscript编译需要
    AbstractGeom(const AbstractGeom &){};

    AbstractGeom &operator=(const AbstractGeom &) = delete;

    AbstractGeom &Position(const string &field);
    AbstractGeom &Color(const string &field, const vector<string> &colors);
    AbstractGeom &Color(const string &color);
    AbstractGeom &Color(const CanvasLinearGradient &color);
    AbstractGeom &Color(const CanvasRadialGradient &color);
    AbstractGeom &Size(const string &field, const vector<float> &sizes);
    AbstractGeom &Size(const float size);
    AbstractGeom &Shape(const string &field, const vector<string> &shapes);
    AbstractGeom &Shape(const string &shape);
    AbstractGeom &Adjust(const string &adjust);
    AbstractGeom &Opacity(const float opacity);
//    AbstractGeom &Style(const std::string &json);
    AbstractGeom &StyleObject(const StyleCfg &cfg);
    

    const unique_ptr<attr::AttrBase> &GetColor();

    // create scale & attrs & processData
    void Init(XChart *chart);

    void Paint(XChart *chart);

    const std::string &GetXScaleField();
    const std::string &GetYScaleField();

    std::string GetType() const { return this->type_; }

    bool ContainsAttr(attr::AttrType type);

    XDataArray GetSnapRecords(XChart *chart, util::Point point);
    const XData &GetLastSnapRecord(XChart *chart);
    const XData &GetFirstSnapRecord(XChart *chart);

    const AttrBase *GetAttr(AttrType type) {
        if (attrs_.count(type)) {
            return attrs_[type].get();
        }
        return nullptr;
    }

    virtual void Clear();
    
    void ClearInner() {
        dataArray_.clear();
    }

    const XDataGroup &GetDataArray() { return dataArray_; }

#if defined(EMSCRIPTEN)
    // wasm 返回引用有问题，单独开接口返回指针
    AbstractGeom *PositionWasm(const string &field) { return &Position(field); }
    AbstractGeom *ColorWasm(const string &field, const vector<string> &colors) { return &Color(field, colors); }
    AbstractGeom *ColorWasm(const string &color) { return &Color(color); }
    AbstractGeom *SizeWasm(const string &field, const vector<float> &sizes) { return &Size(field, sizes); }
    AbstractGeom *SizeWasm(const float size) { return &Size(size); }
    AbstractGeom *ShapeWasm(const string &field, const vector<string> &shapes) { return &Shape(field, shapes); }
    AbstractGeom *ShapeWasm(const string &shape) { return &Shape(shape); }
    AbstractGeom *AdjustWasm(const string &adjust) { return &Adjust(adjust); }
    AbstractGeom *StyleWasm(const StyleCfg &json) { return &StyleObject(json); }
#endif //EMSCRIPTEN
  protected:
    AbstractGeom(Group *_container, utils::Tracer *tracer) : container_(_container), tracker_(tracer){};
    // 数据分组
    void InitAttributes(XChart &chart);
    void ProcessData(XChart &chart);
    XDataGroup GroupData(XChart &chart);
    const set<string> GetGroupFieldNames(XChart &chart);
    // void GetAttrValues(XChart &chart, const AttrBase &attr, AttrCfg &acfg, DrawCfg &dcfg);

    virtual void Mapping(XChart &chart, XDataArray &dataArray, std::size_t start, std::size_t end);

    virtual void BeforeMapping(XChart &chart, XDataGroup &groupData) {}

    virtual void Draw(XChart &chart, const XDataArray &dataArray, std::size_t start, std::size_t end) const;

    void updateStackRange(XChart &chart);

    virtual double GetYMinValue(XChart &chart);

  protected:
    /*各种属性*/
    bool generatePoints_ = false;
    bool sortable_ = false;
    bool visiable_ = true;
    bool connectNulls_ = false;
    bool ignoreEmptyGroup_ = false;
    string type_ = "";
    string shapeType_ = "";
    
    StyleCfg styleConfig_;
    
    XDataGroup dataArray_;
    map<AttrType, unique_ptr<AttrBase>> attrs_{};

    Group *container_ = nullptr;
    utils::Tracer *tracker_ = nullptr;
};
} // namespace geom
} // namespace xg

#endif // GRAPHICS_GEOM_GEOM_H
