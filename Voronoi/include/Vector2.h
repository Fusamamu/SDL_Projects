#pragma once
#include <iostream>

namespace Voronoi
{
    struct Vector2
    {
        double x;
        double y;

        Vector2(double _x = 0, double _y = 0);

        Vector2 operator-() const;
        Vector2& operator+=(const Vector2& _other);
        Vector2& operator-=(const Vector2& _other);
        Vector2& operator*=(double _scalar);

        Vector2 GetOrthogonal() const;
        double Dot(const Vector2& _other) const;

        double GetNorm() const;
        double GetLength() const;
        double GetDistance(const Vector2& _other) const;
        double GetDet(const Vector2& _other) const;
    };

    Vector2 operator+(Vector2 _lhs, const Vector2& _rhs);
    Vector2 operator-(Vector2 _lhs, const Vector2& _rhs);
    Vector2 operator*(double _scalar, Vector2 _vec);
    Vector2 operator*(Vector2 _vec, double _scalar);
    std::ostream& operator<<(std::ostream& _os, const Vector2& _vec);

}