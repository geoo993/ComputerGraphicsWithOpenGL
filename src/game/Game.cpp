
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
    m_isOrthographicCamera = true;
    
    // materials
    m_parallaxHeightScale = 1.2f;
    m_materialShininess = 10.0f;
    m_materialUseTexture = true;
    m_materialColor = glm::vec4(Extensions::randRGB(), 1.0f);
    m_useFog = false;
    m_fogColor = glm::vec3(0.6f);
    
    m_uvTiling = 1.4f;
    m_magnitude = 0.3f;
    
    m_albedo = 1.0f;
    m_metallic = 1.0f;
    m_roughness = 1.0f;
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
    
    // HDR
    m_hdrName = "hrdlight";
    m_HDR = false;
    m_useBlinn = true;
    
    // Attenuation
    m_constant = 1.0f;
    m_linear = 0.022f;
    m_exponent = 0.0019f;
    
    // Dir Light
    m_dirName = "R_directionallight";
    m_useDir = true;
    m_dirColor = glm::vec3(0.95f, 0.9f, 1.0f);
    m_dirIntensity = 0.4f;
    m_directionalLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f),
    
    // Point Light
    m_pointName = "R_pointlight";
    m_usePoint = false;
    m_pointIntensity = 18.0f;
    
    m_pointLights = {
        std::make_tuple(glm::vec3(  100.0f,  40.0f,  250.0f     ), glm::vec4(  1.0f,  0.1f,  0.9f, 1.0f     )   ),      // 1
        std::make_tuple(glm::vec3(  326.0f,  -23.0f,  -32.0f    ), glm::vec4(  0.1f,  0.9f,  0.8f , 1.0f    )   ),      // 2
        std::make_tuple(glm::vec3(  -450.7f,  50.2f,  -150.0f   ), glm::vec4(  1.0f,  1.0f,  0.0f  , 1.0f   )   ),      // 3
        std::make_tuple(glm::vec3(  202.3f,  43.3f, -224.0f     ), glm::vec4(  0.01f, 0.96f, 0.0f , 1.0f    )   ),      // 4
        std::make_tuple(glm::vec3(  -204.0f,  12.0f, -200.0f    ), glm::vec4(  0.0f,  0.0f, 1.0f  , 1.0f    )   ),      // 5
        std::make_tuple(glm::vec3(  450.0f,  -50.0f, -233.0f    ), glm::vec4(  1.0f,  0.0f, 0.1f , 1.0f     )   ),      // 6
        std::make_tuple(glm::vec3(  320.0f,  70.0f, 350.0f      ), glm::vec4(  0.97f,  0.6f, 0.1f  , 1.0f   )   ),      // 7
        std::make_tuple(glm::vec3(  -600.0f,  10.0f, 370.0f     ), glm::vec4(  0.6f,  0.8f, 0.0f , 1.0f     )   ),      // 8
        std::make_tuple(glm::vec3(  -120.0f,  -50.0f, 233.0f    ), glm::vec4(  1.0f,  0.2f, 0.5f  , 1.0f    )   ),      // 9
        std::make_tuple(glm::vec3(  -2.0f,  800.0f, 30.0f   ), glm::vec4(  50.0f, 50.0f, 50.0f, 1.0f )   )       // 10
    };
    m_pointLightIndex = m_pointLights.size() - 1;
    
    // Spot Light
    m_spotName = "R_spotlight";
    m_useSpot = false;
    m_spotColor = glm::vec3(0.3f, 0.5f, 1.0f);;
    m_spotIntensity = 40.4f;
    m_spotCutOff = 22.5f;
    m_spotOuterCutOff = 28.0f;
    
    // screens
    m_pQuad = nullptr;
    
    // skybox
    m_pSkybox = nullptr;
    m_pEnvSkybox = nullptr;
    m_pIrrSkybox = nullptr;
    m_skyboxSize = (GLfloat)SKYBOX;
    m_skyboxNumber = int(Extensions::randomFloat(0.0f, 6.0f));
    m_changeSkybox = false;
    
    // terrain
    m_mapSize = (GLfloat)ZFAR;
    m_showTerrain = true;
    m_useTerrain = true;
    m_pPlanarTerrain = nullptr;
    m_pHeightmapTerrain = nullptr;
    m_heightMapMinHeight = 0.0f ;
    m_heightMapMaxHeight = 100.0f;
    
    //models
    m_teapot1 = nullptr;
    m_teapot2 = nullptr;
    m_teapot3 = nullptr;
    m_teapot4 = nullptr;
    m_teapot5 = nullptr;
    m_teapot6 = nullptr;
    m_teapot7 = nullptr;
    m_teapot8 = nullptr;
    m_teapot10 = nullptr;
    m_teapot15 = nullptr;
    m_teapot16 = nullptr;
    m_teapot17 = nullptr;
    m_teapot19 = nullptr;
    
    m_trolley = nullptr;
    m_lamborginhi = nullptr;
    
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
    m_pCube12 = nullptr;
    m_pCube13 = nullptr;
    m_pCube14 = nullptr;
    m_pInteriorBox = nullptr;
    
    // woodenBox
    m_pWoodenBox = nullptr;
    m_woodenBoxesPosition = {
        glm::vec3(  -186.0f,  193.0f,  -132.0f   ),
        glm::vec3(  -135.7f,  150.2f,  -222.0f   ),
        glm::vec3(  52.3f, 123.3f, -44.0f      ),
        glm::vec3(  -24.0f,  152.0f, -112.0f    ),
        glm::vec3(  220.0f,  220.0f, -133.0f    ),
        glm::vec3(  -152.3f, 263.3f, -74.0f  ),
        glm::vec3(  224.0f,  172.0f, 12.0f   ),
        glm::vec3(  -220.0f,  210.0f, 133.0f  ),
    };
    
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
    m_mouseX = 0.0f;
    m_mouseY = 0.0f;
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
    delete m_teapot3;
    delete m_teapot4;
    delete m_teapot5;
    delete m_teapot6;
    delete m_teapot7;
    delete m_teapot8;
    delete m_teapot9;
    delete m_teapot10;
    delete m_teapot15;
    delete m_teapot16;
    delete m_teapot17;
    delete m_teapot19;
    
    delete m_trolley;
    delete m_lamborginhi;
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
    
    delete m_pCube12;
    delete m_pCube13;
    delete m_pCube14;
    delete m_pInteriorBox;
    
    delete m_pLamp;
    delete m_pWoodenBox;
    
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
    UpdateAudio();
}

// Render scene method runs
void Game::Render()
{
    
    ChangePPFXScene( m_currentPPFXMode );
    
    // bind framebuffer
    BindPPFXFBO( m_currentPPFXMode );
    
    // Clear screen
    m_gameWindow->ClearBuffers(ClearBuffersType::COLORDEPTHSTENCIL);
    
    RenderScene();
    
    // set viewport and clear buffers
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
    InitialiseAudio(filepath);
    
    LoadShaderPrograms(filepath);
    LoadFrameBuffers(width, height);
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
