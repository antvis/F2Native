#ifndef XG_GRAPHICS_UTIL_MATRIX_H
#define XG_GRAPHICS_UTIL_MATRIX_H

#include <array>
#include <math.h>
#include <string>
#include "Vector2d.h"

namespace xg {
namespace util {

using Matrix = std::array<double, 6>;

struct TransformAction {
    std::string type;
    Vector2D v;

    TransformAction(std::string type, Vector2D v) {
        this->type = type;
        this->v = v;
    }
};

class MatrixUtil {
  public:
    static bool IsUnchanged(const Matrix &m) {
        return xg::IsEqual(m[0], 1.) && xg::IsEqual(m[1], 0.) && xg::IsEqual(m[2], 0.) && xg::IsEqual(m[3], 1.) &&
               xg::IsEqual(m[4], 0.) && xg::IsEqual(m[5], 0);
    }

    static void Reset(Matrix *matrix) {
        (*matrix)[0] = 1.0;
        (*matrix)[1] = 0.0;
        (*matrix)[2] = 0.0;
        (*matrix)[3] = 1.0;
        (*matrix)[4] = 0.0;
        (*matrix)[5] = 0.0;
    }

    static void Scale(Matrix *out, Matrix &matrix, const Vector2D &vector) {
        (*out)[0] = matrix[0] * vector[0];
        (*out)[1] = matrix[1] * vector[0];
        (*out)[2] = matrix[2] * vector[1];
        (*out)[3] = matrix[3] * vector[1];
        (*out)[4] = matrix[4];
        (*out)[5] = matrix[5];
    }

    static void Rotate(Matrix *out, const Matrix &matrix, double r) {
        double c = std::cos(r);
        double s = std::sin(r);
        double m11 = matrix[0] * c + matrix[2] * s;
        double m12 = matrix[1] * c + matrix[3] * s;
        double m21 = matrix[0] * -s + matrix[2] * c;
        double m22 = matrix[1] * -s + matrix[3] * c;
        (*out)[0] = m11;
        (*out)[1] = m12;
        (*out)[2] = m21;
        (*out)[3] = m22;
        (*out)[4] = matrix[4];
        (*out)[5] = matrix[5];
    }

    static void Translate(Matrix *out, Matrix &matrix, const Vector2D &vector) {
        (*out)[0] = matrix[0];
        (*out)[1] = matrix[1];
        (*out)[2] = matrix[2];
        (*out)[3] = matrix[3];
        (*out)[4] = matrix[4] + matrix[0] * vector[0] + matrix[2] * vector[1];
        (*out)[5] = matrix[5] + matrix[1] * vector[0] + matrix[3] * vector[1];
    }

    static void Transform(Matrix *out, const std::vector<TransformAction> &actions) {
        std::for_each(actions.begin(), actions.end(), [&](const TransformAction &action) -> void {
            if(action.type == "t") {
                Translate(out, *out, action.v);
            } else if(action.type == "s") {
                Scale(out, *out, action.v);
            } else if(action.type == "r") {
                Rotate(out, *out, action.v[1]);
            }
        });
    }
};
} // namespace util
} // namespace xg
#endif // XG_GRAPHICS_UTIL_MATRIX_H
