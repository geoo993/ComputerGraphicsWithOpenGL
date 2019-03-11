
#include "Game.h"


// Constructor
Game::Game()
{
    // game window
    m_gameWindow = nullptr;
    m_gameManager = nullptr;
    
    // game timer
    m_pGameTimer = nullptr;
    m_date = glm::vec4(1.0f);
    m_timeInSeconds = 0.0f;
    m_timeInMilliSeconds = 0.0f;
    m_timePerSecond = 0.0f;
    m_channelTime = 0.0f;
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
    m_linear = 0.022f;
    m_exponent = 0.0019f;

    // Dir Light
    m_useDir = false;
    m_dirColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_dirIntensity = 0.8f;
    m_directionalLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f),
    
    // Point Light
    m_usePoint = true;
    m_pointIntensity = 40.5f;
    m_pointLightPositionsIndex = 0;
    m_pointLightPositions = {
        glm::vec3(  100.0f,  400.0f,  50.0f      ),
        glm::vec3(  226.0f,  583.0f,  -32.0f      ),
        glm::vec3(  -335.7f,  550.2f,  122.0f      ),
        glm::vec3(  102.3f, 663.3f, -44.0f      ),
        glm::vec3(  -104.0f,  612.0f, -200.0f    ),
        glm::vec3(  300.0f,  620.0f, -233.0f      ),
        glm::vec3(  320.0f,  310.0f, 410.0f  ),
        glm::vec3(  20.0f,  410.0f, 413.0f  ),
        glm::vec3(  -120.0f,  350.0f, 233.0f  ),
        glm::vec3(  -600.0f,  2000.0f, 130.0f )
    };
    
    m_pointLightColors = {
        glm::vec4(  1.0f,  0.1f,  0.9f, 1.0f      ),
        glm::vec4(  0.1f,  0.9f,  0.8f , 1.0f       ),
        glm::vec4(  1.0f,  1.0f,  0.0f  , 1.0f      ),
        glm::vec4(  0.1f, 0.9f, 0.0f , 1.0f       ),
        glm::vec4(  0.0f,  0.0f, 1.0f  , 1.0f    ),
        glm::vec4(  1.0f,  0.0f, 0.1f , 1.0f       ),
        glm::vec4(  0.97f,  0.6f, 0.1f  , 1.0f      ),
        glm::vec4(  0.6f,  0.8f, 0.0f , 1.0f       ),
        glm::vec4(  1.0f,  0.2f, 0.5f  , 1.0f      ),
        glm::vec4(  10.0f,  10.0f, 10.0f  , 1.0f      )
    };
    
    // Spot Light
    m_useSpot = false;
    m_spotColor = glm::vec3(0.3f, 0.5f, 1.0f);;
    m_spotIntensity = 40.4f;
    m_spotCutOff = 22.5f;
    m_spotOuterCutOff = 28.0f;
    
    // SSAO
    // generate sample kernel
    // ----------------------
    srand(glfwGetTime()); // initialize random seed
    m_ssaoKernelSamples = 64;
    for (GLuint i = 0; i < m_ssaoKernelSamples; ++i)
    {
        glm::vec3 sample(Extensions::randFloat() * 2.0f - 1.0f,
                         Extensions::randFloat() * 2.0f - 1.0f,
                         Extensions::randFloat());
        sample = glm::normalize(sample);
        sample *= Extensions::randFloat();
        float scale = float(i) / m_ssaoKernelSamples;
        
        // scale samples s.t. they're more aligned to center of kernel
        scale = Extensions::interpolate(0.1f, 1.0f, scale * scale);
        sample *= scale;
        m_ssaoKernel.push_back(sample);
    }
    
    // generate noise texture
    // ----------------------
    m_ssaoNoiseSamples = 16;
    for (GLuint i = 0; i < m_ssaoNoiseSamples; i++)
    {
        glm::vec3 noise(Extensions::randFloat() * 2.0f - 1.0f,
                        Extensions::randFloat() * 2.0f - 1.0f,
                        0.0f); // rotate around z-axis (in tangent space)
        m_ssaoNoise.push_back(noise);
    }
    
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
    m_pFireBallSphere = nullptr;
    m_spherePosition = glm::vec3(130.0f,350.0f,-450.0f);
    
    //cube object
    m_pCube = nullptr;
    m_pInteriorBox = nullptr;
    m_pParallaxCube = nullptr;
    m_pChromaticAberrationCube = nullptr;
    m_cubePosition = glm::vec3(20.0f,120.0f, -50.0f);
    
    // woodenBox
    m_pWoodenBox = nullptr;
    m_woodenBoxesColor = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
    m_woodenBoxesUseTexture = true;
    m_woodenBoxesPosition = {
        glm::vec3(  -186.0f,  593.0f,  -132.0f   ),
        glm::vec3(  -135.7f,  550.2f,  322.0f   ),
        glm::vec3(  52.3f, 523.3f, -44.0f      ),
        glm::vec3(  -24.0f,  452.0f, -112.0f    ),
        glm::vec3(  220.0f,  620.0f, -133.0f    ),
        glm::vec3(  -152.3f, 663.3f, -74.0f  ),
        glm::vec3(  224.0f,  572.0f, 12.0f   ),
        glm::vec3(  -220.0f,  610.0f, 133.0f  ),
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
    m_mouseX, m_mouseY = 0.0;
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
    delete m_pInteriorBox;
    delete m_pParallaxCube;
    delete m_pChromaticAberrationCube;
    delete m_pWoodenBox;
    delete m_pNanosuit;
    delete m_pTorus;
    delete m_pTorusKnot;
    delete m_pMetaballs;
    delete m_pQuad;
    delete m_pFireBallSphere;
    
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

void Game::PreRendering() {
    
    // update game timer
    UpdateSystemTime();
    UpdateGameTime();
    
    // update controls
    UpdateControls();
    
    // update audio
    UpdateAudio();
}

// Render scene method runs
void Game::Render()
{

    ActivateFBO( m_currentPPFXMode );
    
    // Clear buffers before rendering
    m_gameWindow->ClearBuffers();
    
    RenderScene();
    
    ResetFrameBuffer();
    
    // Post Processing Effects
    RenderPPFX( m_currentPPFXMode );
    
    // Draw the 2D graphics after the 3D graphics
    RenderHUD();
    
    // Draw controls GUI objects
    RenderControls();
    
    // Swap buffers right after rendering all, this is to show the current rendered image
    m_gameWindow->SwapBuffers();
}

void Game::PostRendering() {
    ResetCamera(m_deltaTime);
    ClearControls();
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

void Game::Execute(const std::string &filepath, const GLuint &width, const GLuint &height)
{

    InitialiseResources();
    InitialiseGameWindow("OpenGL Window", filepath, width, height);
    InitialiseFrameBuffers(width, height);
    InitialiseCamera(width, height, glm::vec3(220.0f, 500.0f, -400.0f));
    InitialiseAudio(filepath);
    
    LoadShaderPrograms(filepath);
    LoadResources(filepath);
    LoadTextures(filepath);
    LoadControls();
    
    m_gameManager->SetLoaded(true); // everything has loaded
    m_gameWindow->PreRendering();
    m_gameManager->SetActive(true); // game is now going to be active, or activate application
    
    while ( !m_gameWindow->ShouldClose() ){
        
        if (m_gameManager->IsActive()) {
            GameLoop();
        }else{
            std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Do not consume processor power if application isn't active
        }
        
        m_gameWindow->PostRendering();
    }
    
    RemoveControls();
    
    m_gameWindow->DestroyWindow();
    
    return;
}
