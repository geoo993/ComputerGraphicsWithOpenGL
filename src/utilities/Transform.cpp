
#include "Transform.h"

CTransform::CTransform(): m_model(1){}
CTransform::CTransform(const glm::mat4 &initialMatrix): m_model(initialMatrix) {}
CTransform::~CTransform(){}

CTransform::CTransform(const CTransform &other){
    this -> m_model = other.m_model;
}

CTransform &CTransform::operator=(const CTransform &other){
    this -> m_model = other.m_model;
    return *this;
}

CTransform &CTransform::operator*=(const glm::mat4 &theMatrix) {
    ApplyMatrix(theMatrix);
    return *this;
}

glm::mat4 CTransform::GetModel() const
{
    return m_model;
}

void CTransform::Rotate( const glm::vec3 axis, float angDegCCW )
{
    m_model = glm::rotate(m_model, angDegCCW, axis);
}

void CTransform::RotateX( float angDegCCW )
{
    Rotate(glm::vec3(1.0f, 0.0f, 0.0f), angDegCCW);
}

void CTransform::RotateY( float angDegCCW )
{
    Rotate(glm::vec3(0.0f, 1.0f, 0.0f), angDegCCW);
}

void CTransform::RotateZ( float angDegCCW )
{
    Rotate(glm::vec3(0.0f, 0.0f, 1.0f), angDegCCW);
}

void CTransform::RotateRadians( const glm::vec3 axisOfRotation, float angRadCCW )
{
    float fCos = cosf(angRadCCW);
    float fInvCos = 1.0f - fCos;
    float fSin = sinf(angRadCCW);
    float fInvSin = 1.0f - fSin;

    glm::vec3 axis = glm::normalize(axisOfRotation);

    glm::mat4 theMat(1.0f);
    theMat[0].x = (axis.x * axis.x) + ((1 - axis.x * axis.x) * fCos);
    theMat[1].x = axis.x * axis.y * (fInvCos) - (axis.z * fSin);
    theMat[2].x = axis.x * axis.z * (fInvCos) + (axis.y * fSin);

    theMat[0].y = axis.x * axis.y * (fInvCos) + (axis.z * fSin);
    theMat[1].y = (axis.y * axis.y) + ((1 - axis.y * axis.y) * fCos);
    theMat[2].y = axis.y * axis.z * (fInvCos) - (axis.x * fSin);

    theMat[0].z = axis.x * axis.z * (fInvCos) - (axis.y * fSin);
    theMat[1].z = axis.y * axis.z * (fInvCos) + (axis.x * fSin);
    theMat[2].z = (axis.z * axis.z) + ((1 - axis.z * axis.z) * fCos);
    m_model *= theMat;
}

void CTransform::Scale( const glm::vec3 &scaleVec )
{
    m_model = glm::scale(m_model, scaleVec);
}

void CTransform::Scale(float scaleX, float scaleY, float scaleZ) {
    Scale(glm::vec3(scaleX, scaleY, scaleZ));
}

void CTransform::Scale(float uniformScale) {
    Scale(glm::vec3(uniformScale));
}

void CTransform::Translate( const glm::vec3 &offsetVec )
{
    m_model = glm::translate(m_model, offsetVec);
}

void CTransform::Translate(float transX, float transY, float transZ) {
    Translate(glm::vec3(transX, transY, transZ));
}

void CTransform::ApplyMatrix( const glm::mat4 &theMatrix )
{
    m_model *= theMatrix;
}

void CTransform::SetMatrix( const glm::mat4 &theMatrix )
{
    m_model = theMatrix;
}

void CTransform::SetIdentity()
{
    m_model = glm::mat4(1.0f);
}

void CTransform::LookAt( const glm::vec3 &myPosition, const glm::vec3 &lookatPosition, const glm::vec3 &upDir )
{
    m_model *= glm::lookAt(myPosition, lookatPosition, upDir);
}
