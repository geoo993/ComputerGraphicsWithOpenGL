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
    m_pTrolley = new CModel;
    m_pShotgun = new CModel;
    m_pSuitcase = new CModel;
    m_pFlareGun = new CModel;
    m_pMedicalSaw = new CModel;
    m_pSphere = new CSphere;
    m_pFireBallSphere = new CSphere;
    m_pCube = new CCube(1.0f);
    m_pInteriorBox = new CCube(10.0f);
    m_pParallaxCube = new CCube(1.0f);
    m_pChromaticAberrationCube = new CCube(1.0f);
    m_pWoodenBox = new CCube(1.0f);
    m_pTorus = new CTorus(5.0f);
    m_pTorusKnot = new CTorusKnot;
    m_pMetaballs = new CMetaballs;
}

void Game::LoadResources(const std::string &path)
{
    
    // Create the planar terrain
    m_pPlanarTerrain->Create(path+"/textures/pbr/metalpainted/", { // http://www.crazyrobinhood.org/textures/
        { "Metal_Painted_001_albedo.jpg", TextureType::ALBEDO},              // albedo map
        { "Metal_Painted_001_diffuse.jpg", TextureType::DIFFUSE},
        { "Metal_Painted_001_metallic.jpg",  TextureType::METALNESS },           // metallic map
        { "Metal_Painted_001_roughness.jpg",   TextureType::ROUGHNESS},         // roughness map
        { "Metal_Painted_001_normal.jpg", TextureType::NORMAL},                  // normalMap 3
        { "Metal_Painted_001_ambientOcclusion.jpg",   TextureType::AO },           // aoMap 4
        { "Metal_Painted_001_specular.jpg",   TextureType::SPECULAR }
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
    
    m_pLamp->Create(path+"/textures/woodenBox/", {} );
    
    /// https://www.cgtrader.com/free-3d-models/vehicle/industrial/industrial-trolley
    m_pTrolley->Create(path+"/models/Trolley/", "Industrial_Trolley.obj",
                       {   { "Trolley Base Color.tga", TextureType::ALBEDO },         // albedo map
                           { "Trolley Metallic.tga",  TextureType::METALNESS },      // metallic map
                           { "Trolley Roughness.tga",   TextureType::ROUGHNESS},     // roughness map
                           { "Trolley Normal.tga", TextureType::NORMAL},             // normalMap
                           { "Trolley Ambient Occlusion.tga",   TextureType::AO },   // aoMap
                       });
    
    /// https://www.cgtrader.com/free-3d-models/military/gun/lowpoly-pbr-quad-barrel-shotgun
    m_pShotgun->Create(path+"/models/QuadShotgun/", "Quad_Shotgun.obj",
    {
        { "Quad_Shotgun_BaseColor.png", TextureType::ALBEDO },         // albedo map
        { "Quad_Shotgun_Metallic.png",  TextureType::METALNESS },      // metallic map
        { "Quad_Shotgun_Roughness.png",   TextureType::ROUGHNESS},     // roughness map
        { "Quad_Shotgun_Normal.png", TextureType::NORMAL},             // normalMap
        { "Quad_Shotgun_AO.png",   TextureType::AO },                   // aoMap
    });
    
    /// https://www.cgtrader.com/free-3d-models/military/gun/lowpoly-pbr-flare-gun
    m_pFlareGun->Create(path+"/models/FlareGun/", "FlareGun.obj",
                        {
                            { "FlareGun_BaseColor.png", TextureType::ALBEDO },         // albedo map
                            { "FlareGun_Metallic.png",  TextureType::METALNESS },      // metallic map
                            { "FlareGun_Roughness.png",   TextureType::ROUGHNESS},     // roughness map
                            { "FlareGun_Normal.png", TextureType::NORMAL},             // normalMap
                            { "FlareGun_AO.png",   TextureType::AO },                   // aoMap
                        });
    
    /// https://www.cgtrader.com/free-3d-models/household/other/vintage-suitcase-528ee5e8-4a33-4e50-a8aa-aac7267d1b78
    m_pSuitcase->Create(path+"/models/VintageSuitcase/", "Vintage_Suitcase_LP.obj",
                        {   { "Vintage_Suitcase_Colour.png", TextureType::ALBEDO },         // albedo map
                            { "Vintage_Suitcase_Metallic.png",  TextureType::METALNESS },      // metallic map
                            { "Vintage_Suitcase_Roughness.png",   TextureType::ROUGHNESS},     // roughness map
                            { "Vintage_Suitcase_Normal.png", TextureType::NORMAL},             // normalMap
                            { "Vintage_Suitcase_AO.png",   TextureType::AO },                  // aoMap
                        });

    /// https://www.cgtrader.com/free-3d-models/industrial/tool/medical-saw
    m_pMedicalSaw->Create(path+"/models/MedicalSaw/", "SM_Medical_Saw_01.obj",
                          {   { "BaseColor.png", TextureType::ALBEDO },         // albedo map
                              { "Metallic.png",  TextureType::METALNESS },      // metallic map
                              { "Roughness.png",   TextureType::ROUGHNESS},     // roughness map
                              { "Normal.png", TextureType::NORMAL},             // normalMap
                              { "AO.png",   TextureType::AO },                  // aoMap
                          });
    
    m_pCube->Create(path+"/textures/pixarLibrary/brick/", {
        { "Standard_red_pxr128.tif", TextureType::DIFFUSE},
        { "Standard_red_pxr128_normal.tif", TextureType::NORMAL},
        { "Standard_red_pxr128_bmp.tif", TextureType::SPECULAR}
    } );
    m_pInteriorBox->Create(path+"/textures/pbr/harshbricks/", {
        { "harshbricks-albedo.png", TextureType::ALBEDO},              // albedo map
        { "harshbricks-diffuse.png", TextureType::DIFFUSE},
        { "harshbricks-metal.png",  TextureType::METALNESS },           // metallic map
        { "harshbricks-roughness.png",   TextureType::ROUGHNESS},         // roughness map
        { "harshbricks-normal.png", TextureType::NORMAL},                  // normalMap 3
        { "harshbricks-ao.png",   TextureType::AO },            // aoMap 4
        { "harshbricks-specular.png",   TextureType::SPECULAR }
    } );
    
    m_pParallaxCube->Create(path+"/textures/pixarLibrary/brick/", {
        { "bricks_diffuse.jpg", TextureType::DIFFUSE},
        { "bricks_normal.jpg", TextureType::NORMAL},
        { "bricks_disp.jpg", TextureType::DISPLACEMENT },
        { "bricks_height_map.png", TextureType::HEIGHT }
    } );
    
    m_pChromaticAberrationCube->Create(path+"/textures/", {
        { "clean-gray-paper.png", TextureType::DIFFUSE},
        { "moon_surface.jpg", TextureType::GLOSSINESS}
    } );
   
    m_pWoodenBox->Create(path+"/textures/woodenBox/",
                         {
                             { "woodenBoxDiffuse.png", TextureType::DIFFUSE},       // diffuseMap 1
                             { "woodenBoxSpecular.png", TextureType::SPECULAR},      // specularMap 2
                         });
    
    // Create a sphere
    // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    m_pSphere->Create(path+"/textures/pbr/chippedpaintmetal/",
                      {   { "chipped-paint-metal-albedo.png", TextureType::ALBEDO },           // albedo map
                          { "chipped-paint-metal-diffuse.png", TextureType::DIFFUSE },
                          { "chipped-paint-metal-metal.png",  TextureType::METALNESS },           // metallic map
                          { "chipped-paint-metal-rough.png",   TextureType::ROUGHNESS},         // roughness map
                          { "chipped-paint-metal-normal.png", TextureType::NORMAL},          // normalMap 3
                          { "chipped-paint-ao.png",   TextureType::AO },                          // aoMap 4
                          { "chipped-paint-specular.png",   TextureType::SPECULAR}
                      }, 50, 50);
    m_pFireBallSphere->Create(path+"/textures/",
                              //{   { "explosion.png", TextureType::NOISE }
                              {   { "watercolor.png", TextureType::NOISE }
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
    m_pSkybox->Create(m_mapSize, path, TextureType::CUBEMAP, SkyboxType::Default, nullptr, nullptr, TextureType::EMISSION, m_skyboxNumber);
    CShaderProgram *pEquirectangularCubeProgram = (*m_pShaderPrograms)[77];
    SetMaterialUniform(pEquirectangularCubeProgram, "material", glm::vec4(1.0f));
    m_pEnvSkybox->Create(m_mapSize, path, TextureType::CUBEMAP, SkyboxType::EnvironmentMap, nullptr, pEquirectangularCubeProgram, TextureType::EMISSION, m_skyboxNumber);
    
    CShaderProgram *pIrradianceProgram = (*m_pShaderPrograms)[78];
    SetMaterialUniform(pIrradianceProgram, "material", glm::vec4(1.0f));
    m_pIrrSkybox->Create(m_mapSize, path, TextureType::CUBEMAP, SkyboxType::IrradianceMap, pIrradianceProgram, pEquirectangularCubeProgram, TextureType::EMISSION, m_skyboxNumber);
    
    
    // screens
    m_pQuad->Create(path+"/textures/pixarLibrary/fabric/", { {"Wild_flowers_pxr128.tif", TextureType::DIFFUSE}}, 1.0f, 1.0f);
}
