
#include "Game.h"


// Constructor
Game::Game()
{
    // game window
    m_gameWindow = nullptr;
    m_gameManager = nullptr;
    
    // game timer
    m_pGameTimer = nullptr;
    m_timeInSeconds = 0.0f;
    m_timeInMilliSeconds = 0.0f;
    m_timePerSecond = 0.0f;
    m_deltaTime = 0.5f;
    m_elapsedTime = 0.0f;
    m_framesPerSecond = 0;
    m_frameCount = 0;
    
    //audio settings
    m_pAudio = nullptr;
    m_audioNumber = 0;
    m_changeAudio = false;
    m_audioFiles.reserve(10);
    
    //camera setting
    m_pCamera = nullptr;
    
    // materials
    m_materialShininess = 30.0f;
    m_uvTiling = 1.4f;
    m_magnitude = 0.3f;
    
    //textures settings
    m_textures.reserve(50);
    
    // shader programs
    m_pShaderPrograms = nullptr;
    
    // lights
    m_pLamp = nullptr;
    m_ambient = 0.4f;
    m_diffuse = 2.5f;
    m_specular = 0.6f;
    m_exposure = 0.8f;
    m_gama = 1.2f;
    m_HDR = true;
    
    // Attenuation
    m_constant = 1.0f;
    m_linear = 0.09f;
    m_exponent = 0.32f;
    
    // Dir Light
    m_dirColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_dirIntensity = 0.8f;
    m_directionalLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f),
    
    // Point Light
    m_pointIntensity = 2500.5f;
    m_pointLightPositionsIndex = 0;
    m_pointLightPositions = {
        glm::vec3(  100.0f,  400.0f,  50.0f      ),
        glm::vec3(  226.0f,  583.0f,  -32.0f      ),
        glm::vec3(  -335.7f,  550.2f,  122.0f      ),
        glm::vec3(  102.3f, 663.3f, -44.0f      ),
        glm::vec3(  -104.0f,  612.0f, -200.0f    ),
        glm::vec3(  300.0f,  620.0f, -233.0f      ),
        glm::vec3(  -600.0f,  750.0f, 130.0f      )
    };
    
    m_pointLightColors = {
        glm::vec3(  1.0f,  0.1f,  0.9f      ),
        glm::vec3(  0.1f,  0.9f,  0.8f      ),
        glm::vec3(  1.0f,  1.0f,  0.0f      ),
        glm::vec3(  0.1f, 0.9f, 0.0f      ),
        glm::vec3(  0.0f,  0.0f, 1.0f    ),
        glm::vec3(  1.0f,  0.0f, 0.1f      ),
        glm::vec3(  21.0f,  20.0f, 50.0f      )
    };
    
    // Spot Light
    m_spotColor = glm::vec3(0.3f, 0.5f, 1.0f);;
    m_spotIntensity = 2500.4f;
    m_spotCutOff = 22.5f;
    m_spotOuterCutOff = 28.0f;
    
    // screens
    m_pQuad = nullptr;
    
    // skybox
    m_pSkybox = nullptr;
    m_mapSize = 3000.0f;
    m_skyboxNumber = 0;
    m_changeSkybox = false;
    
    // terrain
    m_pPlanarTerrain = nullptr;
    m_pHeightmapTerrain = nullptr;
    m_heightMapMinHeight = 0.0f ;
    m_heightMapMaxHeight = 100.0f;
    
    //models
    m_pCrossbow = nullptr;
    m_pHorn = nullptr;
    m_pSchofield = nullptr;
    m_pMusket = nullptr;
    m_pGrenade = nullptr;
    m_pNanosuit = nullptr;
    m_pFlashlight = nullptr;
    
    //sphere object
    m_pSphere = nullptr;
    m_spherePosition = glm::vec3(130.0f,350.0f,-450.0f);
    
    //cube object
    m_pCube = nullptr;
    m_pParallaxCube = nullptr;
    m_pChromaticAberrationCube = nullptr;
    m_cubePosition = glm::vec3(20.0f,120.0f, -50.0f);
    
    // woodenBox
    m_pWoodenBox = nullptr;
    m_woodenBoxesColor = glm::vec3(1.0f, 0.5f, 0.31f);
    m_woodenBoxesUseTexture = true;
    m_woodenBoxesPosition = {
        glm::vec3(  -186.0f,  593.0f,  -132.0f   ),
        glm::vec3(  -135.7f,  550.2f,  322.0f   ),
        glm::vec3(  52.3f, 523.3f, -44.0f      ),
        glm::vec3(  -24.0f,  452.0f, -112.0f    ),
        glm::vec3(  220.0f,  620.0f, -133.0f    ),
        glm::vec3(  -152.3f, 663.3f, -74.0f  ),
        glm::vec3(  224.0f,  572.0f, 12.0f   ),
        glm::vec3(  -220.0f,  610.0f, 133.0f  )
    };
    
    //torus object
    m_pTorus = nullptr;
    m_torusPosition = glm::vec3(-160.0f,530.0f,-450.0f);
    
    //torusknot object
    m_pTorusKnot = nullptr;
    m_torusKnotPosition = glm::vec3(130.0f, 560.0f, 250.0f);
    
    // metalballs
    m_pMetaballs = nullptr;
    m_metalballsPosition = glm::vec3(500.0f,500.0f,0.0f);
    
    // hud
    m_enableHud = true;
    m_pFtFont = nullptr;
    
    // inputs
    m_mouseButtonDown = false;
    m_enableMouseMovement = true;
    m_isMouseCursorVisible = true;
    m_mouseMouseMoveClickSwitch = false;
    m_mouseX, m_mouseY = 0.0;
    m_keyPressTime = 0.0;
    m_lastKeyPressTime = 0.0f;
    m_lastKeyPress = -1;
    m_isKeyPressRestriction = true;
}

// Destructor
Game::~Game()
{
    //delete objects when desrtuctor occurs
    delete m_pCamera;
    delete m_pFtFont;
    delete m_pAudio;
    delete m_pSkybox;
    delete m_pPlanarTerrain;
    delete m_pHeightmapTerrain;
    delete m_pLamp;
    delete m_pCrossbow;
    delete m_pHorn;
    delete m_pSchofield;
    delete m_pMusket;
    delete m_pGrenade;
    delete m_pFlashlight;
    delete m_pSphere;
    delete m_pCube;
    delete m_pParallaxCube;
    delete m_pChromaticAberrationCube;
    delete m_pWoodenBox;
    delete m_pNanosuit;
    delete m_pTorus;
    delete m_pTorusKnot;
    delete m_pMetaballs;
    delete m_pQuad;
    
    for (GLuint i = 0; i < m_pFBOs.size(); i++) {
        // delete current buffers
        m_pFBOs[i]->Release();
        delete m_pFBOs[i];
    }
    m_pFBOs.clear();
    
    if (m_pShaderPrograms != nullptr) {
        for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
            delete (*m_pShaderPrograms)[i];
    }
    delete m_pShaderPrograms;
    
    delete m_pGameTimer;
    
    delete m_gameManager;
    
    delete m_gameWindow;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise()
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
    m_pCrossbow = new CModel;
    m_pHorn = new CModel;
    m_pSchofield = new CModel;
    m_pMusket = new CModel;
    m_pGrenade = new CModel;
    m_pNanosuit = new CModel;
    m_pFlashlight = new CModel;
    m_pSphere = new CSphere;
    m_pCube = new CCube(1.0f);
    m_pParallaxCube = new CCube(1.0f);
    m_pChromaticAberrationCube = new CCube(1.0f);
    m_pWoodenBox = new CCube(1.0f);
    m_pTorus = new CTorus(5.0f);
    m_pTorusKnot = new CTorusKnot;
    m_pMetaballs = new CMetaballs;
}

void Game::LoadResources(const std::string &path)
{
   
    // font
    m_pFtFont->LoadFont(path+"/fonts/Arial.ttf", 32);
    
    // screens
    m_pQuad->Create(path+"/textures/pixarLibrary/fabric/", { {"Wild_flowers_pxr128.tif", TextureType::DIFFUSE}}, 1.0f, 1.0f);
    
    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(m_mapSize, path, TextureType::CUBEMAP, m_skyboxNumber);
    
    // Create the planar terrain
    m_pPlanarTerrain->Create(path+"/textures/terrain/", { {"grassfloor.jpg", TextureType::AMBIENT} },
                             m_mapSize, m_mapSize, 50.0f, 50); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    
    // Create the heightmap terrain
    m_pHeightmapTerrain->Create((path+"/textures/terrain/heightmap4.bmp").c_str(),
                                {
                                    { path+"/textures/terrain/sand.png", TextureType::AMBIENT },            // ambientMap 0
                                    { path+"/textures/terrain/stone.png", TextureType::DIFFUSE },           // diffuseMap 1
                                    { path+"/textures/terrain/snow.png", TextureType::SPECULAR },           // specularMap 2
                                    { path+"/textures/terrain/patchygrass.png", TextureType::NORMAL }       // normalMap 3
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
    m_pGrenade->Create(path+"/models/mk2_grenade/", "MK2.obj");
    
    m_pNanosuit->Create(path+"/models/nanosuit/", "nanosuit.obj");
    
    /// https://www.cgtrader.com/free-3d-models/military/gun/mipim-d180606
    //m_pFlashlight
    
    
    m_pCube->Create(path+"/textures/pixarLibrary/brick/", {
        { "Standard_red_pxr128.tif", TextureType::DIFFUSE},
        { "Standard_red_pxr128_normal.tif", TextureType::NORMAL},
        { "Standard_red_pxr128_bmp.tif", TextureType::SPECULAR}
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
                      {   { "chipped-paint-metal-albedo.png", TextureType::AMBIENT },           // ambient map (albedo map)
                          { "chipped-paint-metal-metal.png",  TextureType::DIFFUSE },           // diffuse map (metallic map)
                          { "chipped-paint-metal-rough2.png",   TextureType::SPECULAR},         // specular map (roughness map)
                          { "chipped-paint-metal-normal-dx.png", TextureType::NORMAL},          // normalMap 3
                          { "chipped-paint-ao.png",   TextureType::AO }                          // aoMap 4
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

void Game::PreRendering() {
    
    Update();
}

// Render scene method runs
void Game::Render()
{

    ActivateFBO(m_currentPPFXMode);
    
    RenderScene();
    
    ResetFrameBuffer();
    
    // Post Processing Effects
    RenderPPFX( m_currentPPFXMode );
    
    // Draw the 2D graphics after the 3D graphics
    RenderHUD();
    
    // Swap buffers right after rendering all, this is to show the current rendered image
    m_gameWindow->SwapBuffers();
}

void Game::PostRendering() {
    UpdateCameraEndFrame(m_deltaTime);
    
}

// Update method runs repeatedly with the Render method
void Game::Update()
{
    UpdateGameTimer();
    
    UpdateCamera(m_deltaTime, keyPressedCode, m_enableMouseMovement);
    
    UpdateMouseControls(mouseButton, mouseAction);
    UpdateKeyBoardControls(keyPressedCode, keyReleasedCode, keyPressedAction);
    
    UpdateAudio();
}


// The game loop runs repeatedly until game over
void Game::GameLoop()
{
    // Variable timer
    m_pGameTimer->Start();
    PreRendering();
    Render();
    PostRendering();
    m_deltaTime = m_pGameTimer->Elapsed();
}


static void OnMouseDown_callback(GLFWwindow* window, int button, int action, int mods){
    //std::cout << "Mouse Down with button: " << button << " and with action: " << action << std::endl;
    mouseButton = button;
    mouseAction = action;
}


static void OnKeyDown_callback( GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keyPressedAction = action;
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    switch (action) {
        case GLFW_PRESS:
            keyPressedCode = key;
            break;
        case GLFW_RELEASE:
            keyReleasedCode = key;
            break;
        default:
            break;
    }
}


void Game::Execute(const std::string &filepath, const GLuint &width, const GLuint &height)
{

    Initialise();
    InitialiseGameWindow("OpenGL Window", filepath, width, height);
    InitialiseFrameBuffers(width, height);
    InitialiseCamera(width, height, glm::vec3(220.0f, 500.0f, -400.0f));
    InitialiseAudio(filepath);
    
    LoadShaderPrograms(filepath);
    LoadResources(filepath);
    LoadTextures(filepath);
    
    m_gameManager->SetLoaded(true); // everything has loaded
    
    m_gameWindow->SetInputs(OnKeyDown_callback, OnMouseDown_callback);
    
    m_gameWindow->PreRendering();
    
    m_gameManager->SetActive(true); // game is now going to be active, or activate application
    
    while ( !m_gameWindow->ShouldClose() ){
        m_gameWindow->ClearBuffers();
        
        if (m_gameManager->IsActive()) {
            GameLoop();
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Do not consume processor power if application isn't active
        }
        
        m_gameWindow->PostRendering();
    }
    
    m_gameWindow->DestroyWindow();
    
    return;
}
