#ifndef XG_GRAPHICS_UTIL_MATRIX_H
#define XG_GRAPHICS_UTIL_MATRIX_H

#include "graphics/util/Vector2d.h"
#include <array>
#include <math.h>

namespace xg {
namespace util {

using Matrix = std::array<double, 6>;
class MatrixUtil {
    public:
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

    static void Rotate(Matrix *out, const std::array<double, 6> &matrix, int r) {
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
};
} // namespace util
} // namespace xg
#endif // XG_GRAPHICS_UTIL_MATRIX_H
