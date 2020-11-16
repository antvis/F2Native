#ifndef XG_GRAPHICS_GEOM_ATTR_H
#define XG_GRAPHICS_GEOM_ATTR_H

#include "graphics/canvas/Coord.h"
#include "graphics/scale/Category.h"
#include "graphics/scale/Scale.h"
#include <map>
#include <math.h>
#include <string>
#include <vector>

using namespace xg::canvas::coord;
using namespace xg::scale;
using namespace xg::util;
using namespace std;

namespace xg {
namespace attr {

enum class AttrType { Position = 1, Color, Size, Shape, Adjust };

class AttrBase {
  public:
    AttrBase(const vector<string> &fields, const vector<string> &names) : fields_(fields), names_(names) {}

    virtual ~AttrBase() = default;

    const vector<string> &GetNames() const { return names_; }
    const vector<string> &GetFields() const { return fields_; }

    virtual AttrType GetType() const = 0;

    virtual void
    Mapping(nlohmann::json &groupData, const AbstractScale &xScale, const AbstractScale &yScale, const AbstractCoord &coord) const {};

  protected:
    vector<string> fields_;
    vector<string> names_;
};

class Position : public AttrBase {
  public:
    Position(const vector<string> &fields) : AttrBase(fields, {"x", "y"}) {
        if(this->fields_.size() <= 0 || this->fields_.size() > 2) {
            throw runtime_error("fields is invalid.");
        }
    }

    AttrType GetType() const override { return AttrType::Position; }

    void Mapping(nlohmann::json &groupData, const AbstractScale &xScale, const AbstractScale &yScale, const AbstractCoord &coord) const override {
        for(size_t i = 0; i < groupData.size(); i++) {
            auto &item = groupData[i];
            nlohmann::json &xVal = item[fields_[0]];
            nlohmann::json &yVal = item[fields_[1]];

            if(xVal.is_null() && yVal.is_null()) {
                item["_x"] = std::nan("0"); // attr names[x, y]
                item["_y"] = std::nan("0");
            } else if(xVal.is_array() && yVal.is_array()) {
                // TODO
            } else if(xVal.is_array()) {
                // TODO
            } else if(yVal.is_array()) {
                double x = xScale.Scale(item[fields_[0]]);

                double rstX;
                nlohmann::json rstY;
                for(std::size_t index = 0; index < yVal.size(); ++index) {
                    util::Point _point = coord.ConvertPoint(util::Point{x, yScale.Scale(yVal[index])});
                    rstX = _point.x;
                    rstY.push_back(_point.y);
                }
                item["_x"] = rstX;
                item["_y"] = rstY;
            } else {
                double x = xScale.Scale(item[fields_[0]]);
                double y = yScale.Scale(item[fields_[1]]);
                util::Point point = coord.ConvertPoint(util::Point(x, y));
                item["_x"] = point.x; // attr names[x, y]
                item["_y"] = point.y;
            }
        }
    }
};

class Color : public AttrBase {
  public:
    Color(const vector<string> &fields, const vector<string> &colors) : AttrBase(fields, {"color"}), colors_(colors) {
        if(fields_.size() != 1) {
            throw runtime_error("fields must be one element.");
        }
    };

    Color(const string &color) : AttrBase({}, {"color"}) { colors_.push_back(color); }

    AttrType GetType() const override { return AttrType::Color; }

    inline const string &GetColor(int index) const { return index < colors_.size() ? colors_[index] : colors_[0]; }

    void Mapping(nlohmann::json &groupData, const AbstractScale &xScale, const AbstractScale &yScale, const AbstractCoord &coord) const override {
        if(fields_.empty()) {
            //只赋值第一个对象
            if(groupData.size()) {
                groupData[0]["_color"] = colors_[0];
            }
        } else {
            if(xScale.GetType() == ScaleType::Cat || xScale.GetType() == ScaleType::TimeCat) {
                const Category &cat = (Category &)xScale;
                for(size_t i = 0; i < groupData.size(); i++) {
                    auto &item = groupData[i];
                    int index = cat.Transform(item[fields_[0]]);
                    item["_color"] = colors_[index];
                }
            }
        }
    }

  private:
    vector<string> colors_{};
};

class Size : public AttrBase {
  public:
    Size(const string &field, const vector<float> &sizes) : AttrBase({field}, {"color"}), sizes_(sizes) {}

    Size(const float size) : AttrBase({}, {"size"}) { sizes_.push_back(size); }

    inline const float GetSize(int index) const { return index < sizes_.size() ? sizes_[index] : sizes_[0]; }

    AttrType GetType() const override { return AttrType::Size; }

    void Mapping(nlohmann::json &groupData, const AbstractScale &xScale, const AbstractScale &yScale, const AbstractCoord &coord) const override {
        //只赋值第一个对象
        if(fields_.empty()) {
            groupData[0]["_size"] = sizes_[0];
        }
    }

  private:
    vector<float> sizes_{};
};

class Shape : public AttrBase {
  public:
    Shape(const string &field, const vector<string> &shapes) : AttrBase({field}, {"shape"}), shapes_(shapes){};

    Shape(const string &shape) : AttrBase({}, {"shape"}) { shapes_.push_back(shape); }

    AttrType GetType() const override { return AttrType::Shape; }

    inline const string &GetShape(int index) const { return index < shapes_.size() ? shapes_[index] : shapes_[0]; }

    void Mapping(nlohmann::json &groupData, const AbstractScale &xScale, const AbstractScale &yScale, const AbstractCoord &coord) const override {
        //只赋值第一个对象
        if(fields_.empty()) {
            groupData[0]["_shape"] = shapes_[0];
        }
    }

  private:
    vector<string> shapes_{};
};

class Adjust : public AttrBase {
  public:
    Adjust(const string &field, const string &adjust) : AttrBase({field}, {"adjust"}), adjust_(adjust){};

    AttrType GetType() const override { return AttrType::Adjust; }

    inline const string &GetAdjust() const { return adjust_; }

    void Mapping(nlohmann::json &groupData, const AbstractScale &xScale, const AbstractScale &yScale, const AbstractCoord &coord) const override {
        //只赋值第一个对象
        if(fields_.empty()) {
            groupData[0]["_adjust"] = adjust_;
        }
    }

  private:
    string adjust_;
};

} // namespace attr
} // namespace xg

#endif // XG_GRAPHICS_GEOM_ATTR_H
