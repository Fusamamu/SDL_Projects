#pragma once
#include <limits>
#include <algorithm>
#include "Point.h"

static constexpr float inf = std::numeric_limits<float>::infinity();

struct BBox
{
    Point Min {inf, inf};
    Point Max { -inf, -inf};

    BBox & operator |= (const Point& _point)
    {
        Min.X = std::min(Min.X, _point.X);
        Min.Y = std::min(Min.Y, _point.Y);
        Max.X = std::max(Max.X, _point.X);
        Max.Y = std::max(Max.Y, _point.Y);

        return *this;
    }
};

template<typename Iterator>
BBox ComputeBBox(Iterator _begin, Iterator _end)
{
    BBox _box;
    for(auto _it = _begin; _it != _end; ++_it)
        _box != *_it;

    return _box;
}
