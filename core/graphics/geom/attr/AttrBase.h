#ifndef XG_GRAPHICS_GEOM_ATTR_H
#define XG_GRAPHICS_GEOM_ATTR_H

#include <map>
#include <math.h>
#include <string>
#include <vector>
#include "../../canvas/Coord.h"
#include "../../canvas/CanvasFillStrokeStyle.h"
#include "../../scale/Category.h"
#include "../../scale/Scale.h"
#include "../../util/json_data.h"

using namespace xg::canvas::coord;
using namespace xg::scale;
using namespace xg::util;
using namespace std;

namespace xg {
namespace attr {

enum class AttrType { Position = 1, Color, Size, Shape, Adjust, Opacity };

class AttrBase {
  public:
    AttrBase(const vector<string> &fields, const vector<string> &names) : fields_(fields), names_(names) {}

    virtual ~AttrBase() = default;

    const vector<string> &GetNames() const { return names_; }
    const vector<string> &GetFields() const { return fields_; }

    virtual AttrType GetType() const = 0;

    virtual void
    Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord){};

  protected:
    vector<string> fields_;
    vector<string> names_;
};

class Position : public AttrBase {
  public:
    Position(const vector<string> &fields) : AttrBase(fields, {"x", "y"}) {
        // if(this->fields_.size() <= 0 || this->fields_.size() > 2) {
        // throw runtime_error("fields is invalid.");
        // }
    }

    AttrType GetType() const override { return AttrType::Position; }

    void Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord) override {
        auto &xField = fields_[0];
        auto &yField = fields_[1];
        for(size_t i = start; i <= end; i++) {
            auto &item = groupData[i];
            auto xVal = item.data->count(xField) ? item.data->at(xField) : Any();
            auto yVal = item.data->count(yField) ? item.data->at(yField) : Any();

            if(xVal.IsEmpty() && yVal.IsEmpty()) {
                item._x = std::nan("0"); // attr names[x, y]
                item._y = std::nan("0");
            } else if(xVal.GetType().IsArray() && yVal.GetType().IsArray()) {
                // TODO
            } else if(xVal.GetType().IsArray()) {
                // TODO
            } else if(item.adjust.size() >= 2) { //stack
                double x = xScale.Scale(item.data->at(xField));

                std::vector<double> rstY;
                for(std::size_t index = 0; index < item.adjust.size(); ++index) {
                    util::Point _point = coord.ConvertPoint(util::Point{x, yScale.Scale(item.adjust[index])});
                    item._x  = _point.x;
                    rstY.push_back(_point.y);
                }
                item._y0 = rstY;
            } else if(item.dodge.size() >= 1) { //dodge
                double x = xScale.Scale(item.dodge[0]);
                double y = item.dodge.size() >= 2 ? yScale.Scale(item.dodge[1]) : yScale.Scale(item.data->at(fields_[1]));
                util::Point point = coord.ConvertPoint(util::Point(x, y));
                item._x = point.x; // attr names[x, y]
                item._y = point.y;
            } else if(!yVal.IsEmpty() && yVal.GetType().IsArray()) { //区间柱状图
                double x = xScale.Scale(xVal);
                std::vector<double> rstY;
                auto &yValCast = yVal.Cast<std::vector<double> &>();
                for(std::size_t index = 0; index < yValCast.size(); ++index) {
                    util::Point _point = coord.ConvertPoint(util::Point{x, yScale.Scale(yValCast[index])});
                    item._x  = _point.x;
                    rstY.push_back(_point.y);
                }
                item._y0 = rstY;
            }
            else {
                double x = xScale.Scale(xVal);
                double y = yScale.Scale(yVal);
                util::Point point = coord.ConvertPoint(util::Point(x, y));
                item._x = point.x; // attr names[x, y]
                item._y = point.y;
            }
        }
    }
};

class Color : public AttrBase {
  public:
    Color(const vector<string> &fields, const vector<string> &colors)
        : AttrBase(fields, {"color"}), colors_(colors){
                                           // if(fields_.size() != 1) {
                                           // throw runtime_error("fields must be one element.");
                                           // }
                                       };

    Color(const string &color) : AttrBase({}, {"color"}), color_(color) { }
    Color(const canvas::CanvasLinearGradient &color) : AttrBase({}, {"color"}), color_(color) {  }
    Color(const canvas::CanvasRadialGradient &color) : AttrBase({}, {"color"}), color_(color) { }

    AttrType GetType() const override { return AttrType::Color; }

    inline const string &GetColor(int index) const { return index < colors_.size() ? colors_[index] : colors_[0]; }

    void Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord) override {
        //设置的是一个固定的颜色
        if (fields_.empty()) {
            if (colors_.empty()) {
                for(std::size_t index = start; index <= end; ++index) {
                    groupData[index]._color = color_;
                }
            } else {
                for(std::size_t index = start; index <= end; ++index) {
                    groupData[index]._color = canvas::CanvasFillStrokeStyle(colors_[0]);
                }
            }
        } else {
            for(size_t i = start; i <= end; i++) {
                auto &item = groupData[i];
                if(scale::IsCategory(xScale.GetType())) {
                    const scale::Category &cat = (scale::Category &)xScale;
                    std::size_t index = cat.Transform(item.data->at(fields_[0]));
                    item._color = canvas::CanvasFillStrokeStyle(colors_[index]);
                } else {
                    item._color = canvas::CanvasFillStrokeStyle(colors_[0]);
                }
            }
        }
    }

  private:
    vector<string> colors_;
    canvas::CanvasFillStrokeStyle color_;
};

class Size : public AttrBase {
  public:
    Size(const string &field, const vector<float> &sizes) : AttrBase({field}, {"color"}), sizes_(sizes) {}

    Size(const float size) : AttrBase({}, {"size"}) { sizes_.push_back(size); }

    inline const float GetSize(int index) const { return index < sizes_.size() ? sizes_[index] : sizes_[0]; }

    AttrType GetType() const override { return AttrType::Size; }

    void Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord) override {
        //用户设置的是一个固定的size
        if (fields_.empty()) {
            for(std::size_t index = start; index <= end; ++index) {
                groupData[index]._size = sizes_[0];
            }
        } else {
            for(std::size_t index = start; index <= end; ++index) {
                auto &item = groupData[index];
                if(scale::IsCategory(xScale.GetType())) {
                    std::size_t val = static_cast<scale::Category &>(xScale).Transform(item.data->at(xScale.field));
                    item._size = sizes_[val % sizes_.size()];
                } else {
                    double percent = xScale.Scale(item.data->at(xScale.field));
                    item._size = sizes_[GetLinear(percent) % sizes_.size()];
                }
            }
        }
    }

  private:
    std::size_t GetLinear(double percent) {
        std::size_t steps = sizes_.size() - 1;
        std::size_t step = static_cast<std::size_t>(floor(steps * percent));
        double leftPercent = steps * percent - step;
        std::size_t start = sizes_[step];
        std::size_t end = step == steps ? start : sizes_[step + 1];
        std::size_t rst = static_cast<std::size_t>(start + (end - start) * leftPercent);
        return rst;
    }

  private:
    vector<float> sizes_{};
};

class Shape : public AttrBase {
  public:
    Shape(const string &field, const vector<string> &shapes) : AttrBase({field}, {"shape"}), shapes_(shapes){};

    Shape(const string &shape) : AttrBase({}, {"shape"}) { shapes_.push_back(shape); }

    AttrType GetType() const override { return AttrType::Shape; }

    // inline const string &GetShape(int index) const { return index < shapes_.size() ? shapes_[index] : shapes_[0]; }

    void Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord) override {
        if(shapes_.empty()) {
            return;
        }
        for(std::size_t index = start; index <= end; ++index) {
            groupData[index]._shape = shapes_[0];
        }
    }

  private:
    vector<string> shapes_{};
};

class Adjust : public AttrBase {
  public:
    Adjust(const string &field, const string &adjust) : AttrBase({field}, {"adjust"}), adjust_(adjust){};
    Adjust(const string &adjust) : AttrBase({}, {"adjust"}), adjust_(adjust){};

    AttrType GetType() const override { return AttrType::Adjust; }

    inline const string &GetAdjust() const { return adjust_; }

    void Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord) override {
        for(std::size_t index = start; index <= end; ++index) {
            groupData[index]._adjust = adjust_;
        }
    }

  private:
    string adjust_;
};

class Opacity : public AttrBase {
  public:
    Opacity(const string &field, const vector<float> &opacity) : AttrBase({field}, {"color"}), opacity_(opacity) {}

    Opacity(const float opacity) : AttrBase({}, {"opacity"}) { opacity_.push_back(opacity); }

    inline const float GetSize(int index) const { return index < opacity_.size() ? opacity_[index] : opacity_[0]; }

    AttrType GetType() const override { return AttrType::Opacity; }

    void Mapping(XDataArray &groupData, std::size_t start, std::size_t end, AbstractScale &xScale, AbstractScale &yScale, const AbstractCoord &coord) override {
        //用户设置的是一个固定的size
        if (fields_.empty()) {
            for(std::size_t index = start; index <= end; ++index) {
                groupData[index]._size = opacity_[0];
            }
        } else {
            for(std::size_t index = start; index <= end; ++index) {
                auto &item = groupData[index];
                item._opacity = opacity_[index];
            }
        }
    }

 

  private:
    vector<float> opacity_;
};

} // namespace attr
} // namespace xg

#endif // XG_GRAPHICS_GEOM_ATTR_H
