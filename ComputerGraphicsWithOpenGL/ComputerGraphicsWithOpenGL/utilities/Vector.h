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


//https://cboard.cprogramming.com/game-programming/71050-can-i-have-some-sample-code-opengl-gldrawpixels-;.html


#ifndef Vector_h
#define Vector_h

#include "Common.h"

class Vector {
public:
    // Constructors
    Vector();
    Vector(float x, float y, float z);
    
    // Methods
    float x() const;
    float y() const;
    float z() const;
    float operator[](int index) const;
    const float* array() const;
        
    bool operator == (const Vector &v) const;
    bool operator != (const Vector &v) const;
    Vector operator + (const Vector &v) const;
    Vector operator - () const;
    Vector operator - (const Vector &v) const;
    Vector operator * (float s) const;
    Vector operator / (float s) const;
    
    float magnitude() const;
    Vector normalize() const;
    float dot(const Vector &v) const;
    Vector cross(const Vector &v) const;
    
private:
    // Fields
    float X, Y, Z;
};

Vector operator * (float s, const Vector &v);
std::ostream& operator << (std::ostream &os, Vector v);

#endif
