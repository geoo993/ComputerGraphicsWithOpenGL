//
//  Lighting.hpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 24/07/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef Lighting_h
#define Lighting_h

#include "../LightingBase.h"

class BaseLight
{

public:
    glm::vec3 color;
    float intensity;
    
    BaseLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), GLfloat intensity = 0.0f) :
    color(color), intensity(intensity) {}

    virtual ~BaseLight();

private:
    BaseLight(BaseLight& other) {}
    void operator=(BaseLight& other) {}

};

struct DirectionalLight : public BaseLight
{
    DirectionalLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), GLfloat intensity = 0.0f);
};

struct Attenuation
{
    float constant;
    float linear;
    float exponent;

    Attenuation(GLfloat constant = 0.0f, GLfloat linear = 0.0f, GLfloat exponent = 1.0f) :
    constant(constant),
    linear(linear),
    exponent(exponent) {}
};

struct PointLight : public BaseLight
{
    Attenuation attenuation;
    glm::vec3 position;
    float range;

    PointLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), GLfloat intensity = 0.0f,
               const Attenuation& attenuation = Attenuation(),
               const glm::vec3 & position = glm::vec3(0.0f,0.0f,0.0f));
};

struct SpotLight : public PointLight
{
    float cutOff;         // between 0 - 1
    float outerCutOff;    // between 0 - 1
    SpotLight(const glm::vec3 & color = glm::vec3(0.0f,0.0f,0.0f), GLfloat intensity = 0.0f,
              const Attenuation& attenuation = Attenuation(),
              const glm::vec3 & position = glm::vec3(0.0f,0.0f,0.0f),
              GLfloat cutOff = 0.0f, GLfloat outerCutOff = 0.0f);

};


#endif /* Lighting_h */
