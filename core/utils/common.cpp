//
//  common.cpp
//  Pods
//
//  Created by weiqing.twq on 2022/6/1.
//

#include "common.h"

bool xg::IsZero(double val) noexcept { return (fabs(val) < XG_EPS); }
bool xg::IsEqualDeviation (double v1, double v2, double deviation) noexcept { return fabs(v1 - v2) < deviation; }
bool xg::IsEqual(double v1, double v2) noexcept { return IsEqualDeviation(v1, v2, XG_EPS); }
