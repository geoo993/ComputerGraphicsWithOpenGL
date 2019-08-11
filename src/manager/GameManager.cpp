//
//  GameManager.cpp
//  New_OpenGL_CourseWork_Template
//
//  Created by GEORGE QUENTIN on 07/10/2017.
//  Copyright © 2017 LEXI LABS. All rights reserved.
//

#include "GameManager.h"

CGameManager::CGameManager() {
    this->resourcePath = "";
    this->isLoaded = false;
    this->isActive = false;
}

CGameManager::~CGameManager() {
    
}

void CGameManager::SetResourcePath(const std::string &filepath) {
    resourcePath = filepath;
}

std::string CGameManager::GetResourcePath() const {
    return resourcePath;
}

//void SetResourceType(RESOURCE_TYPE type) { resourceType = type; }
//RESOURCE_TYPE GetResourceType() const { return resourceType; }

void CGameManager::SetLoaded(const bool &value) {
    isLoaded = value;
}

bool CGameManager::IsLoaded() const {
    return isLoaded;
}

void CGameManager::SetActive(const bool &value) {
    isActive = value;
}

bool CGameManager::IsActive() const {
    return isActive;
}
