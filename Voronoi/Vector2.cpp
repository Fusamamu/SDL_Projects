#include "Vector2.h"
#include <cmath>

namespace Voronoi
{
    Vector2::Vector2(double _x, double _y): x(_x), y(_y)
    {

    }


    Vector2 Vector2::operator-() const
    {
        return {-x, -y};
    }

    Vector2& Vector2::operator+=(const Vector2& _other)
    {
        x += _other.x;
        y += _other.y;
        return *this;
    }

    Vector2& Vector2::operator-=(const Vector2& _other)
    {
        x -= _other.x;
        y -= _other.y;
        return *this;
    }

    Vector2& Vector2::operator*=(double _scalar)
    {
        x *= _scalar;
        y *= _scalar;
        return *this;
    }

    Vector2 Vector2::GetOrthogonal() const
    {
        return {-y, x};
    }

    double Vector2::Dot(const Vector2& _other) const
    {
        return x*_other.x + y*_other.y;
    }

    double Vector2::GetNorm() const
    {
        return 1.0;//Wrong
    }

    double Vector2::GetLength() const
    {
        return std::sqrt(x*x + y*y);
    }

    double Vector2::GetDistance(const Voronoi::Vector2 &_other) const
    {
        return (*this - _other).GetLength();
    }

    double Vector2::GetDet(const Voronoi::Vector2 &_other) const
    {
        return x*_other.y - y*_other.x;
    }

    Vector2 operator+(Vector2 _lhs, const Vector2& _rhs)
    {
        _lhs += _rhs;
        return _lhs;
    }

    Vector2 operator-(Vector2 _lhs, const Vector2& _rhs)
    {
        _lhs -= _rhs;
        return _lhs;
    }

    Vector2 operator*(double _scalar, Vector2 _vec)
    {
        _vec*= _scalar;
        return _vec;
    }

    Vector2 operator*(Vector2 _vec, double _scalar)
    {
        return _scalar*_vec;
    }

    std::ostream& operator<<(std::ostream& _os, const Vector2& _vec)
    {
        _os << "(" << _vec.x << ", " << _vec.y << ")";
        return _os;
    }
}