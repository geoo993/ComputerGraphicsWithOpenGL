
#include "Game.h"


// Constructor
Game::Game()
{
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
    m_materialShininess = 32.0f;
    
    //textures settings
    m_textures.reserve(50);
    
    // shader programs
    m_pShaderPrograms = nullptr;
    
    // lights
    m_pLamp = nullptr;
    m_ambient = 0.2f;
    m_diffuse = 0.7f;
    m_specular = 0.8f;
    
    // Attenuation
    m_constant = 1.0f;
    m_linear = 0.09f;
    m_exponent = 0.32f;
    
    // Dir Light
    m_dirColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_dirIntensity = 0.8f;
    m_directionalLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f),
    
    // Point Light
    m_pointIntensity = 1500.5f;
    m_pointLightPositionsIndex = 0;
    m_pointLightPositions = {
        glm::vec3(  100.0f,  400.0f,  50.0f      ),
        glm::vec3(  226.0f,  583.0f,  -32.0f      ),
        glm::vec3(  -335.7f,  550.2f,  122.0f      ),
        glm::vec3(  102.3f, 663.3f, -44.0f      ),
        glm::vec3(  -224.0f,  612.0f, -312.0f    ),
        glm::vec3(  420.0f,  620.0f, -233.0f      )
    };
    
    m_pointLightColors = {
        glm::vec3(  1.0f,  0.0f,  0.9f      ),
        glm::vec3(  0.1f,  0.9f,  0.8f      ),
        glm::vec3(  1.0f,  1.0f,  0.0f      ),
        glm::vec3(  0.1f, 0.9f, 0.0f      ),
        glm::vec3(  0.0f,  0.0f, 1.0f    ),
        glm::vec3(  1.0f,  0.0f, 0.1f      )
    };
    
    // Spot Light
    m_spotColor = glm::vec3(1.0f, 1.0f, 1.0f);;
    m_spotIntensity = 1500.4f;
    m_spotCutOff = 12.5f;
    m_spotOuterCutOff = 18.0f;
    
    // skybox
    m_pSkybox = nullptr;
    m_mapSize = 3000.0f;
    m_skyboxNumber = 10;
    m_changeSkybox = false;
    
    // terrain
    m_pPlanarTerrain = nullptr;
    m_pHeightmapTerrain = nullptr;
    m_heightMapMinHeight = 0.0f ;
    m_heightMapMaxHeight = 100.0f;
    
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
    
    // woodenBox
    m_pWoodenBox = nullptr;
    m_woodenBoxesColor = glm::vec3(0.5f, 0.6f, 0.1f);
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
    m_torusPosition = glm::vec3(-160.0f,30.0f,150.0f);
    
    //torusknot object
    m_pTorusKnot = nullptr;
    m_torusKnotPosition = glm::vec3(130.0f,160.0f,250.0f);
    
    // metalballs
    m_pMetaballs = nullptr;
    m_metalballsPosition = glm::vec3(500.0f,500.0f,0.0f);
    
    // post processing
    m_currentPPFXMode = PostProcessingEffectMode::DefaultFrameBuffer;
    
    // hud
    m_enableHud = true;
    m_pFtFont = nullptr;
    
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
    delete m_pBarrel;
    delete m_pSphere;
    delete m_pCube;
    delete m_pWoodenBox;
    delete m_pTorus;
    delete m_pTorusKnot;
    delete m_pMetaballs;
    
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
    m_pLamp = new CCube(1.0f);
    m_pBarrel = new COpenAssetImportMesh;
    m_pSphere = new CSphere;
    m_pCube = new CCube(1.0f);
    m_pWoodenBox = new CCube(1.0f);
    m_pTorus = new CTorus(5.0f);
    m_pTorusKnot = new CTorusKnot;
    m_pMetaballs = new CMetaballs;
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
                                    path+"/textures/terrain/sand.png", // ambientMap 0
                                    path+"/textures/terrain/patchygrass.png", // normalMap 1
                                    path+"/textures/terrain/stone.png", // diffuseMap 2
                                    path+"/textures/terrain/snow.png", // specularMap 4
                                },
                                glm::vec3(0, 0, 0),
                                m_mapSize,
                                m_mapSize,
                                200.0f);
    m_pLamp->Create(path+"/textures/woodenBox/", "woodenBoxDiffuse.png");
    
    m_pBarrel->Load(path+"/models/barrel/barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
    
    m_pCube->Create(path+"/textures/woodenBox/", "woodenBoxDiffuse.png");
    
    m_pWoodenBox->Create(path+"/textures/woodenBox/",
    {   "woodDiffuse.jpg",          // ambientMap 0
        "woodNormal.jpg",           // normalMap 1
        "woodenBoxDiffuse.png",     // diffuseMap 2
        "woodenBoxSpecular.png"     // specularMap 4
    });
    
    // Create a sphere
    // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    m_pSphere->Create(path+"/textures/pixarLibrary/stone/", "Gray_granite_pxr128.tif", 50, 50);
    
    m_pTorus->Create(path+"/textures/3912Tex/", "3912-diffuse.jpg");
    
    m_pTorusKnot->Create(path+"/textures/pixarLibrary/metal/Round_mesh_pxr128.tif",
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
    m_pMetaballs->Create(100.0f, 10, 0, 32, path+"/textures/pixarLibrary/metal/Wire_screen_pxr128.tif");{
        m_pMetaballs->SetGridSize(50);
        CMarchingCubes::BuildTables();
    }
    
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
    InitialiseCamera(width, height, glm::vec3(100.0f, 430.0f, 300.0f));
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
