
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
    m_channelTime = 1.0f;
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
    m_parallaxHeightScale = 1.2f;
    m_materialShininess = 10.0f;
    m_materialUseTexture = true;
    m_materialColor = glm::vec4(Extensions::randRGB(), 1.0f);
    m_useFog = false;
    m_fogColor = glm::vec3(0.6f);
    
    m_uvTiling = 1.4f;
    m_magnitude = 0.3f;
    
    m_albedo = 0.5f;
    m_metallic = 0.9f;
    m_roughness = 0.1f;
    m_ao = 1.0f;
    m_useIrradiance = true;
    m_useIrradianceMap = false;
    
    //textures settings
    m_textures.reserve(50);
    
    // shader programs
    m_pShaderPrograms = nullptr;
    
    // lights
    m_pLamp = nullptr;
    m_ambient = 0.4f;
    m_diffuse = 2.5f;
    m_specular = 0.6f;
    m_exposure = 1.0f;
    m_gama = 1.0f;
    m_HDR = false;
    m_useBlinn = true;
    
    // Attenuation
    m_constant = 1.0f;
    m_linear = 0.022f;
    m_exponent = 0.0019f;

    // Dir Light
    m_useDir = true;
    m_dirColor = glm::vec3(1.0f, 1.0f, 1.0f);
    m_dirIntensity = 0.4f;
    m_directionalLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f),
    
    // Point Light
    m_usePoint = true;
    m_pointIntensity = 18.0f;
    m_pointLightPositionsIndex = 0;
    m_pointLightPositions = {
        glm::vec3(  100.0f,  40.0f,  50.0f      ),
        glm::vec3(  226.0f,  -83.0f,  -32.0f      ),
        glm::vec3(  -335.7f,  50.2f,  122.0f      ),
        glm::vec3(  202.3f,  43.3f, -224.0f      ),
        glm::vec3(  -204.0f,  12.0f, -200.0f    ),
        glm::vec3(  300.0f,  -120.0f, -233.0f      ),
        glm::vec3(  320.0f,  150.0f, 410.0f  ),
        glm::vec3(  20.0f,  10.0f, 413.0f  ),
        glm::vec3(  -120.0f,  -50.0f, 233.0f  ),
        glm::vec3(  -600.0f,  1500.0f, 130.0f )
    };
    
    m_pointLightColors = {
        glm::vec4(  1.0f,  0.1f,  0.9f, 1.0f      ),
        glm::vec4(  0.1f,  0.9f,  0.8f , 1.0f       ),
        glm::vec4(  1.0f,  1.0f,  0.0f  , 1.0f      ),
        glm::vec4(  0.01f, 0.96f, 0.0f , 1.0f       ),
        glm::vec4(  0.0f,  0.0f, 1.0f  , 1.0f    ),
        glm::vec4(  1.0f,  0.0f, 0.1f , 1.0f       ),
        glm::vec4(  0.97f,  0.6f, 0.1f  , 1.0f      ),
        glm::vec4(  0.6f,  0.8f, 0.0f , 1.0f       ),
        glm::vec4(  1.0f,  0.2f, 0.5f  , 1.0f      ),
        glm::vec4(  200.0f,  200.0f, 200.0f  , 1.0f )
    };
    
    // Spot Light
    m_useSpot = false;
    m_spotColor = glm::vec3(0.3f, 0.5f, 1.0f);;
    m_spotIntensity = 40.4f;
    m_spotCutOff = 22.5f;
    m_spotOuterCutOff = 28.0f;
    
    // PPFX
    m_ffaaOffset = 0.0f;
    m_changePPFXMode = false;
    m_prevPPFXMode = false;
    m_nextPPFXMode = false;
    
    // SSAO
    // generate sample kernel
    // ----------------------
    srand(glfwGetTime()); // initialize random seed
    m_ssaoBias, m_ssaoRadius, m_ssaoNoiseSize = 0.0f;
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
    m_pEnvSkybox = nullptr;
    m_pIrrSkybox = nullptr;
    m_skyboxSize = (GLfloat)SKYBOX;
    m_skyboxNumber = 0;
    m_changeSkybox = false;
    
    // terrain
    m_mapSize = (GLfloat)ZFAR;
    m_pPlanarTerrain = nullptr;
    m_pHeightmapTerrain = nullptr;
    m_heightMapMinHeight = 0.0f ;
    m_heightMapMaxHeight = 100.0f;
    
    //models
    m_teapot1 = nullptr;
    m_teapot2 = nullptr;
    m_vehicle = nullptr;
    
    //sphere object
    m_sphereRotation = 0.0f;
    m_pSpherePBR1 = nullptr;
    m_pSpherePBR2 = nullptr;
    m_pSpherePBR3 = nullptr;
    m_pSpherePBR4 = nullptr;
    m_pSpherePBR5 = nullptr;
    m_pSpherePBR6 = nullptr;
    m_pSpherePBR7 = nullptr;
    m_pSpherePBR8 = nullptr;
    m_pSpherePBR9 = nullptr;
    m_pSpherePBR10 = nullptr;
    m_pSpherePBR11 = nullptr;
    m_pSpherePBR12 = nullptr;
    m_pSpherePBR13 = nullptr;
    m_pSpherePBR14 = nullptr;
    m_pSpherePBR15 = nullptr;
    m_pSpherePBR16 = nullptr;
    m_pSpherePBR17 = nullptr;
    m_pSpherePBR18 = nullptr;
    m_pSpherePBR19 = nullptr;
    
    //cube object
    m_pCube1 = nullptr;
    m_pCube2 = nullptr;
    m_pCube3 = nullptr;
    m_pCube4 = nullptr;
    m_pCube5 = nullptr;
    m_pCube6 = nullptr;
    m_pCube7 = nullptr;
    m_pCube8 = nullptr;
    m_pCube9 = nullptr;
    m_pCube10 = nullptr;
    m_pCube11 = nullptr;
    m_pCube12 = nullptr;
    m_pCube13 = nullptr;
    m_pCube14 = nullptr;

    //torus object
    m_pTorus = nullptr;
    
    //torusknot object
    m_pTorusKnot = nullptr;
    
    // metalballs
    m_pMetaballs = nullptr;
    
    // hud
    m_enableHud = true;
    m_pFtFont = nullptr;
    
    // https://juiceboxinteractive.com/blog/color/
    m_textColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.7f);  // unhighlighed text
    m_guiColor = glm::vec4(0.16f, 0.20f, 0.43f, 0.6f); // sliding / selected
    m_guiBackgroundColor = glm::vec4(0.16f, 0.49f, 0.62f, 0.6f); // background
    m_guiHighlightedColor = glm::vec4(0.29f, 0.79f, 0.84f, 0.7f);  // highlighed text
    
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
    delete m_pEnvSkybox;
    delete m_pIrrSkybox;
    delete m_pPlanarTerrain;
    delete m_pHeightmapTerrain;
    delete m_teapot1;
    delete m_teapot2;
    delete m_vehicle;
    delete m_pSpherePBR1;
    delete m_pSpherePBR2;
    delete m_pSpherePBR3;
    delete m_pSpherePBR4;
    delete m_pSpherePBR5;
    delete m_pSpherePBR6;
    delete m_pSpherePBR7;
    delete m_pSpherePBR8;
    delete m_pSpherePBR9;
    delete m_pSpherePBR10;
    delete m_pSpherePBR11;
    delete m_pSpherePBR12;
    delete m_pSpherePBR13;
    delete m_pSpherePBR14;
    delete m_pSpherePBR15;
    delete m_pSpherePBR16;
    delete m_pSpherePBR17;
    delete m_pSpherePBR18;
    delete m_pSpherePBR19;
    
    delete m_pCube1;
    delete m_pCube2;
    delete m_pCube3;
    delete m_pCube4;
    delete m_pCube5;
    delete m_pCube6;
    delete m_pCube7;
    delete m_pCube8;
    delete m_pCube9;
    delete m_pCube10;
    delete m_pCube11;
    delete m_pCube12;
    delete m_pCube13;
    delete m_pCube14;
    
    delete m_pLamp;
    
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

void Game::PreRendering() {

    // update game timer
    UpdateSystemTime();
    UpdateGameTime();
    
    // update controls
    UpdateControls();
    
    // update audio
    //UpdateAudio();
}

// Render scene method runs
void Game::Render()
{
    ChangePPFXScene( m_currentPPFXMode );
    
    BindPPFXFBO( m_currentPPFXMode );
    
    // Clear buffers before rendering
    m_gameWindow->ClearBuffers(true);
    
    RenderScene();
     
    ResetFrameBuffer();
    
    // Post Processing Effects
    RenderPPFX( m_currentPPFXMode );
    
    // Draw the 2D graphics after the 3D graphics
    RenderHUD();
    
    // Draw controls GUI objects
    RenderControls();
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
    InitialiseCamera(width, height, glm::vec3(0.0f, 0.0f, 200.0f));
    //InitialiseAudio(filepath);
    
    LoadShaderPrograms(filepath);
    LoadResources(filepath);
    LoadTextures(filepath);
    LoadControls();
    
    m_gameManager->SetLoaded(true); // everything has loaded
    m_gameWindow->PreRendering();
    m_gameManager->SetActive(true); // game is now going to be active, or activate application
    
    // Set frame viewport at the beginning
    m_gameWindow->SetViewport();
    
    while ( !m_gameWindow->ShouldClose() ){
        
        if (m_gameManager->IsActive()) {
            GameLoop();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(60)); // Do not consume processor power if application isn't active
        }

        // Poll IO events (keys pressed/released, mouse moved etc.)
        m_gameWindow->PostRendering();
        
        // Swap buffers right after rendering all, this is to show the current rendered image
        m_gameWindow->SwapBuffers();
    }
    
    RemoveControls();
    
    m_gameWindow->DestroyWindow();
    
    return;
}
