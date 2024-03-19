#pragma once

struct Point
{
    float X;
    float Y;
};

Point Middle(const Point& _p1, const Point& _p2)
{
    return { (_p1.X + _p2.X) / 2.f, (_p1.Y + _p2.Y) / 2.f };
}
