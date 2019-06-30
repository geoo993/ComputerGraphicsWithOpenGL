//
//  Slider.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 25/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Slider.h"

CSlider::CSlider(std::string label, GLfloat min, GLfloat max, GLuint tickSize,
                 GUIBoxData *data, const GUIMode &mode, const GLboolean & create, const PostProcessingEffectMode &ppfxMode) :
CControl(data, mode, ppfxMode) {
    m_label = label;
    m_labelSize = data->textSize;
    
    m_dragging = false;
    m_defaultValue = 0.0f;
    m_min = min;
    m_max = max;
    m_current = nullptr;
    m_tickSize = tickSize;
    m_isActive = false;
    m_uuid = "Slider::"+label;
    
    if (create) Create();
}

void CSlider::Create() {
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

GLboolean CSlider::Update(const MouseState &state) {
    CControl::Update(state);
    
    GLint x = state.x;
    GLint y = state.y;
    
    if (m_isInside) {
        if (state.m_leftButtonDown == ControlType::LEFTMOUSE) {
            // if left button down then start dragging.
            m_dragging = true;
        }
        
        if (state.m_rightButtonDown == ControlType::RIGHTMOUSE) {
            *m_current = m_defaultValue;
        }
    }
    
    if (state.m_leftButtonDown != ControlType::LEFTMOUSE) {
        m_dragging = false;
    }
    
    if (m_dragging) {
        *m_current = (GLfloat)(x - m_posX) / (GLfloat)m_width * (m_max - m_min) + m_min;
        
        if (*m_current > m_max) {
            *m_current = m_max;
        } else if( *m_current < m_min) {
            *m_current = m_min;
        }
    }
    
    return m_dragging;
}

void CSlider::Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material,
                    const glm::vec4 &textColor, const glm::vec4 &textHighlightedColor,
                    const glm::vec4 &sliderColor, const glm::vec4 &backgroundColor) {
    if (m_isActive && m_current != nullptr) {
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
        
        // Slider bar
        {
            GLfloat currentX = (*m_current - m_min) / (m_max - m_min) * (GLfloat)(m_width - m_tickSize) + (GLfloat)m_posX;
            GLuint VertexArrayID;
            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);
            
            GLfloat xP = m_posX; //(GLfloat)currentX;
            GLfloat yP = m_posY;
            
            GLfloat rectWidth = currentX + (m_tickSize / 2.0f) - m_posX; // m_tickSize
            GLfloat rectHeight = m_height - 4.0f;
            
            GLfloat left = xP / ((GLfloat)SCREEN_WIDTH / 2.0f) - 1.0f;
            GLfloat right = (xP + rectWidth) / ((GLfloat)SCREEN_WIDTH / 2.0f) - 1.0f;
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
            hudProgram->SetUniform(material+".color", sliderColor);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, g_quad_vertex_buffer_data.size());
            glBindVertexArray(0);
        }
        
        // Highlight button
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

GUIType CSlider::GetGUIType() {
    return GUIType::SLIDER;
}

void CSlider::SetValue(GLfloat *value) {
    m_current = value;
    
    if (m_current != nullptr) {
        m_defaultValue = *m_current;
    }
}

void CSlider::Clear() {
    CControl::Clear();
}


void CSlider::Release() {
    CControl::Release();
    delete m_current;
    m_current = nullptr;
    
    m_dragging = false;
    m_defaultValue = 0.0f;
}
