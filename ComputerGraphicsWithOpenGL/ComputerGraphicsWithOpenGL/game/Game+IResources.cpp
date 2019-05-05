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
    m_pPlanarTerrain = new CPlane;
    m_pHeightmapTerrain = new CHeightMapTerrain;
    m_pLamp = new CCube(1.0f);
    //m_pCrossbow = new CModel;
    //m_pHorn = new CModel;
    //m_pSchofield = new CModel;
    //m_pMusket = new CModel;
    //m_pGrenade = new CModel;
    m_pFlashlight = new CModel;
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
    m_pEquirectangularCube = new CEquirectangularCube(1.0f);
}

void Game::LoadResources(const std::string &path)
{
    
    // font
    m_pFtFont->LoadFont(path+"/fonts/Arial.ttf", 32, TextureType::DEPTH);
    
    // screens
    m_pQuad->Create(path+"/textures/pixarLibrary/fabric/", { {"Wild_flowers_pxr128.tif", TextureType::DIFFUSE}}, 1.0f, 1.0f);
    
    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(m_mapSize, path, TextureType::CUBEMAP, m_skyboxNumber);
    
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
    
    /// https://www.cgtrader.com/free-3d-models/military/other/crossbow-d180227
    //m_pCrossbow->Load(path+"/models/barrel/barrels_obj.obj");
    
    /// https://www.cgtrader.com/free-3d-models/household/kitchenware/drinking-horn
    //m_pHorn;
    
    /// https://www.cgtrader.com/free-3d-models/military/gun/musket-d180221
    // m_pMusket
    
    /// https://www.cgtrader.com/free-3d-models/military/gun/schofield-3-co2-bb
    // m_pSchofield
    
    /// https://www.cgtrader.com/free-3d-models/military/other/mk2-grenade-1c0f476e-3caa-4b74-9b3f-22280615a688
    //m_pGrenade->Create(path+"/models/mk2_grenade/", "MK2.obj");
    
    /// https://www.cgtrader.com/free-3d-models/military/gun/mipim-d180606
    //m_pFlashlight
    
    
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
   
    m_pEquirectangularCube->Create(path+"/skyboxes/deserthighway/", {
        { "Road_to_MonumentValley_Ref.hdr", TextureType::EMISSION},
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
                         20.0f,         // in: Scale of the knot
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
}
