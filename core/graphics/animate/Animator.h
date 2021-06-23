#include "graphics/shape/Shape.h"
#include <math.h>

#ifndef XG_GRAPHICS_ANIMATE_ANIMATOR_H
#define XG_GRAPHICS_ANIMATE_ANIMATOR_H

namespace xg {
namespace animate {

struct AnimateState {
    util::Matrix matrix{1.0, 0, 0, 1., 0, 0};
    // std::vector<util::Point> points;
    float width = 0;
    float height = 0;
    float angle = 0.;
};

struct InterpolateNumber {
    double a = 0, b = 0;
    InterpolateNumber(double _a, double _b) {
        a = +_a;
        b = _b - a;
    }

    void interval(double *out, double t) { (*out) = a + b * t; }
};

struct InterpolateMatrix {
    InterpolateMatrix(util::Matrix a, util::Matrix b) {
        std::size_t na = a.size();

        std::size_t i = 0;
        for(; i < na; ++i) {
            x.push_back(InterpolateNumber(a[i], b[i]));
        }
    }

    void interval(util::Matrix *out, double t) {
        for(std::size_t i = 0; i < x.size(); ++i) {
            x[i].interval(&c[i], t);
        }

        for(std::size_t i = 0; i < c.size(); ++i) {
            (*out)[i] = c[i];
        }
    }

    std::vector<InterpolateNumber> x;
    util::Matrix c;
};

struct InterpolateArray {
    InterpolateArray(std::vector<double> &a, std::vector<double> &b) {
        std::size_t nb = b.size();
        std::size_t na = fmin(nb, a.size());

        std::size_t i = 0;
        for(; i < na; ++i) {
            x.push_back(InterpolateNumber(a[i], b[i]));
        }

        for(i; i < nb; ++i) {
            c[i] = b[i];
        }
    }

    void interval(std::vector<double> *out, double t) {
        for(std::size_t i = 0; i < x.size(); ++i) {
            x[i].interval(&c[i], t);
        }

        for(std::size_t i = 0; i < c.size(); ++i) {
            (*out)[i] = c[i];
        }
    }

    std::vector<InterpolateNumber> x;
    std::vector<double> c;
};

/// 针对不同属性类型的插值器
struct Interpolator {
    std::unique_ptr<InterpolateMatrix> matrix = nullptr;
    std::unique_ptr<InterpolateArray> points = nullptr;
    // numbers
    std::map<std::string, std::unique_ptr<InterpolateNumber>> attrs;
};

struct AnimInfo {
    shape::Shape *shape = nullptr;
    std::size_t delay = 0;
    std::size_t duration = 0;
    std::string erasing;
    AnimateState startState;
    AnimateState endState;
    Interpolator interpolate;
    bool hasStarted = false;
    bool hasEnded = false;
    long startTime = 0;
    long endTime = 0;
    std::function<void()> onEnd = nullptr;
};

static AnimInfo CreateAnimInfo(shape::Shape *shape, const AnimateState &source, const AnimateState &end, nlohmann::json &cfg) {
    AnimInfo animInfo;
    animInfo.delay = cfg["delay"];
    animInfo.duration = cfg["duration"];
    animInfo.erasing = cfg["erasing"];

    Interpolator interpolator;
    if(!MatrixUtil::IsUnchanged(source.matrix)) {
        interpolator.matrix = std::make_unique<InterpolateMatrix>(source.matrix, end.matrix);
    }

    if(end.width > 0) {
        interpolator.attrs["width"] = std::make_unique<InterpolateNumber>(source.width, end.width);
    }
    if(end.height > 0) {
        interpolator.attrs["height"] = std::make_unique<InterpolateNumber>(source.height, end.height);
    }

    if(end.angle > 0) {
        interpolator.attrs["endAngle"] = std::make_unique<InterpolateNumber>(source.angle, end.angle);
    }

    animInfo.interpolate = std::move(interpolator);
    animInfo.startState = source;
    animInfo.endState = end;
    animInfo.shape = shape;

    return std::move(animInfo);
}

namespace erasing {

static double linear(double k) { return k; }

static double quadraticIn(double k) { return k * k; }

static double quadraticOut(double k) { return k * (2 - k); }

static double quadraticInOut(double k) {
    if((k *= 2) < 1) {
        return 0.5 * k * k;
    }
    return -0.5 * (--k * (k - 2) - 1);
}

static double cubicIn(double k) { return k * k * k; }
static double cubicOut(double k) { return --k * k * k + 1; }
static double cubicInOut(double k) {
    if((k *= 2) < 1) {
        return 0.5 * k * k * k;
    }
    return 0.5 * ((k -= 2) * k * k + 2);
}

static double elasticIn(double k) {
    double s;
    double a = 0.1;
    double p = 0.4;
    if(IsEqual(k, 0))
        return 0;
    if(IsEqual(k, 1))
        return 1;
    if(!p) {
        p = 0.3;
    }
    if(!a || a < 1) {
        a = 1;
        s = p / 4;
    } else {
        s = p / (2 * M_PI) * asin(1 / a);
    }
    return -(a * pow(2, 10 * (k -= 1)) * sin((k - s) * (2 * M_PI) / p));
}

static double elasticOut(double k) {
    double s;
    double a = 0.1;
    double p = 0.4;
    if(IsEqual(k, 0))
        return 0;
    if(IsEqual(k, 1))
        return 1;
    if(!p) {
        p = 0.3;
    }
    if(!a || a < 1) {
        a = 1;
        s = p / 4;
    } else {
        s = p / (2 * M_PI) * asin(1 / a);
    }
    return (a * pow(2, -10 * k) * sin((k - s) * (2 * M_PI) / p) + 1);
}

static double elasticInOut(double k) {
    double s;
    double a = 0.1;
    double p = 0.4;
    if(IsEqual(k, 0))
        return 0;
    if(IsEqual(k, 1))
        return 1;
    if(!p) {
        p = 0.3;
    }
    if(!a || a < 1) {
        a = 1;
        s = p / 4;
    } else {
        s = p / (2 * M_PI) * asin(1 / a);
    }
    if((k *= 2) < 1) {
        return -0.5 * (a * pow(2, 10 * (k -= 1)) * sin((k - s) * (2 * M_PI) / p));
    }
    return a * pow(2, -10 * (k -= 1)) * sin((k - s) * (2 * M_PI) / p) * 0.5 + 1;
}

static double backIn(double k) {
    double s = 1.70158;
    return k * k * ((s + 1) * k - s);
}

static double backOut(double k) {
    double s = 1.70158;
    return (k = k - 1) * k * ((s + 1) * k + s) + 1;
}

static double backInOut(double k) {
    double s = 1.70158 * 1.525;
    if((k *= 2) < 1) {
        return 0.5 * (k * k * ((s + 1) * k - s));
    }
    return 0.5 * ((k -= 2) * k * ((s + 1) * k + s) + 2);
}

static double bounceOut(double k) {
    if((k /= 1) < (1 / 2.75)) {
        return 7.5625 * k * k;
    } else if(k < (2 / 2.75)) {
        return 7.5625 * (k -= (1.5 / 2.75)) * k + 0.75;
    } else if(k < (2.5 / 2.75)) {
        return 7.5625 * (k -= (2.25 / 2.75)) * k + 0.9375;
    }

    return 7.5625 * (k -= (2.625 / 2.75)) * k + 0.984375;
}

static double bounceIn(double k) { return 1 - bounceOut(1 - k); }

static double bounceInOut(double k) {
    if(k < 0.5) {
        return bounceIn(k * 2) * 0.5;
    }
    return bounceOut(k * 2 - 1) * 0.5 + 0.5;
}

static double DoErasing(std::string type, double k) {
    if(type == "linear") {
        return linear(k);
    } else if(type == "quadraticIn") {
        return quadraticIn(k);
    } else if(type == "quadraticOut") {
        return quadraticOut(k);
    } else if(type == "quadraticInOut") {
        return quadraticInOut(k);
    } else if(type == "cubicIn") {
        return cubicIn(k);
    } else if(type == "cubicOut") {
        return cubicOut(k);
    } else if(type == "elasticIn") {
        return elasticIn(k);
    } else if(type == "elasticOut") {
        return elasticOut(k);
    } else if(type == "elasticInOut") {
        return elasticInOut(k);
    } else if(type == "backIn") {
        return backIn(k);
    } else if(type == "backOut") {
        return backOut(k);
    } else if(type == "backInOut") {
        return backInOut(k);
    } else if(type == "bounceOut") {
        return bounceOut(k);
    } else if(type == "bounceIn") {
        return bounceIn(k);
    } else if(type == "bounceInOut") {
        return bounceInOut(k);
    }
    return linear(k);
}
} // namespace erasing

// template <typename Function> class Animator {
//   public:
//     Animator(shape::Shape *shape, TimeLine *timeLine) {}

//     Animator &to();

//     Animator &onFrame(Function f) { return *this; }

//     Animator &onEnd(Function f) { return *this; }

//   private:
//   public:
//     bool hasStarted = false;
//     bool hasEnded = false;
//     shape::Shape *shape = nullptr;
//     animate::TimeLine *timeLine = nullptr;
// };
} // namespace animate
} // namespace xg

#endif // XG_GRAPHICS_ANIMATE_ANIMATOR_H
