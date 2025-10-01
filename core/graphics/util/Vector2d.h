#ifndef XG_GRAPHICS_UTIL_VECTOR2D_H
#define XG_GRAPHICS_UTIL_VECTOR2D_H

#include <array>
#include <math.h>
#include "../../utils/common.h"

namespace xg {
namespace util {

using Vector2D = std::array<double, 2>;
class Vector2DUtil {
  public:
    static void Reset(Vector2D *vector, double a = 0.0, double b = 0.0) {
        (*vector)[0] = a;
        (*vector)[1] = b;
    }

    static void TransformMat2D(Vector2D *out, Vector2D &v, const std::array<double, 6> &m) {
        double x = v[0];
        double y = v[1];

        (*out)[0] = m[0] * x + m[2] * y + m[4];
        (*out)[1] = m[1] * x + m[3] * y + m[5];
    }

    static Vector2D Max(Vector2D &v1, Vector2D &v2) {
        Vector2D out;
        out[0] = fmax(v1[0], v2[0]);
        out[1] = fmax(v1[1], v2[1]);
        return out;
    }

    static Vector2D Min(Vector2D &v1, Vector2D &v2) {
        Vector2D out;
        out[0] = fmin(v1[0], v2[0]);
        out[1] = fmin(v1[1], v2[1]);
        return out;
    }

    static Vector2D Add(Vector2D &v1, Vector2D &v2) {
        Vector2D out;
        out[0] = v1[0] + v2[0];
        out[1] = v1[1] + v2[1];
        return out;
    }

    static Vector2D Sub(Vector2D &v1, Vector2D &v2) {
        Vector2D out;
        out[0] = v1[0] - v2[0];
        out[1] = v1[1] - v2[1];
        return out;
    }

    static Vector2D Scale(Vector2D &v, double b) {
        Vector2D out;
        out[0] = v[0] * b;
        out[1] = v[1] * b;
        return out;
    }

    static double Distance(Vector2D &v1, Vector2D &v2) {
        double x = v2[0] - v1[0];
        double y = v2[1] - v1[1];
        return hypot(x, y);
    }

    static double Dot(Vector2D &v1, Vector2D &v2) { return v1[0] * v2[0] + v1[1] * v2[1]; }

    static double Length(Vector2D &v) {
        double x = v[0];
        double y = v[1];
        return std::sqrt(x * x + y * y);
    }

    static double Angle(Vector2D &v1, Vector2D &v2) {
        double theta = Vector2DUtil::Dot(v1, v2) / (Vector2DUtil::Length(v1) * Vector2DUtil::Length(v2));
        return std::acos(theta);
    }

    static double Direction(Vector2D &v1, Vector2D &v2) { return v1[0] * v2[1] - v2[0] * v1[1]; }

    static bool Zero(Vector2D &v) {
        if(abs(v[0]) < XG_EPS && abs(v[1]) < XG_EPS) {
            return true;
        }
        return false;
    }

    static double AngleTo(Vector2D &v1, Vector2D &v2, bool direction) {
        double angle = Vector2DUtil::Angle(v1, v2);
        bool angleLargeThanPI = Vector2DUtil::Direction(v1, v2) >= XG_EPS;
        if(direction) {
            if(angleLargeThanPI) {
                return M_PI * 2 - angle;
            }
            return angle;
        }

        if(angleLargeThanPI) {
            return angle;
        }
        return M_PI * 2 - angle;
    }
    
    static Vector2D Normalize(Vector2D &v) {
        double length = Length(v);
        if(xg::IsZero(length)) {
            return {0, 0};
        }
        return {v[0] / length, v[1] / length};
    }
};
} // namespace util
} // namespace xg
#endif
