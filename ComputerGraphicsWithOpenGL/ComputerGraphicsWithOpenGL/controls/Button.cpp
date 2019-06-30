//
//  Button.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 22/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Button.h"


CButton::CButton(std::string label, GUIBoxData *data,
                 const GUIMode &mode, const GLboolean & create, const PostProcessingEffectMode &ppfxMode):
CControl(data, mode, ppfxMode) {
    m_down = false;
    m_label = label;
    m_labelSize = data->textSize;
    m_defaultIsEnabled = false;
    m_isEnabled = nullptr;
    m_isActive = false;
    m_uuid = "Button::"+label;
    
    if (create) Create();
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
    
    m_isActive = true;
}

GLboolean CButton::Update(const MouseState &state) {
    CControl::Update(state);
    
    // is the mouse inside the button
    if (m_isInside) {
        if (state.m_leftButtonDown == ControlType::LEFTMOUSE) {
            m_down = true;
        } else if ( m_down ) { // if the button was down and is no longer down
            if (m_isEnabled != nullptr) {
                *m_isEnabled = !(*m_isEnabled);
            }
            m_down = false;
            return true;
        }
    }
    return false;
}

void CButton::Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material,
                    const glm::vec4 &textColor, const glm::vec4 &textHighlightedColor,
                    const glm::vec4 &buttonColor, const glm::vec4 &backgroundColor) {
  
    if (m_isActive && m_isEnabled != nullptr) {
        hudProgram->UseProgram();
        hudProgram->SetUniform("bUseScreenQuad", true);
        hudProgram->SetUniform("material.bUseTexture", false);
        hudProgram->SetUniform(material+".color", backgroundColor);
        
        glBindVertexArray(m_vao);
        // Draw the triangle !
        // https://www.youtube.com/watch?v=4qECwne-CD8
        // https://stackoverflow.com/questions/39430404/drawing-pixels-in-opengl
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_numTriangles);
        glBindVertexArray(0);
        
        if (*m_isEnabled == true)
        {
            GLuint VertexArrayID;
            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);
            
            GLfloat xP = m_posX;
            GLfloat yP = m_posY;
            
            GLfloat rectWidth = m_width - 4.0f;
            GLfloat rectHeight = m_height - 4.0f;
            
            GLfloat left = ((2.0f + xP) / ((GLfloat)SCREEN_WIDTH / 2.0f)) - 1.0f;
            GLfloat right = ((2.0f + xP + rectWidth) / ((GLfloat)SCREEN_WIDTH / 2.0f)) - 1.0f;
            GLfloat top = ((-2.0f - yP) / ((GLfloat)SCREEN_HEIGHT / 2.0f)) + 1.0f;
            GLfloat bottom = ((-2.0f - yP - rectHeight) / ((GLfloat)SCREEN_HEIGHT / 2.0f)) + 1.0f;
            std::vector<glm::vec2> g_quad_vertex_buffer_data = {
                glm::vec2(right, top),                  // top right
                glm::vec2(left, top),                   // top left
                glm::vec2(left, bottom),                // bottom left
                glm::vec2(left, bottom),                // bottom left
                glm::vec2(right, top),                  // top right
                glm::vec2(right, bottom),               // bottom right
            };
            
            // This will identify our vertex buffer
            GLuint vertexbuffer;
            // Generate 1 buffer, put the resulting identifier in vertexbuffer
            glGenBuffers(1, &vertexbuffer);
            // The following commands will talk about our 'vertexbuffer' buffer
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            // Give our vertices to OpenGL.
            //glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, g_quad_vertex_buffer_data.size() * sizeof(g_quad_vertex_buffer_data[0]), &g_quad_vertex_buffer_data[0], GL_STATIC_DRAW);
            
            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(
                                  0,                            // attribute 0. No particular reason for 0, but must match the layout in the shader.
                                  2,                            // size
                                  GL_FLOAT,                     // type
                                  GL_FALSE,                     // normalized?
                                  sizeof(glm::vec2),            // stride
                                  (void*)0                      // array buffer offset
                                  );
            hudProgram->SetUniform(material+".color", buttonColor);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, g_quad_vertex_buffer_data.size());
            glBindVertexArray(0);
        }
        
        // Highlight button text
        if (m_isInside) {
            hudProgram->SetUniform(material+".color", textHighlightedColor);
        } else {
            hudProgram->SetUniform(material+".color", textColor);
        }
        
        // Draw text inside button
        GLuint textWidth = (strlen(m_label.data()) * m_labelSize) / 2;
        GLuint textHeight = m_labelSize / 2;
        int textX = m_posX + (m_width - textWidth) / 2;
        int textY = m_posY + (m_height + textHeight) / 2;
        
        hudProgram->SetUniform("bUseScreenQuad", false);
        hudProgram->SetUniform("material.bUseTexture", true);
        font->Render(hudProgram, textX, (GLfloat)SCREEN_HEIGHT - textY, m_labelSize, "%s", m_label.data());
        
    }
}

GUIType CButton::GetGUIType() {
    return GUIType::BUTTON;
}

void CButton::SetValue(GLboolean *value) {
    m_isEnabled = value;
    
    if (m_isEnabled != nullptr) {
        m_defaultIsEnabled = *m_isEnabled;
    }
}

void CButton::Clear() {
    CControl::Clear();
}

void CButton::Release() {
    CControl::Release();
    delete m_isEnabled;
    m_isEnabled = nullptr;
    m_down = false;
    m_defaultIsEnabled = false;
}
