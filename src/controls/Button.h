//
//  Button.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 22/02/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#ifndef Button_h
#define Button_h

#include "Control.h"

class CButton: public CControl {
  
public:
    CButton(std::string label, GUIBoxData *data,
            const GUIMode &mode = GUIMode::STATIC, const GLboolean & create = true,
            const PostProcessingEffectMode &ppfxMode = PostProcessingEffectMode::PBR);
    
    virtual void Create();
    GLboolean Update(const MouseState &state) override;
    virtual void Render(CFreeTypeFont *font, CShaderProgram *hudProgram, const std::string &material,
    const glm::vec4 &textColor, const glm::vec4 &textHighlightedColor,
    const glm::vec4 &buttonColor, const glm::vec4 &backgroundColor);
    virtual GUIType GetGUIType();
    void SetValue(GLboolean *value);
    void Clear() override;
    void Release() override;
protected:
    GLboolean           m_defaultIsEnabled;
    GLboolean           *m_isEnabled;
    GLboolean           m_down;
    std::string         m_label;
    GLuint              m_labelSize;
};

#endif /* Button_h */
