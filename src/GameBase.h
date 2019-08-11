//
//  GameBase.h
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 28/01/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//
#pragma once

#ifndef GameBase_h
#define GameBase_h

#include "manager/GameManager.h"
#include "window/GameWindow.h"
#include "interfaces/IAudio.h"
#include "interfaces/ICamera.h"
#include "interfaces/IControls.h"
#include "interfaces/IGameTimer.h"
#include "interfaces/IGameWindow.h"
#include "interfaces/IHud.h"
#include "interfaces/ILights.h"
#include "interfaces/IMaterials.h"
#include "interfaces/IPostProcessing.h"
#include "interfaces/IRenderer.h"
#include "interfaces/IRenderObject.h"
#include "interfaces/IResources.h"
#include "interfaces/IShaders.h"
#include "interfaces/IShaderUniform.h"
#include "interfaces/ITextures.h"
#include "skybox/Skybox.h"
#include "mesh/Model.h"
#include "controls/Button.h"
#include "controls/ListBox.h"
#include "controls/Slider.h"
#include "objects/Plane.h"
#include "objects/HeightMapTerrain.h"
#include "objects/Cube.h"
#include "objects/Sphere.h"
#include "objects/Torus.h"
#include "objects/TorusKnot.h"
#include "objects/Metaballs.h"
#include "objects/Quad.h"

#endif /* GameBase_h */
