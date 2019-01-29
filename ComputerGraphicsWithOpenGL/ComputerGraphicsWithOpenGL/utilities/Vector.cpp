// Copyright Jay Conrod 2010.
// http://jayconrod.com
// jayconrod@gmail.com
//
// You are free to modify and distribute this code as long as this
// copyright notice is maintained.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

#include "Vector.h"

#include <cassert>
#include <cmath>

using namespace std;

Vector::Vector() :
X(0.0),
Y(0.0),
Z(0.0)
{}

Vector::Vector(float x, float y, float z) :
X(x),
Y(y),
Z(z)
{}

float Vector::x() const
{
    return X;
}

float Vector::y() const
{
    return Y;
}

float Vector::z() const
{
    return Z;
}

float Vector::operator [] (int index) const
{
    assert(index >= 0 && index < 3);
    if (index == 0)
        return X;
    else if (index == 1)
        return Y;
    else
        return Z;
}

const float* Vector::array() const {
    return &X;
}

bool Vector::operator == (const Vector &v) const
{
    return X == v.X && Y == v.Y && Z == v.Z;
}

bool Vector::operator != (const Vector &v) const
{
    return !(*this == v);
}

Vector Vector::operator + (const Vector &v) const
{
    return Vector(X + v.X, Y + v.Y, Z + v.Z);
}

Vector Vector::operator - () const
{
    return Vector(-X, -Y, -Z);
}

Vector Vector::operator - (const Vector &v) const
{
    return *this + -v;
}

Vector Vector::operator * (float s) const
{
    return Vector(X * s, Y * s, Z * s);
}

Vector operator * (float s, const Vector &v)
{
    return v * s;
}

Vector Vector::operator / (float s) const
{
    return Vector(X / s, Y / s, Z / s);
}

float Vector::magnitude() const
{
    return sqrt(X * X + Y * Y + Z * Z);
}

Vector Vector::normalize() const
{
    return *this / magnitude();
}

float Vector::dot(const Vector &v) const
{
    return X * v.X + Y * v.Y + Z * v.Z;
}

Vector Vector::cross(const Vector &v) const
{
    return Vector(Y * v.Z - Z * v.Y,
                  Z * v.X - X * v.Z,
                  X * v.Y - Y * v.X);
}

ostream& operator << (ostream &os, Vector v) {
    os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    return os;
}
