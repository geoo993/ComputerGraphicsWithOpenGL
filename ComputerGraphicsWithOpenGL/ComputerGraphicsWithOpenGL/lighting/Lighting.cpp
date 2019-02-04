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

DirectionalLight::DirectionalLight(const glm::vec3 & color, GLfloat intensity) :
BaseLight(color, intensity)
{
    
}

PointLight::PointLight(const glm::vec3 & color, GLfloat intensity, const Attenuation& attenuation, const glm::vec3 & position) :
BaseLight(color, intensity),
attenuation(attenuation), position(position), range(0)
{
    std::vector<GLfloat> colorVec = {
        color.x,
        color.y,
        color.z
    };
    GLfloat colorMax = *max_element(colorVec.begin(), colorVec.end());

    GLfloat a = attenuation.exponent;
    GLfloat b = attenuation.linear;
    GLfloat c = attenuation.constant - COLOR_DEPTH * intensity * colorMax;

    this->range = GLfloat(-b + sqrtf(b * b - 4.0f * a * c)) / ( 2.0f * a);
}

SpotLight::SpotLight(const glm::vec3 & color, GLfloat intensity, const Attenuation& attenuation,
                     const glm::vec3 & position,
                     GLfloat cutOff, GLfloat outerCutOff) :
PointLight(color, intensity, attenuation, position),
cutOff(cutOff),
outerCutOff(outerCutOff)
{
    
}

