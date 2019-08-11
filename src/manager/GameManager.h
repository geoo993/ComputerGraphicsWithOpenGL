//
//  GameManager.h
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 07/10/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#ifndef GameManager_h
#define GameManager_h

#include "../ManagerBase.h"

class CGameManager
{
    
public:
    CGameManager();
    virtual ~CGameManager();
    
    void SetResourcePath(const std::string &filepath);
    std::string GetResourcePath() const;
    
    //void SetResourceType(RESOURCE_TYPE type) { resourceType = type; }
    //RESOURCE_TYPE GetResourceType() const { return resourceType; }
 
    void SetLoaded(const bool &value);
    bool IsLoaded() const;
    
    void SetActive(const bool &value);
    bool IsActive() const;
    
private:
    
//protected:
    std::string resourcePath;
    //RESOURCE_TYPE resourceType;
    bool isLoaded;
    bool isActive;
    
};

#endif /* GameManager_h */
