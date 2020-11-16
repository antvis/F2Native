#ifndef XG_GRAPHICS_UTIL_BBOX_H
#define XG_GRAPHICS_UTIL_BBOX_H

#include "graphics/util/Point.h"
#include <math.h>
#include <vector>

using namespace std;

namespace xg {
namespace util {

typedef struct BBox_ {
    //顺序固定
    float minX, maxX, minY, maxY, width, height, x, y;
} BBox;

class BBoxUtil {
    public:
    static BBox GetBBoxFromLine(float x0, float y0, float x1, float y1, float lineWidth) {
        const float hafWidth = lineWidth / 2;
        return { fmin(x0, x1) - hafWidth, fmax(x0, x1) + hafWidth, fmin(y0, y1) - hafWidth, fmax(y0, y1) + hafWidth, .0f, .0f };
    }

    static BBox GetBBoxFromPoints(const std::vector<Point> &points, float lineWidth) {
        if(points.empty()) {
            return { 0 };
        }
        auto &p = points[0];
        auto left = p.x;
        auto right = p.x;
        auto top = p.y;
        auto bottom = p.y;

        for(int i = 1; i < points.size(); ++i) {
            auto &p1 = points[i];
            left = min(left, p1.x);
            right = max(right, p1.x);
            top = min(top, p1.y);
            bottom = max(bottom, p1.y);
        }

        const float hafWidth = lineWidth / 2;
        return { static_cast<float>(left - hafWidth),
                 static_cast<float>(right + hafWidth),
                 static_cast<float>(top - hafWidth),
                 static_cast<float>(bottom + hafWidth),
                 .0f,
                 .0f };
    }
};

} // namespace util
} // namespace xg

#endif /* XG_GRAPHICS_UTIL_BBOX_H */
