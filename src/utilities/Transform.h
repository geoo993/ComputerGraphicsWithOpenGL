#pragma once

#include "../UtilitiesBase.h"

// http://www.songho.ca/opengl/gl_anglestoaxes.html
// https://gamedev.stackexchange.com/questions/93771/translating-an-object-in-the-rotation-direction

class CTransform {

public:
    
    /*
     The order or format of speciifying the model matrix is to do:
     
         1 - Translation
         2 - Rotation
         3 - Scale
     
     */

    ///Initializes the matrix stack with the identity matrix.
    CTransform();
    ~CTransform();
    
    ///Initializes the matrix stack with the given matrix.
    explicit CTransform(const glm::mat4 &initialMatrix);
    
    glm::mat4 GetModel() const;
    
    ///Applies a rotation matrix about the given axis, with the given angle in degrees.
    void Rotate( const glm::vec3 axis, float angDegCCW );
    
    //Applies a rotation matrix about the +X axis, with the given angle in degrees.
    void RotateX( float angDegCCW );
    
    ///Applies a rotation matrix about the +Y axis, with the given angle in degrees.
    void RotateY( float angDegCCW );
    
    ///Applies a rotation matrix about the +Z axis, with the given angle in degrees.
    void RotateZ( float angDegCCW );
    
    ///Applies a rotation matrix about the given axis, with the given angle in radians.
    void RotateRadians( const glm::vec3 axisOfRotation, float angRadCCW );
    
    ///Applies a scale matrix, with the given glm::vec3 as the axis scales.
    void Scale( const glm::vec3 &scaleVec );
    
    ///Applies a scale matrix, with the given values as the axis scales.
    void Scale(float scaleX, float scaleY, float scaleZ);
    
    ///Applies a uniform scale matrix.
    void Scale(float uniformScale);
    
    ///Applies a translation matrix, with the given glm::vec3 as the offset.
    void Translate( const glm::vec3 &offsetVec );

    ///Applies a translation matrix, with the given X, Y and Z values as the offset.
    void Translate(float transX, float transY, float transZ);

    void ApplyMatrix( const glm::mat4 &theMatrix );
    
    ///The given matrix becomes the current matrix.
    void SetMatrix( const glm::mat4 &theMatrix );
    
    ///Sets the current matrix to the identity matrix.
    void SetIdentity();
    
    void LookAt( const glm::vec3 &myPosition, const glm::vec3 &lookatPosition, const glm::vec3 &upDir );

private:
    
    CTransform(const CTransform &other);
    CTransform &operator=(const CTransform &other);
    CTransform &operator*=(const glm::mat4 &theMatrix);
    
    glm::mat4 m_model;

};

