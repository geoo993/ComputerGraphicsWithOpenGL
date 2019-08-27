//
//  Game+IResources.cpp
//  ComputerGraphicsWithOpenGL
//
//  Created by GEORGE QUENTIN on 09/03/2019.
//  Copyright © 2019 GEORGE QUENTIN. All rights reserved.
//

#include "Game.h"

// Initialisation:  This method only runs once at startup
void Game::InitialiseResources()
{
    /// Create objects
    m_gameWindow = new CGameWindow;
    m_gameManager = new CGameManager;
    m_pGameTimer = new CHighResolutionTimer;
    m_pCamera = new CCamera;
    m_pShaderPrograms = new std::vector <CShaderProgram *>;
    m_pFtFont = new CFreeTypeFont;
    m_pAudio = new CAudio;
    m_pQuad = new CQuad;
    m_pSkybox = new CSkybox;
    m_pEnvSkybox = new CSkybox;
    m_pIrrSkybox = new CSkybox;
    m_pPlanarTerrain = new CPlane;
    m_pHeightmapTerrain = new CHeightMapTerrain;
    
    m_teapot1 = new CModel;
    m_teapot2 = new CModel;
    m_teapot3 = new CModel;
    m_teapot4 = new CModel;
    m_teapot5 = new CModel;
    m_teapot6 = new CModel;
    m_teapot7 = new CModel;
    m_teapot8 = new CModel;
    m_teapot9 = new CModel;
    m_teapot10 = new CModel;
    m_teapot15 = new CModel;
    m_teapot16 = new CModel;
    m_teapot17 = new CModel;
    m_teapot19 = new CModel;
    m_trolley = new CModel;
    m_lamborginhi = new CModel;
    
    m_pSpherePBR1 = new CSphere;
    m_pSpherePBR2 = new CSphere;
    m_pSpherePBR3 = new CSphere;
    m_pSpherePBR4 = new CSphere;
    m_pSpherePBR5 = new CSphere;
    m_pSpherePBR6 = new CSphere;
    m_pSpherePBR7 = new CSphere;
    m_pSpherePBR8 = new CSphere;
    m_pSpherePBR9 = new CSphere;
    m_pSpherePBR10 = new CSphere;
    m_pSpherePBR11 = new CSphere;
    m_pSpherePBR12 = new CSphere;
    m_pSpherePBR13 = new CSphere;
    m_pSpherePBR14 = new CSphere;
    m_pSpherePBR15 = new CSphere;
    m_pSpherePBR16 = new CSphere;
    m_pSpherePBR17 = new CSphere;
    m_pSpherePBR18 = new CSphere;
    m_pSpherePBR19 = new CSphere;
    
    m_pCube12 = new CCube(1.0f);
    m_pCube13 = new CCube(1.0f);
    m_pCube14 = new CCube(1.0f);
    m_pInteriorBox = new CCube(10.0f);
    m_pWoodenBox = new CCube(1.0f);
    m_pLamp = new CCube(1.0f);
    
    m_pTorus = new CTorus;
    m_pTorusKnot = new CTorusKnot;
    m_pMetaballs = new CMetaballs;
}

void Game::LoadResources(const std::string &path)
{
    // Create the planar terrain
    m_pPlanarTerrain->Create(path+"/textures/pbr/wood/", {
        { "albedo.png", TextureType::ALBEDO },           // albedo map
        { "metallic.png",  TextureType::METALNESS },           // metallic map
        { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
        { "normal.png", TextureType::NORMAL},          // normalMap 3
        { "ao.png",   TextureType::AO },                          // aoMap 4
        { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
        { "diffuse.png",   TextureType::DIFFUSE},
        { "specular.png",   TextureType::SPECULAR} 
    }, m_mapSize, m_mapSize, 5.0f, 50);
    
     
    // Create the heightmap terrain
    m_pHeightmapTerrain->Create((path+"/textures/heightmap/heightmap4.bmp").c_str(),
                                {
                                    { path+"/textures/heightmap/sand.png", TextureType::AMBIENT },            // ambientMap 0
                                    { path+"/textures/heightmap/stone.png", TextureType::DIFFUSE },           // diffuseMap 1
                                    { path+"/textures/heightmap/snow.png", TextureType::SPECULAR },           // specularMap 2
                                    { path+"/textures/heightmap/patchygrass.png", TextureType::NORMAL }       // normalMap 3
                                },
                                glm::vec3(0, 0, 0),
                                m_mapSize,
                                m_mapSize,
                                200.0f);
    
    m_pLamp->Create("", {} );
    m_pWoodenBox->Create(path+"/textures/pbr/woodenbox/",
                         {
                             { "albedo.png", TextureType::ALBEDO },           // albedo map
                             { "metallic.png",  TextureType::METALNESS },           // metallic map
                             { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                             { "normal.png", TextureType::NORMAL},          // normalMap 3
                             { "ao.png",   TextureType::AO },                          // aoMap 4
                             { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                             { "diffuse.png",   TextureType::DIFFUSE},
                             { "specular.png",   TextureType::SPECULAR},
                             { "bump.png", TextureType::DISPLACEMENT},      // bump
                         });
    m_pInteriorBox->Create(path+"/textures/pbr/wood/", {
        { "albedo.png", TextureType::ALBEDO},              // albedo map
        { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
        { "diffuse.png", TextureType::DIFFUSE},
        { "metallic.png",  TextureType::METALNESS },           // metallic map
        { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
        { "normal.png", TextureType::NORMAL},                  // normalMap 3
        { "ao.png",   TextureType::AO },            // aoMap 4
        { "specular.png",   TextureType::SPECULAR }
    } );
    
    
     // how to create ao, albedo, roughness, metallic, bump, normal, specular textures:
     // https://www.youtube.com/watch?v=qYB6hAYXoho
     // https://www.youtube.com/watch?v=0EHljiUMRA4
     // https://www.youtube.com/watch?v=-6cay07QxhM
     // https://www.youtube.com/watch?v=weTAM7CNbcU
     // https://www.youtube.com/watch?v=VBoHh6CR-kM
    // Texture downloaded from https://www.pinterest.co.uk/adigitaldreamer/free-pbr-materials/
    // https://www.textures.com/browse/pbr-materials/114558
    // https://3dtextures.me/
    // https://freepbr.com/
    m_pSpherePBR1->Create(path+"/textures/pbr/gold/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot1->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/gold/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR2->Create(path+"/textures/pbr/copper/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot2->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/copper/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR3->Create(path+"/textures/pbr/plastic/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot3->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/plastic/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR4->Create(path+"/textures/pbr/granite/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot4->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/granite/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
 
    m_pSpherePBR5->Create(path+"/textures/pbr/marble/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot5->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/marble/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR6->Create(path+"/textures/pbr/aluminum/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot6->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/aluminum/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR7->Create(path+"/textures/pbr/metal/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot7->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/metal/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR8->Create(path+"/textures/pbr/iron/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_teapot8->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/iron/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    
    m_pSpherePBR9->Create(path+"/textures/pbr/blackmarble/",
                           {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                               { "metallic.png",  TextureType::METALNESS },           // metallic map
                               { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                               { "normal.png", TextureType::NORMAL},          // normalMap 3
                               { "ao.png",   TextureType::AO },                          // aoMap 4
                               { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                               { "diffuse.png",   TextureType::DIFFUSE},
                               { "specular.png",   TextureType::SPECULAR}
                           }, 50, 50);
    m_teapot9->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/blackmarble/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
  
    m_pSpherePBR10->Create(path+"/textures/pbr/rustedmetal/",
                           {   { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                               { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                               { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                               { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                               { "ao.jpg",   TextureType::AO },                          // aoMap 4
                               { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                               { "diffuse.jpg",   TextureType::DIFFUSE},
                               { "specular.jpg",   TextureType::SPECULAR}
                           }, 50, 50);
    m_teapot10->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/rustedmetal/",
                       {   { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                           { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                           { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                           { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                           { "ao.jpg",   TextureType::AO },                          // aoMap 4
                           { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                           { "diffuse.jpg",   TextureType::DIFFUSE},
                           { "specular.jpg",   TextureType::SPECULAR}
                      });
  
    
    m_pSpherePBR11->Create(path+"/textures/pbr/circleplate/",
                           {
                               { "albedo.png", TextureType::ALBEDO },           // albedo map
                               { "metallic.png",  TextureType::METALNESS },           // metallic map
                               { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                               { "normal.png", TextureType::NORMAL},          // normalMap 3
                               { "ao.png",   TextureType::AO },                          // aoMap 4
                               { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                               { "diffuse.png",   TextureType::DIFFUSE},
                               { "specular.png",   TextureType::SPECULAR},
                               { "bump.png", TextureType::DISPLACEMENT},
                           }, 50, 50);
    m_pTorus->Create(path+"/textures/pbr/circleplate/",
                     {
                         { "albedo.png", TextureType::ALBEDO },           // albedo map
                         { "metallic.png",  TextureType::METALNESS },           // metallic map
                         { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                         { "normal.png", TextureType::NORMAL},          // normalMap 3
                         { "ao.png",   TextureType::AO },                          // aoMap 4
                         { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                         { "diffuse.png",   TextureType::DIFFUSE},
                         { "specular.png",   TextureType::SPECULAR},
                         { "bump.png", TextureType::DISPLACEMENT},
                    }, 50, 50, 2.0f, 1.0f);
    m_pTorusKnot->Create(path+"/textures/pbr/circleplate/",
                         {
                             { "albedo.png", TextureType::ALBEDO },           // albedo map
                             { "metallic.png",  TextureType::METALNESS },           // metallic map
                             { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                             { "normal.png", TextureType::NORMAL},          // normalMap 3
                             { "ao.png",   TextureType::AO },                          // aoMap 4
                             { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                             { "diffuse.png",   TextureType::DIFFUSE},
                             { "specular.png",   TextureType::SPECULAR},
                             { "bump.png", TextureType::DISPLACEMENT}
                         },
                         1024,         // in: Number of steps in the torus knot
                         32,           // in: Number of facets
                         20.0f,        // in: Scale of the knot
                         0.1f,         // in: Thickness of the knot
                         0.0f,         // in: Number of clumps in the knot
                         0.0f,         // in: Offset of the clump (in 0..2pi)
                         0.0f,         // in: Scale of a clump
                         2.0f,         // in: U coordinate scale
                         32.0f,        // in: V coordinate scale
                         7.0f,         // in: P parameter of the knot
                         -2.0f         // in: Q parameter of the knot
                         );
    
    
    m_pSpherePBR12->Create("", {}, 50, 50);
    m_pCube12->Create("", { });
    m_pMetaballs->Create(100.0f, 10, 0, 32, path+"/textures/pbr/metalpainted/",
                         {   { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                             { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                             { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                             { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                             { "ao.jpg",   TextureType::AO },                          // aoMap 4
                             { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                             { "diffuse.jpg",   TextureType::DIFFUSE},
                             { "specular.jpg",   TextureType::SPECULAR}
                         }); {
                             m_pMetaballs->SetGridSize(50);
                             CMarchingCubes::BuildTables();
                         }
    
    
    m_pSpherePBR13->Create(path+"/textures/pbr/brick/",
                           {
                               { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                               { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                               { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                               { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                               { "ao.jpg",   TextureType::AO },                          // aoMap 4
                               { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                               { "diffuse.jpg",   TextureType::DIFFUSE},
                               { "specular.jpg",   TextureType::SPECULAR},
                               { "bump.jpg", TextureType::DISPLACEMENT},
                               { "height.jpg", TextureType::HEIGHT }
                           }, 50, 50);
    m_pCube13->Create(path+"/textures/pbr/brick/",
                     {
                         { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                         { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                         { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                         { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                         { "ao.jpg",   TextureType::AO },                          // aoMap 4
                         { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                         { "diffuse.jpg",   TextureType::DIFFUSE},
                         { "specular.jpg",   TextureType::SPECULAR},
                         { "bump.jpg", TextureType::DISPLACEMENT},
                         { "height.jpg", TextureType::HEIGHT }
                     });

    
    m_pSpherePBR14->Create(path+"/textures/",
                           {   { "clean-gray-paper.png", TextureType::DIFFUSE},
                               { "moon_surface.jpg", TextureType::GLOSSINESS},
                           }, 50, 50);
    m_pCube14->Create(path+"/textures/",
                           {   { "clean-gray-paper.png", TextureType::DIFFUSE},
                               { "moon_surface.jpg", TextureType::GLOSSINESS},
                           });
    
    
    m_pSpherePBR15->Create(path+"/textures/pbr/dirtpile/",
                          {
                              { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                              { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                              { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                              { "ao.jpg",   TextureType::AO },                          // aoMap 4
                              { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                              { "diffuse.jpg",   TextureType::DIFFUSE},
                              { "specular.jpg",   TextureType::SPECULAR},
                              { "moss.png", TextureType::DISPLACEMENT }
                          }, 50, 50);
    m_teapot15->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/dirtpile/",
                      {
                          { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                          { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                          { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                          { "ao.jpg",   TextureType::AO },                          // aoMap 4
                          { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                          { "diffuse.jpg",   TextureType::DIFFUSE},
                          { "specular.jpg",   TextureType::SPECULAR},
                          { "moss.png", TextureType::DISPLACEMENT }
                      });
    
    m_pSpherePBR16->Create(path+"/textures/pbr/metalpainted/",
                           {   { "albedo.jpg", TextureType::ALBEDO},              // albedo map
                               { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                               { "diffuse.jpg", TextureType::DIFFUSE},
                               { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                               { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                               { "normal.jpg", TextureType::NORMAL},                  // normalMap 3
                               { "ao.jpg",   TextureType::AO },           // aoMap 4
                               { "specular.jpg",   TextureType::SPECULAR }
                           }, 50, 50);
    m_teapot16->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/metalpainted/",
                       {   { "albedo.jpg", TextureType::ALBEDO},              // albedo map
                           { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                           { "diffuse.jpg", TextureType::DIFFUSE},
                           { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                           { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                           { "normal.jpg", TextureType::NORMAL},                  // normalMap 3
                           { "ao.jpg",   TextureType::AO },           // aoMap 4
                           { "specular.jpg",   TextureType::SPECULAR }
                       });
    
    
    m_pSpherePBR17->Create(path+"/textures/", {}, 50, 50);
    m_teapot17->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/", {});
    
    
    m_pSpherePBR18->Create(path+"/textures/pbr/fireball/",
                           {
                               { "explosion.png", TextureType::DISPLACEMENT }
                           }, 50, 50);
    
    
    
    m_pSpherePBR19->Create(path+"/textures/pbr/diamondplate/",
    {
        { "albedo.png", TextureType::ALBEDO },           // albedo map
        { "metallic.png",  TextureType::METALNESS },           // metallic map
        { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
        { "normal.png", TextureType::NORMAL},          // normalMap 3
        { "ao.png",   TextureType::AO },                          // aoMap 4
        { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
        { "diffuse.png",   TextureType::DIFFUSE},
        { "specular.png",   TextureType::SPECULAR},
        { "bump.png", TextureType::DISPLACEMENT}
    }, 50, 50);
    m_teapot19->Create(path+"/models/teapot/utah-teapot.obj", path+"/textures/pbr/diamondplate/",
                       {
                           { "albedo.png", TextureType::ALBEDO },           // albedo map
                           { "metallic.png",  TextureType::METALNESS },           // metallic map
                           { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                           { "normal.png", TextureType::NORMAL},          // normalMap 3
                           { "ao.png",   TextureType::AO },                          // aoMap 4
                           { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                           { "diffuse.png",   TextureType::DIFFUSE},
                           { "specular.png",   TextureType::SPECULAR},
                           { "bump.png", TextureType::DISPLACEMENT}
                       });
    
    
    m_trolley->Create(path+"/models/trolley/Industrial_Trolley.obj", path+"/models/trolley/",
                      {
                          { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "ambient.png", TextureType::AMBIENT },            // ambientMap 0
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      });
    m_lamborginhi->Create(path+"/models/lamborginhi/lamborginhi.obj", path+"/models/lamborginhi/",
                      {
                          { "albedo.jpg", TextureType::ALBEDO },           // albedo map
                          { "ambient.jpg", TextureType::AMBIENT },            // ambientMap 0
                          { "metallic.jpg",  TextureType::METALNESS },           // metallic map
                          { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.jpg", TextureType::NORMAL},          // normalMap 3
                          { "ao.jpg",   TextureType::AO },                          // aoMap 4
                          { "diffuse.jpg",   TextureType::DIFFUSE},
                          { "specular.jpg",   TextureType::SPECULAR},
                          { "bump.jpg",   TextureType::DISPLACEMENT },
                          { "glossiness.jpg",   TextureType::GLOSSINESS }
                      });
    
    // font
    m_pFtFont->LoadFont(path+"/fonts/Arial.ttf", 32, TextureType::DEPTH);
    
    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(m_skyboxSize, path, TextureType::CUBEMAP, SkyboxType::Default, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
    m_pEnvSkybox->Create(m_skyboxSize, path, TextureType::CUBEMAP, SkyboxType::EnvironmentMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
    m_pIrrSkybox->Create(m_skyboxSize, path, TextureType::CUBEMAP, SkyboxType::IrradianceMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
    
    
    // screens
    m_pQuad->Create(path+"/textures/ppfx/", { {"noise_texture_colored.png", TextureType::DIFFUSE }}, 1.0f, 1.0f);
}
