//
//  Control.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 22/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Control.h"

std::list<CControl *> CControl::m_controls;

CControl::CControl(GLint positionX, GLint positionY, GLint width, GLint height) {
    m_controls.push_back(this);
    m_posX = positionX;
    m_posY = positionY;
    m_width = width;
    m_height = height;
    m_vao, m_numTriangles = 0;
}

CControl::~CControl() {
    Release();
}
/*
void CControl::Create() {
    
}

void CControl::Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material) {
    
}
*/
GLboolean CControl::Update(const MouseState &state){
    
    // is the mouse inside the control
    GLint x = (GLint)state.x;
    GLint y = (GLint)state.y;
    
    m_isInside = false;
    
    if (x >= m_posX && x <= m_posX + m_width &&
        y >= m_posY && y <= m_posY + m_height) {
        m_isInside = true;
    }
    
    return false;
}

//std::string CControl::GetControlType() {
//    return "Control";
//}
void CControl::SetPosition(GLint x, GLint y) {
    m_posX = x;
    m_posY = y;
}

void CControl::SetSize(GLint width, GLint height){
    m_width = width;
    m_height = height;
}

GLint CControl::GetWidth() const {
    return m_width;
}

GLint CControl::GetHeight() const {
    return m_height;
}

GLboolean CControl::GetIsInside() const {
    return m_isInside;
}

void CControl::Release() {
    this->m_controls.remove(this);
    glDeleteVertexArrays(1, &m_vao);
    m_vbo.Release();
}

CControl * AddControl(CControl *control) {
    /*
    static GLint lastX = 5;
    static GLint lastY = 5;
    
    control->SetPosition(lastX, lastY);
    
    lastY += control->GetHeight() + 5;
    */
    return control;
}
