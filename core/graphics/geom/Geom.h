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

struct XStyle {
    //虚线
    std::vector<float> dash;
    
    //面积图是否从0开始
    bool startOnZero = true;
    
    //线宽
    float lineWidth = 1.0f;

    //描边颜色
    string stroke = "#ffffff";
    //填充颜色
    string fill = "";
    
    //candle的填充色
    std::array<std::string, 3> candle = {"#1CAA3D", "#808080", "#F4333C"};
    
    //柱子占坐标系的比值
    float widthRatio = 0.5f;
    
    //两点之间与null数据 是否链接
    bool connectNulls = true;
    
    //圆角
    std::array<float, 4> roundings = {0, 0, 0, 0};
    
    //圆半径
    float size = 3.0f;
};

extern void from_json(const nlohmann::json& j, XStyle& x);

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
    AbstractGeom &Size(const string &field, const vector<float> &sizes);
    AbstractGeom &Size(const float size);
    AbstractGeom &Shape(const string &field, const vector<string> &shapes);
    AbstractGeom &Shape(const string &shape);
    AbstractGeom &Adjust(const string &adjust);
    AbstractGeom &Style(const std::string &json);
    AbstractGeom &StyleObject(const XStyle &cfg);
    

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

    const std::unique_ptr<AttrBase> &GetAttr(AttrType type) { return attrs_[type]; }

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
    AbstractGeom *StyleWasm(const std::string &json) { return &Style(json); }
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
//    nlohmann::json styleConfig_ = {{"startOnZero", true}};
    
    XStyle styleConfig_;
    
    XDataGroup dataArray_;
    map<AttrType, unique_ptr<AttrBase>> attrs_{};

    Group *container_ = nullptr;
    utils::Tracer *tracker_ = nullptr;
};
} // namespace geom
} // namespace xg

#endif // GRAPHICS_GEOM_GEOM_H
