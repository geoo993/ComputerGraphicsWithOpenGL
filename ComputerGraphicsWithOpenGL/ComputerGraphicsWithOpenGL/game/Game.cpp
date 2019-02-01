
#include "Game.h"


// Constructor
Game::Game()
{
    // game time
    m_pGameTimer = nullptr;
    
    // shader programs
    m_pShaderPrograms = nullptr;
    
    // hud
    m_enableHud = true;
    m_pFtFont = nullptr;
    
    // terrain
    m_pPlanarTerrain = nullptr;
    m_pHeightmapTerrain = nullptr;
    m_heightMapMinHeight = 0.0f ;
    m_heightMapMaxHeight = 100.0f;
    
    // skybox
    m_pSkybox = nullptr;
    m_mapSize = 3000.0f;
    m_skyboxNumber = 0;
    m_changeSkybox = false;
    
    //screen and debuging settings
    m_framesPerSecond = 0;
    m_frameCount = 0;
    m_elapsedTime = 0.0f;
    m_deltaTime = 0.5f;
    m_timeInSeconds = 0.0f;
    m_timeInMilliSeconds = 0.0f;
    m_timePerSecond = 0.0f;
    
    //audio settings
    m_pAudio = nullptr;
    m_audioNumber = 0;
    m_changeAudio = false;
    m_audioFiles.reserve(5);
    
    //textures settings
    m_textures.reserve(50);
    
    //models
    m_pBarrel = nullptr;
    m_barrelPosition = glm::vec3(30.0f,10.0f,0.0f);
    m_barrelRotation = 0.0f;
    
    //sphere object
    m_pSphere = nullptr;
    m_spherePosition = glm::vec3(130.0f,50.0f,50.0f);
    
    //cube object
    m_pCube = nullptr;
    m_cubePosition = glm::vec3(20.0f,120.0f, -50.0f);
    
    //torus object
    m_pTorus = nullptr;
    m_torusPosition = glm::vec3(-160.0f,30.0f,150.0f);
    
    //torusknot object
    m_pTorusKnot = nullptr;
    m_torusKnotPosition = glm::vec3(130.0f,160.0f,250.0f);
    
    //camera setting
    m_pCamera = nullptr;
    
    // inputs
    m_mouseButtonDown = false;
    m_enableMouseMovement = true;
    m_isMouseCursorVisible = true;
    m_mouseMouseMoveClickSwitch = false;
    m_mouseX = 0.0;
    m_mouseY = 0.0;
    m_keyPressTime = 0.0;
    m_lastKeyPressTime = 0.0f;
    m_lastKeyPress = -1;
    m_isKeyPressRestriction = true;
    
    // post processing
    m_currentPPFXMode = PostProcessingEffectMode::DefaultFrameBuffer;
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
    delete m_pBarrel;
    delete m_pSphere;
    delete m_pCube;
    delete m_pTorus;
    delete m_pTorusKnot;
    
    if (m_pShaderPrograms != nullptr) {
        for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
            delete (*m_pShaderPrograms)[i];
    }
    delete m_pShaderPrograms;
    
    delete m_pGameTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise()
{
    /// Create objects
    m_pGameTimer = new CHighResolutionTimer;
    m_pCamera = new CCamera;
    m_pShaderPrograms = new std::vector <CShaderProgram *>;
    m_pFtFont = new CFreeTypeFont;
    m_pAudio = new CAudio;
    m_pSkybox = new CSkybox;
    m_pPlanarTerrain = new CPlane;
    m_pHeightmapTerrain = new CHeightMapTerrain;
    m_pBarrel = new COpenAssetImportMesh;
    m_pSphere = new CSphere;
    m_pCube = new CCube(1.0f);
    m_pTorus = new CTorus(5.0f);
    m_pTorusKnot = new CTorusKnot;
}

void Game::LoadResources(const std::string &path)
{
   
    // font
    m_pFtFont->LoadFont(path+"/fonts/Arial.ttf", 32);
    
    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(m_mapSize, path, m_skyboxNumber);
    
    // Create the planar terrain
    m_pPlanarTerrain->Create(path+"/textures/terrain/grassfloor.jpg", m_mapSize, m_mapSize, 50.0f, 50); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    
    // Create the heightmap terrain
    m_pHeightmapTerrain->Create((path+"/textures/terrain/heightmap4.bmp").c_str(),
                                {
                                    path+"/textures/terrain/sand.png",
                                    path+"/textures/terrain/patchygrass.png",
                                    path+"/textures/terrain/stone.png",
                                    path+"/textures/terrain/snow.png",
                                },
                                glm::vec3(0, 0, 0),
                                m_mapSize,
                                m_mapSize,
                                200.0f);
    
    m_pBarrel->Load(path+"/models/barrel/barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
    
    m_pCube->Create(path+"/textures/woodenBox/", "woodenBox.jpg");
    
    // Create a sphere
    m_pSphere->Create(path+"/textures/", "moon_surface.jpg", 50, 50);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    
    m_pTorus->Create(path+"/textures/3912Tex/", "3912-diffuse.jpg");
    
    m_pTorusKnot->Create(path+"/textures/pattern-design.jpg",
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
}

// Update method runs repeatedly with the Render method
void Game::Update()
{
   
    UpdateCamera(m_deltaTime, keyPressedCode, m_enableMouseMovement);
    
    MouseControls(mouseButton, mouseAction);
    KeyBoardControls(keyPressedCode, keyReleasedCode, keyPressedAction);
    
    UpdateAudio();
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
    // Variable timer
    m_pGameTimer->Start();
    CalculateGameTime();
    Update();
    Render();
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
    
    m_gameWindow.CreateWindow("OpenGL Window", width, height);
    m_gameManager.SetResourcePath(filepath);
    
    Initialise();
    InitialiseFrameBuffers(width, height);
    InitialiseCamera(width, height, glm::vec3(1.0f, 230.0f, 100.0f));
    InitialiseAudio(filepath);
    
    LoadShaderPrograms(filepath);
    LoadResources(filepath);
    LoadTextures(filepath);
    
    m_gameManager.SetLoaded(true); // everything has loaded
    
    m_gameWindow.SetInputs(OnKeyDown_callback, OnMouseDown_callback);
    
    m_gameWindow.PreRendering();
    
    m_gameManager.SetActive(true); // game is now going to be active, or activate application
    
    while ( !m_gameWindow.ShouldClose() ){
        
        m_gameWindow.ClearBuffers();
        
        if (m_gameManager.IsActive()) {
            GameLoop();
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Do not consume processor power if application isn't active
        }
        
        m_gameWindow.PostRendering();
    }
    
    m_gameWindow.DestroyWindow();
    
    return;
}
