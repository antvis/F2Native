#ifndef GRAPHICS_GEOM_GEOM_H
#define GRAPHICS_GEOM_GEOM_H

#include <map>
#include <set>
#include "attr/AttrBase.h"
#include "../animate/GeomAnimate.h"
#include "../canvas/Container.h"
#include "../canvas/Coord.h"
#include "../shape/Group.h"
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
    AbstractGeom &Attrs(const std::string &attrs);
    AbstractGeom &Selection(const std::string &selection);
    
    AbstractGeom &StyleObject(const nlohmann::json &cfg);
    AbstractGeom &AttrsObject(const nlohmann::json &cfg);
    AbstractGeom &SelectionObject(const nlohmann::json &cfg);
    //新增动画参数
    AbstractGeom &Animate(const nlohmann::json &cfg);
    

    const unique_ptr<attr::AttrBase> &GetColor();

    // create scale & attrs & processData
    void Init(XChart *chart);

    void Paint(XChart *chart);

    const std::string &GetXScaleField();
    const std::string &GetYScaleField();

    std::string GetType() const { return this->type_; }

    bool ContainsAttr(attr::AttrType type);
    
    const bool isSelectionEnable();
    const bool isDeselect();
    const bool isSelected(const nlohmann::json &record);

    const nlohmann::json GetSelectedRecords();
    nlohmann::json GetSnapRecords(XChart *chart, util::Point point);
    nlohmann::json GetSnapRecords(XChart *chart, util::Point point, const bool needReMapping);
    const nlohmann::json GetXSnapRecords(XChart *chart, double invertPointX, const bool needReMapping);
    const nlohmann::json GetYSnapRecords(XChart *chart, double invertPointY, const bool needReMapping);
    const nlohmann::json &GetLastSnapRecord(XChart *chart);
    const nlohmann::json &GetFirstSnapRecord(XChart *chart);
    const nlohmann::json GetTickRecord(XChart *chart, std::string field, xg::scale::Tick tick);
    nlohmann::json &getAnimateCfg();

    const std::unique_ptr<AttrBase> &GetAttr(AttrType type) { return attrs_[type]; }
    const bool isAdjustDodge();

    virtual void Clear();

    const nlohmann::json &GetDataArray() { return dataArray_; }
    const nlohmann::json &GetSelectedArray() { return selectedArray_; }
    void SetSelectedArray(const nlohmann::json &selectedArray);
    void SetSelectedArray(const nlohmann::json &selectedArray, const bool needReMapping);
    
    const Group *GetContainer() { return container_; }

    virtual void SetAttrs(const std::string &_attrs) noexcept;
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
    virtual void ProcessScale(XChart &chart) {};
    nlohmann::json GroupData(XChart &chart);
    const set<string> GetGroupFieldNames(XChart &chart);
    // void GetAttrValues(XChart &chart, const AttrBase &attr, AttrCfg &acfg, DrawCfg &dcfg);

    virtual void Mapping(XChart &chart, nlohmann::json &data, std::size_t start, std::size_t end);

    virtual void BeforeMapping(XChart &chart, nlohmann::json &dataArray) {}

    virtual void Draw(XChart &chart, const nlohmann::json &groupData, std::size_t start, std::size_t end) const;

    void updateStackRange(XChart &chart);

    virtual double GetYMinValue(XChart &chart);

  protected:
    /*各种属性*/
    bool generatePoints_ = false;
    bool sortable_ = false;
    bool visiable_ = true;
    bool connectNulls_ = false;
    bool enableSelection_ = false;
    bool deselect_ = false;
    nlohmann::json selectedArray_ = nullptr;
    nlohmann::json selectedStyle_ = nullptr;
    nlohmann::json unSelectedStyle_ = nullptr;
    string selectedColor_ = "";
    bool ignoreEmptyGroup_ = false;
    string type_ = "";
    string shapeType_ = "";
    nlohmann::json styleConfig_ = {{"startOnZero", true}};

    nlohmann::json dataArray_;
    map<AttrType, unique_ptr<AttrBase>> attrs_{};

    Group *container_ = nullptr;
    utils::Tracer *tracker_ = nullptr;
    
    nlohmann::json animateCfg_ = {};
};
} // namespace geom
} // namespace xg

#endif // GRAPHICS_GEOM_GEOM_H
