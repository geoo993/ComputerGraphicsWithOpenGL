//
//  ListBox.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 25/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef ListBox_h
#define ListBox_h

#include "Control.h"

class CListBox: public CControl {
    
public:
    CListBox(GUIBoxData *data, GLint itemHeight,
             const GUIMode &mode = GUIMode::STATIC, const GLboolean & create = true,
             const PostProcessingEffectMode &ppfxMode = PostProcessingEffectMode::PBR);
    
    virtual void Create();
    GLboolean Update(const MouseState &state) override;
    virtual void Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material,
    const glm::vec4 &textColor, const glm::vec4 &textHighlightedColor,
    const glm::vec4 &boxColor, const glm::vec4 &backgroundColor);
    virtual GUIType GetGUIType();
    
    void AddItem(const std::string &item);
    void RemoveItem(const GLint &index);
    void SetCurrent(const GLint &index);
    
    void SetValue(GLuint *value, GLboolean *valueChanged);
    
    GLuint GetIndex();
    GLuint GetCount();
    
    void Clear() override;
    void Release() override;
protected:
    GLuint                      m_labelSize;
    GLuint                      m_itemHeight;
    GLuint                      m_index;
    GLuint                      m_lastIndex;
    GLuint                      *m_currentIndex;
    GLboolean                   *m_currentIndexChanged;
    std::vector<std::string>    m_items;
};

#endif /* ListBox_h */
