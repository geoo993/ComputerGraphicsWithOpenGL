//
//  Slider.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 25/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef Slider_h
#define Slider_h

#include "Control.h"

class CSlider: public CControl {
    
public:
    CSlider(std::string label, GLuint labelSize,
            GLfloat min, GLfloat max, GLuint tickSize,
            GLint positionX, GLint positionY, GLint width, GLint height);
    
    virtual void Create();
    GLboolean Update(const MouseState &state) override;
    virtual void Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material);
    virtual std::string GetControlType();
    
    void SetValue(GLfloat *value);
    
    void Release() override;
protected:
    GLboolean           m_dragging;
    std::string         m_label;
    GLuint              m_labelSize;
    GLuint              m_tickSize;
    GLfloat             m_defaultValue;
    GLfloat             m_min;
    GLfloat             m_max;
    GLfloat             *m_current;
};


#endif /* Slider_h */
