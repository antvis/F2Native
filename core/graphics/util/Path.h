#ifndef XG_GRAPHICS_UTIL_PATH_H
#define XG_GRAPHICS_UTIL_PATH_H

#include <array>
#include <limits>
#include <math.h>
#include <vector>
#include "Point.h"
#include "Vector2d.h"
#include "../../utils/common.h"

namespace xg {
namespace util {
using Path = std::vector<Point>;
using BezierPath = std::vector<BezierPoint>;

class PathUtil {
  public:
    static BezierPath getSplinePath(const std::vector<Point> &points, bool isInCircle, std::array<Point, 2> &constraint) {
        std::vector<double> data;
        Point first = points[0];
        Point prePoint;
        std::size_t len = points.size();
        for(std::size_t i = 0; i < len; i++) {
            Point point = points[i];
            if(!xg::IsEqual(prePoint.x, point.x) || !xg::IsEqual(prePoint.y, point.y)) {
                data.push_back(point.x);
                data.push_back(point.y);
                prePoint.Reset(point.x, point.y);
            }
        }
        // std::array<Point, 2> constr = { Point(0, 0), Point(1, 1) };
        BezierPath splinePath = catmullRom2bezier(data, isInCircle, constraint);
        // splinePath.unshift(['M', first.x, first.y]);
        return splinePath;
    }

    static BezierPath catmullRom2bezier(std::vector<double> &crp, bool z, std::array<Point, 2> &constraint) {
        bool isLoop = !!z;
        std::vector<Point> pointList;
        std::size_t len = crp.size();
        for(std::size_t i = 0; i < len; i += 2) {
            Point p = Point(crp[i], crp[i + 1]);
            pointList.push_back(p);
        }
        Path controlPointList = SmoothBezier(pointList, 0.4, isLoop, constraint);
        std::size_t p_len = pointList.size();
        BezierPath d1;
        Point cp1;
        Point cp2;
        Point p;

        for(std::size_t i = 0; i < p_len - 1; i++) {
            cp1 = controlPointList[i * 2];
            cp2 = controlPointList[i * 2 + 1];
            p = pointList[i + 1];
            d1.push_back(BezierPoint(cp1, cp2, p));
        }
        if(isLoop) {
            cp1 = controlPointList[p_len];
            cp2 = controlPointList[p_len + 1];
            p = pointList[0];
            d1.push_back(BezierPoint(cp1, cp2, p));
        }
        return d1;
    }

  private:
    static Path SmoothBezier(std::vector<Point> &points, double smooth, bool isLoop, std::array<Point, 2> &constraint) {
        std::vector<Point> cps;
        util::Point prevPoint;
        util::Point nextPoint;
        util::Point min;
        util::Point max;
        min = util::Point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());
        max = util::Point(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
        std::size_t len = points.size();
        for(std::size_t i = 0; i < len; i++) {
            util::Point point = points[i];
            min = util::Point::Min(min, point);
            max = util::Point::Max(max, point);
        }
        min = util::Point::Min(min, constraint[0]);
        max = util::Point::Max(max, constraint[1]);

        for(std::size_t i = 0; i < len; i++) {
            Point point = points[i];
            if(isLoop) {
                prevPoint = points[i ? i - 1 : len - 1];
                nextPoint = points[(i + 1) % len];
            } else {
                if(i == 0 || i == len - 1) {
                    cps.push_back(point);
                    continue;
                } else {
                    prevPoint = points[i - 1];
                    nextPoint = points[i + 1];
                }
            }
            util::Point v = Point(0, 0);
            v = util::Point::Sub(nextPoint, prevPoint);
            v = util::Point::Scale(v, smooth);

            double d0 = util::Point::Distance(point, prevPoint);
            double d1 = util::Point::Distance(point, nextPoint);

            double sum = d0 + d1;
            if(!xg::IsZero(sum)) {
                d0 /= sum;
                d1 /= sum;
            }

            Point v1 = util::Point::Scale(v, -d0);
            Point v2 = util::Point::Scale(v, d1);

            Point cp0 = util::Point::Add(point, v1);
            Point cp1 = util::Point::Add(point, v2);

            cp0 = util::Point::Max(cp0, min);
            cp0 = util::Point::Min(cp0, max);
            cp1 = util::Point::Max(cp1, min);
            cp1 = util::Point::Min(cp1, max);

            cps.push_back(cp0);
            cps.push_back(cp1);
        }

        if(isLoop) {
            // cps.push_back(cps.shift());
        }
        return cps;
    }
};
} // namespace util
} // namespace xg
#endif
