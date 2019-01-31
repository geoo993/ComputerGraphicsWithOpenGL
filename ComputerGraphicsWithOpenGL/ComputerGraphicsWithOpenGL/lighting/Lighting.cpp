//
//  Lighting.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 24/07/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "Lighting.h"

#define COLOR_DEPTH 256

BaseLight::~BaseLight()
{

}

DirectionalLight::DirectionalLight(const glm::vec3 & color, float intensity, float power) :
BaseLight(color, intensity, power)
{
    //SetShader(new Shader("forward-directional"));
}

PointLight::PointLight(const glm::vec3 & color, float intensity, float power, const Attenuation& atten, const glm::vec3 & position) :
BaseLight(color, intensity, power),
atten(atten), position(position), range(0)
{
    std::vector<GLfloat> colorVec = {
        color.x,
        color.y,
        color.z
    };
    GLfloat colorMax = *max_element(colorVec.begin(), colorVec.end());

    GLfloat a = atten.exponent;
    GLfloat b = atten.linear;
    GLfloat c = atten.constant - COLOR_DEPTH * intensity * colorMax;

    this->range = GLfloat(-b + sqrtf(b * b - 4.0f * a * c)) / ( 2.0f * a);

    //SetShader(new Shader("forward-point"));
}

SpotLight::SpotLight(const glm::vec3 & color, float intensity, float power, const Attenuation& atten,
                     const glm::vec3 & position,
                     float cutOff, float outerCutOff) :
PointLight(color, intensity, power, atten, position),
cutOff(cutOff),
outerCutOff(outerCutOff)
{
    //SetShader(new Shader("forward-spot"));
}

