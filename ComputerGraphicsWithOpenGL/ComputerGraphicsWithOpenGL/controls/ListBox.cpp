//
//  ListBox.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 25/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "ListBox.h"

CListBox::CListBox(GLuint labelSize, GLint itemHeight,
                   GLint positionX, GLint positionY,
                   GLint width, GLint height):
CControl(positionX, positionY, width, height) {
    m_labelSize = labelSize;
    m_itemHeight = itemHeight;
    m_index = 0;
    m_items = {};
    Create();
}

void CListBox::Create() {
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

GLboolean CListBox::Update(const MouseState &state){
    CControl::Update(state);
    
    int x = state.x;
    int y = state.y;
    
    // is the mouse inside the listbox
    if (m_isInside && state.m_leftButtonDown == ControlType::LEFTMOUSE) {
        int tempIndex = (y - m_posY) / m_itemHeight; //itemHeight is the height of each row
        
        if (tempIndex >= 0 && tempIndex < (int)m_items.size()) {
            m_index = tempIndex;
            
            // if we selected an item, then return true;
            return true;
        }
    }
    
    return false;
}

void CListBox::Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material) {
    
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
    
    if (m_index >= 0) {
        
        int currentY = m_posY + m_index * m_itemHeight;
        
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        
        GLfloat xP = m_posX;
        GLfloat yP = currentY;
        
        GLfloat rectWidth = m_width;
        GLfloat rectHeight = m_itemHeight;
        
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
        hudProgram->SetUniform(material+".color", glm::vec3(0.6f, 0.2f, 0.9f));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, g_quad_vertex_buffer_data.size());
        glBindVertexArray(0);
    }
 
    for (int i = 0; i < (int)m_items.size(); i++) {
        // Highlight current button
        if (m_index == i) {
            hudProgram->SetUniform(material+".color", glm::vec3(0.1f, 0.2f, 0.9f));
        } else {
            hudProgram->SetUniform(material+".color", glm::vec3(0.9f, 0.2f, 0.2f));
        }
        
        // Draw text inside button
        GLuint textWidth = (strlen(m_items[i].data()) * m_labelSize) / 2;
        GLuint textHeight = m_labelSize / 2;
        int textX = m_posX + ((m_width + 1) - textWidth) / 2;
        int textY = (m_posY + i * m_itemHeight) + (m_itemHeight + textHeight) / 2;
        
        hudProgram->SetUniform("bUseScreenQuad", false);
        hudProgram->SetUniform("bUseTexture", true);
        font->Render(hudProgram, textX + 2, ((GLfloat)SCREEN_HEIGHT - textY + 2), m_labelSize, "%s", m_items[i].data());
    }
}

std::string CListBox::GetControlType() {
    return "ListBox";
}

void CListBox::AddItem(const std::string &item) {
    m_items.push_back(item);
}

void CListBox::RemoveItem(const GLint &index) {
    int i = 0;
    for (std::vector<string>::iterator it = m_items.begin(); it != m_items.end(); it++) {
        if (i == index) {
            m_items.erase(it);
            break;
        }
        i++;
    }
    
    if (m_index >= (int)m_items.size()) {
        m_index = (int)m_items.size() - 1;
    }
}

void CListBox::SetCurrent(const GLint &index) {
    m_index = index;
}

GLuint CListBox::GetIndex() {
    return m_index;
}

GLuint CListBox::GetCount() {
    return m_items.size();
}

void CListBox::Release() {
    CControl::Release();
    m_items.clear();
}
