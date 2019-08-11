//
//  Control.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 22/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Control.h"

std::list<CControl *> CControl::m_controls;

CControl::CControl(GUIBoxData *data, const GUIMode &mode, const PostProcessingEffectMode &ppfxMode) {
    m_controls.push_back(this);
    m_posX = data->x;
    m_posY = data->y;
    m_width = data->width;
    m_height = data->height;
    m_vao, m_numTriangles = 0;
    m_uuid = "Control::";
    m_mode = mode;
    m_ppfx = ppfxMode;
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

void CControl::SetPosition(GLint x, GLint y) {
    m_posX = x;
    m_posY = y;
}

void CControl::SetSize(GLint width, GLint height){
    m_width = width;
    m_height = height;
}

void CControl::SetIsInside(const GLboolean &inside) {
    m_isInside = inside;
}


void CControl::SetIsActive(const GLboolean &active) {
    m_isActive = active;
}

GLint CControl::GetWidth() const {
    return m_width;
}

GLint CControl::GetHeight() const {
    return m_height;
}

GUIType CControl::GetGUIType() {
    return GUIType::UNKNOWN;
}

GLboolean CControl::GetIsInside() const {
    return m_isInside;
}

GLboolean CControl::GetIsActive() const {
    return m_isActive;
}

GUIMode CControl::GetGUIMode() const {
    return m_mode;
}

PostProcessingEffectMode CControl::GetPostProcessingEffectMode() const {
    return m_ppfx;
}

void CControl::Clear() {
    //glDeleteVertexArrays(1, &m_vao);
    m_vbo.Release();
    m_isActive = false;
}

void CControl::Release() {
    this->m_controls.remove(this);
    glDeleteVertexArrays(1, &m_vao);
    m_vbo.Release();
    m_isActive = false;
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
