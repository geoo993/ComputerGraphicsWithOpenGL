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
    m_pLamp = new CCube(1.0f);
    m_pSuitcase = new CModel;
    m_pSphere = new CSphere;
    m_pSpherePBR1 = new CSphere;
    m_pSpherePBR2 = new CSphere;
    m_pSpherePBR3 = new CSphere;
    m_pSpherePBR4 = new CSphere;
    m_pSpherePBR5 = new CSphere;
    m_pSpherePBR6 = new CSphere;
    m_pFireBallSphere = new CSphere;
    m_pParallaxCube = new CCube(1.0f);
    m_pWoodenBox = new CCube(1.0f);
    m_pTorus = new CTorus(5.0f);
    m_pTorusKnot = new CTorusKnot;
    m_pMetaballs = new CMetaballs;
}

void Game::LoadResources(const std::string &path)
{
    
    // Create the planar terrain
    m_pPlanarTerrain->Create(path+"/textures/pbr/metalpainted/", { // http://www.crazyrobinhood.org/textures/
        { "albedo.jpg", TextureType::ALBEDO},              // albedo map
        { "diffuse.jpg", TextureType::DIFFUSE},
        { "metallic.jpg",  TextureType::METALNESS },           // metallic map
        { "roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
        { "normal.jpg", TextureType::NORMAL},                  // normalMap 3
        { "ao.jpg",   TextureType::AO },           // aoMap 4
        { "specular.jpg",   TextureType::SPECULAR }
    }, m_mapSize, m_mapSize, 5.0f, 50); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    
    // Create the heightmap terrain
    m_pHeightmapTerrain->Create((path+"/textures/terrain/HeightMap/heightmap4.bmp").c_str(),
                                {
                                    { path+"/textures/terrain/HeightMap/sand.png", TextureType::AMBIENT },            // ambientMap 0
                                    { path+"/textures/terrain/HeightMap/stone.png", TextureType::DIFFUSE },           // diffuseMap 1
                                    { path+"/textures/terrain/HeightMap/snow.png", TextureType::SPECULAR },           // specularMap 2
                                    { path+"/textures/terrain/HeightMap/patchygrass.png", TextureType::NORMAL }       // normalMap 3
                                },
                                glm::vec3(0, 0, 0),
                                m_mapSize,
                                m_mapSize,
                                200.0f);
    
    m_pLamp->Create("", {} );
  
    /*
    /// https://www.cgtrader.com/free-3d-models/household/other/vintage-suitcase-528ee5e8-4a33-4e50-a8aa-aac7267d1b78
    m_pSuitcase->Create(path+"/models/VintageSuitcase/", "Vintage_Suitcase_LP.obj",
                        {   { "Vintage_Suitcase_Colour.png", TextureType::ALBEDO },         // albedo map
                            { "Vintage_Suitcase_Metallic.png",  TextureType::METALNESS },      // metallic map
                            { "Vintage_Suitcase_Roughness.png",   TextureType::ROUGHNESS},     // roughness map
                            { "Vintage_Suitcase_Normal.png", TextureType::NORMAL},             // normalMap
                            { "Vintage_Suitcase_AO.png",   TextureType::AO },                  // aoMap
                        });
    */
    // Create a sphere
    // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    // Also from https://www.pinterest.co.uk/adigitaldreamer/free-pbr-materials/
    m_pSphere->Create(path+"/textures/pbr/chippedpaintmetal/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "diffuse.png", TextureType::DIFFUSE },
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "specular.png",   TextureType::SPECULAR}
                      }, 50, 50);
    m_pSpherePBR1->Create(path+"/textures/pbr/gold/",
                      {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                          { "metallic.png",  TextureType::METALNESS },           // metallic map
                          { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                          { "normal.png", TextureType::NORMAL},          // normalMap 3
                          { "ao.png",   TextureType::AO },                          // aoMap 4
                          { "diffuse.png",   TextureType::DIFFUSE},
                          { "specular.png",   TextureType::SPECULAR}
                      }, 50, 50);
    m_pSpherePBR2->Create(path+"/textures/pbr/copper/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_pSpherePBR3->Create(path+"/textures/pbr/plastic/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_pSpherePBR4->Create(path+"/textures/pbr/granite/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_pSpherePBR5->Create(path+"/textures/pbr/marble/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);
    m_pSpherePBR6->Create(path+"/textures/pbr/aluminum/",
                          {   { "albedo.png", TextureType::ALBEDO },           // albedo map
                              { "metallic.png",  TextureType::METALNESS },           // metallic map
                              { "roughness.png",   TextureType::ROUGHNESS},         // roughness map
                              { "normal.png", TextureType::NORMAL},          // normalMap 3
                              { "ao.png",   TextureType::AO },                          // aoMap 4
                              { "diffuse.png",   TextureType::DIFFUSE},
                              { "specular.png",   TextureType::SPECULAR}
                          }, 50, 50);

    m_pParallaxCube->Create(path+"/textures/pixarLibrary/brick/", {
        { "bricks_diffuse.jpg", TextureType::DIFFUSE},
        { "bricks_normal.jpg", TextureType::NORMAL},
        { "bricks_disp.jpg", TextureType::DISPLACEMENT },
        { "bricks_height_map.png", TextureType::HEIGHT }
    } );
    
    m_pWoodenBox->Create(path+"/textures/woodenBox/",
                         {
                             { "woodenBoxDiffuse.png", TextureType::DIFFUSE},       // diffuseMap
                             { "woodenBoxSpecular.png", TextureType::SPECULAR},      // specularMap 
                             { "woodenBoxNormal.tga", TextureType::NORMAL},      // normalMap
                             { "woodenBoxBump.png", TextureType::DISPLACEMENT},      // bump
                         });
    
    m_pFireBallSphere->Create(path+"/textures/",
                              {   { "explosion.png", TextureType::NOISE }
                              }, 50, 50);
    m_pTorus->Create(path+"/textures/pixarLibrary/metal/", {
        {"Alloy_diamond_plate_pxr128.tif", TextureType::DIFFUSE},
        {"Alloy_diamond_plate_pxr128_normal.tif", TextureType::NORMAL},
        {"Alloy_diamond_plate_pxr128_bmp.tif", TextureType::SPECULAR}
    });
    
    m_pTorusKnot->Create(path+"/textures/pixarLibrary/metal/", {
        {"Round_mesh_pxr128.tif", TextureType::DIFFUSE},
        {"Round_mesh_pxr128_normal.tif", TextureType::NORMAL},
        {"Round_mesh_pxr128_bmp.tif", TextureType::SPECULAR}
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
    m_pMetaballs->Create(100.0f, 10, 0, 32, { { path+"/textures/pixarLibrary/metal/Wire_screen_pxr128.tif",  TextureType::DIFFUSE }} ); {
        m_pMetaballs->SetGridSize(50);
        CMarchingCubes::BuildTables();
    }
    
    // font
    m_pFtFont->LoadFont(path+"/fonts/Arial.ttf", 32, TextureType::DEPTH);
    
    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(m_skyboxSize, path, TextureType::CUBEMAP, SkyboxType::Default, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
    m_pEnvSkybox->Create(m_skyboxSize, path, TextureType::CUBEMAP, SkyboxType::EnvironmentMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
    m_pIrrSkybox->Create(m_skyboxSize, path, TextureType::CUBEMAP, SkyboxType::IrradianceMap, m_pShaderPrograms, this, TextureType::EMISSION, m_skyboxNumber);
    
    
    // screens
    m_pQuad->Create(path+"/textures/pixarLibrary/fabric/", { {"Wild_flowers_pxr128.tif", TextureType::DIFFUSE}}, 1.0f, 1.0f);
}
