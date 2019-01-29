
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
    m_terrainPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    
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
    delete m_pBarrel;
    
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
    m_pBarrel = new COpenAssetImportMesh;
    
}

void Game::InitialiseFrameBuffers(const GLuint &width , const GLuint &height){
    
}

void Game::InitialiseCamera(const GLuint &width, const GLuint &height, const glm::vec3 & position){
    
    // Set the orthographic and perspective projection matrices based on the image size
    m_pCamera->Create(position,                // position
                      glm::vec3(0.0f, 1.0f, 0.0f),       // worldUp
                      PITCH,                            // pitch
                      YAW,                              // yaw
                      FOV,                              // fieldOfView
                      (GLfloat)width,                   // width
                      (GLfloat)height,                  // height
                      ZNEAR,                            // zNear
                      ZFAR                              // zFar
                      );
    
    glm::vec3 pos = position;
    glm::vec3 view = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upV = glm::vec3(0.0f, 1.0f, 0.0f);
    m_pCamera->Set(pos, view, upV);
    
}

void Game::InitialiseAudio(const std::string &path){
    
    ////add audio files from ////http://freemusicarchive.org/music/Kai_Engel/
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_02_-_Better_Way.mp3");
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_03_-_Brooks.mp3");
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_03_-_Realness.mp3");
    m_audioFiles.push_back(path+"/audio/Kai_Engel_-_08_-_Oecumene_Sleeps.mp3");
    m_audioFiles.push_back(path+"/audio/Marcel_Pequel_-_06_-_Six.mp3");
    
    //// Initialise audio and play background music
    m_pAudio->Initialise();
    m_pAudio->LoadMusicStream(m_audioFiles[m_audioNumber].c_str());
    m_pAudio->PlayMusicStream();
    
}

void Game::LoadFromResources(const std::string &path)
{
   
    // font
    m_pFtFont->LoadFont(path+"/fonts/Arial.ttf", 32);
    
    // Create the skybox
    // Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
    m_pSkybox->Create(m_mapSize, path, m_skyboxNumber);
    
    // Create the planar terrain
    m_pPlanarTerrain->Create(path+"/textures/terrain/grassfloor.jpg", m_mapSize, m_mapSize, 50.0f, 50); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
    
}

void Game::RenderScene(){
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    
    // uncomment if stencil buffer is not used
    glStencilMask(0x00); // make sure we don't update the stencil buffer while drawing the floor
    
    // start by deleting current skybox and create new one
    if (m_changeSkybox == true) {
        m_pSkybox->Release();
        m_pSkybox->Create(m_mapSize, m_gameManager.GetResourcePath(), m_skyboxNumber);
        //cout << "Changing skybox to " << m_skyboxNumber << endl;
        m_changeSkybox = false;
    }
    
    if (m_changeAudio == true) {
        m_pAudio->StopAll();
        m_pAudio->LoadMusicStream(m_audioFiles[m_audioNumber].c_str());
        m_pAudio->PlayMusicStream();
        m_changeAudio = false;
    }
    
    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    CShaderProgram *pSkyBoxProgram = (*m_pShaderPrograms)[1];
    pSkyBoxProgram->UseProgram();
    pSkyBoxProgram->SetUniform("cubeMapTex", CUBEMAPTEXTUREUNIT);
    pSkyBoxProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pSkyBoxProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    glm::mat4 inverseViewMatrix = glm::inverse(m_pCamera->GetViewMatrix());
    pSkyBoxProgram->SetUniform("matrices.inverseViewMatrix", inverseViewMatrix);
    
    glm::vec3 vEye = m_pCamera->GetPosition();
    m_pSkybox->transform.SetIdentity();
    m_pSkybox->transform.Translate(vEye);
    
    glm::mat4 skyBoxModel = m_pSkybox->transform.GetModel();
    pSkyBoxProgram->SetUniform("matrices.modelMatrix", skyBoxModel);
    pSkyBoxProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(skyBoxModel));
    m_pSkybox->Render(CUBEMAPTEXTUREUNIT);
    glDepthFunc(GL_LESS);
    
    
    // Use the main shader program
    CShaderProgram *pBasicProgram = (*m_pShaderPrograms)[2];
    pBasicProgram->UseProgram();
    pBasicProgram->SetUniform("bUseTexture", true);
    pBasicProgram->SetUniform("sampler0", 0);
    
    // Set the projection matrix
    pBasicProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
    pBasicProgram->SetUniform("matrices.viewMatrix", m_pCamera->GetViewMatrix());
    // Render the planar terrain
    m_pPlanarTerrain->transform.SetIdentity();
    m_pPlanarTerrain->transform.Translate(m_terrainPosition);
    glm::mat4 terrainModel = m_pPlanarTerrain->transform.GetModel();
    pBasicProgram->SetUniform("matrices.modelMatrix", terrainModel);
    pBasicProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(terrainModel));
    m_pPlanarTerrain->Render();
}

// Render scene method runs
void Game::RenderPPFX(const PostProcessingEffectMode &mode)
{
    
    
}

// Render scene method runs
void Game::Render()
{

    RenderScene();
    
    // It is useful to switch back to the default framebuffer for this to easily see your results.
    // Unbind to render to our default framebuffer or switching back to the default buffer at 0.
    // To make sure all rendering operations will have a visual impact on the main window we need to make the default framebuffer active again by binding to 0:
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    m_gameWindow.SetViewport();
    
    // Post Processing Effects
    RenderPPFX( m_currentPPFXMode );
    
    // Draw the 2D graphics after the 3D graphics
    RenderHUD();
    
    // Swap buffers right after rendering all, this is to show the current rendered image
    m_gameWindow.SwapBuffers();
}


// Update method runs repeatedly with the Render method
void Game::Update()
{
    
    // Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
    m_pCamera->Update(m_gameWindow.GetWindow(), m_deltaTime, keyPressedCode, true, m_enableMouseMovement);
    
    MouseControls(mouseButton, mouseAction);
    KeyBoardControls(keyPressedCode, keyReleasedCode, keyPressedAction);
    
    m_pAudio->Update();
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
    // Variable timer
    m_pGameTimer->Start();
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
    InitialiseCamera(width, height, glm::vec3(1.0f, 20.0f, 1.0f));
    InitialiseAudio(filepath);
    
    CreateShaderPrograms(filepath);
    LoadFromResources(filepath);
    
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
