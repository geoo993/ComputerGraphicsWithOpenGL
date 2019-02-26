//
//  Button.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 22/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Button.h"


CButton::CButton(std::string label, GLuint labelSize, GLint positionX, GLint positionY, GLint width, GLint height):
CControl(positionX, positionY, width, height) {
    m_down = false;
    m_label = label;
    m_labelSize = labelSize;
    
    Create();
}

void CButton::Create() {
   
    // make and bind the VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    m_vbo.Create();
    m_vbo.Bind();
    
    GLfloat xP = m_posX;
    GLfloat yP = m_posY;
    
    GLfloat rectWidth = m_width;
    GLfloat rectHeight = m_height;
    
    GLfloat left = xP / ((GLfloat)SCREEN_WIDTH / 2.0f) - 1.0f;
    GLfloat right = (xP + rectWidth) / ((GLfloat)SCREEN_WIDTH / 2.0f) - 1.0f;
    GLfloat top = -yP / ((GLfloat)SCREEN_HEIGHT / 2.0f) + 1.0f;
    GLfloat bottom = (-yP - rectHeight) / ((GLfloat)SCREEN_HEIGHT / 2.0f) + 1.0f;
    
    std::vector<glm::vec2> g_quad_vertex_buffer_data = {
        glm::vec2(right, top),                  // top right
        glm::vec2(left, top),                   // top left
        glm::vec2(left, bottom),                // bottom left
        glm::vec2(left, bottom),                // bottom left
        glm::vec2(right, top),                  // top right
        glm::vec2(right, bottom),               // bottom right
    };
    m_numTriangles = g_quad_vertex_buffer_data.size();
    
    for (int i = 0; i < g_quad_vertex_buffer_data.size(); i++) {
        m_vbo.AddData(&g_quad_vertex_buffer_data[i], sizeof(glm::vec2));
    }
    
    m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
    
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
                          0,                            // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                            // size
                          GL_FLOAT,                     // type
                          GL_FALSE,                     // normalized?
                          sizeof(glm::vec2),            // stride
                          (void*)0                      // array buffer offset
                          );
    
    glBindVertexArray(0);
    
}

GLboolean CButton::Update(const MouseState &state) {
    CControl::Update(state);
    
    // is the mouse inside the button
    if (m_isInside) {
        if (state.m_leftButtonDown == ControlType::LEFTMOUSE) {
            m_down = true;
        } else if ( m_down ) { // if the button was down and is no longer down
            m_down = false;
            
            return true;
        }
    }
    
    return false;
}

void CButton::Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material) {
  
    hudProgram->UseProgram();
    hudProgram->SetUniform("bUseScreenQuad", true);
    hudProgram->SetUniform("bUseTexture", false);
    hudProgram->SetUniform(material+".color", glm::vec3(0.2f, 0.9f, 0.6f));
    
    glBindVertexArray(m_vao);
    // Draw the triangle !
    // https://www.youtube.com/watch?v=4qECwne-CD8
    // https://stackoverflow.com/questions/39430404/drawing-pixels-in-opengl
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_numTriangles);
    glBindVertexArray(0);
    
    // Highlight button
    if (m_isInside) {
        hudProgram->SetUniform(material+".color", glm::vec3(0.1f, 0.2f, 0.9f));
    } else {
        hudProgram->SetUniform(material+".color", glm::vec3(0.9f, 0.2f, 0.2f));
    }
    
    // Draw text inside button
    GLuint textWidth = (strlen(m_label.data()) * m_labelSize) / 2;
    GLuint textHeight = m_labelSize / 2;
    int textX = m_posX + (m_width - textWidth) / 2;
    int textY = m_posY + (m_height + textHeight) / 2;
    
    hudProgram->SetUniform("bUseScreenQuad", false);
    hudProgram->SetUniform("bUseTexture", true);
    font->Render(hudProgram, textX + 1, (GLfloat)SCREEN_HEIGHT - textY + 1, m_labelSize, "%s", m_label.data());
    
}

std::string CButton::GetControlType() {
    return "Button::"+m_label;
}

void CButton::Release() {
    CControl::Release();
}
