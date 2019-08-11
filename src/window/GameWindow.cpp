#include "GameWindow.h"

#define SIMPLE_OPENGL_CLASS_NAME "simple_openGL_class_name"

CGameWindow::CGameWindow(const CGameWindow &other) {
    this -> m_appName = other.m_appName;
    this -> m_width = other.m_width;
    this -> m_height = other.m_height;
    this -> m_fullscreen = other.m_fullscreen;
}

CGameWindow &CGameWindow::operator=(const CGameWindow &other){
    this -> m_appName = other.m_appName;
    this -> m_width = other.m_width;
    this -> m_height = other.m_height;
    this -> m_fullscreen = other.m_fullscreen;
    return *this;
}

CGameWindow::CGameWindow() {
    this -> m_appName = "";
    this -> m_width = 0;
    this -> m_height = 0;
    this -> m_fullscreen = false;
}

void CGameWindow::Set(const std::string &appName, const int &w, const int &h, const bool &fullscreen){
    m_appName = appName;
    m_width = w;
    m_height = h;
    m_fullscreen = fullscreen;
}

// Initialise GLEW and create the real game window
void CGameWindow::CreateWindow(const std::string &appName,
                               const int &w,
                               const int &h,
                               const bool &fullscreen)
{
    Set(appName, w, h, fullscreen);
    
    if(!InitGLFW()){
        return;
    }
    
    CreateGameWindow(m_appName);
    
    return;
}


// Create a dummy window, intialise GLEW, and then delete the dummy window
bool CGameWindow::InitGLFW()
{
	static bool bGlewInitialized = false;
    if(bGlewInitialized) {
        return true;
    }

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return false;
    }

    GLFWwindow* fakeWindow = glfwCreateWindow(m_width, m_height, m_appName.c_str(), nullptr, nullptr); // Windowed

    if (fakeWindow == nullptr) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(fakeWindow);

    bool bResult = true;

    if(!bGlewInitialized)
    {
        if(glewInit() != GLEW_OK){
            bResult = false;
            throw std::runtime_error("Couldn't initialize GLEW!");
        }
        bGlewInitialized = true;
    }

    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(fakeWindow);
    glfwTerminate();

	return bResult;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// Create the game window
void CGameWindow::CreateGameWindow(const std::string &appName)
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
        return;
    }

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);  // Multi Sample Anti-Aliasing  for smooth edges.
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    //glfwWindowHint(GLFW_STENCIL_BITS, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//We don't want the old OpenGL and want the core profile
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    if (m_fullscreen){
        m_window = glfwCreateWindow(m_width, m_height, appName.c_str(), glfwGetPrimaryMonitor(), nullptr);//// Windowed
    }else{
        m_window = glfwCreateWindow(m_width, m_height, appName.c_str(), nullptr, nullptr); // Fullscreen
    }

    glfwGetWindowSize(m_window, &m_width, &m_height);

    if (m_window == nullptr) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return ;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSwapInterval(1); ////<-- force interval (not guaranteed to work with all graphics drivers)

    //start GLEW extension handler
    if(InitOpenGL()){
        SetCursorVisible(false);
        CenterTheWindow();
        return;
    }

    return;

}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

// Initialise OpenGL, including the pixel format descriptor and the OpenGL version
bool CGameWindow::InitOpenGL()
{

    bool bError = false;

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        throw std::runtime_error("glewInit failed");
        bError = true;
    }

    if(!GLEW_VERSION_3_2){
        throw std::runtime_error("OpenGL 3.2 API is not available.");
        bError = true;
    }

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer as a string
    const GLubyte* version = glGetString(GL_VERSION); // "graphics griver version as a string
    const GLubyte* vendor = glGetString(GL_VENDOR); // vendor as a string
    const GLubyte* shaderLanguage = glGetString(GL_SHADING_LANGUAGE_VERSION); // shader lang as a string
    // print out some info about the graphics drivers
    std::cout << "OpenGL or Graphics Driver version supported: " << version << std::endl;
    std::cout << "GLSL version: " << shaderLanguage << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "Vendor: " << vendor << std::endl;

    //3. Check for specific functionality
    if (GLEW_ARB_vertex_array_object){
        printf("genVertexArrays supported\n");
    }
    if (GLEW_APPLE_vertex_array_object){
        printf("genVertexArrayAPPLE supported\n");
    }

    if(bError)
    {
        glfwSetErrorCallback(error_callback);
        return bError;
    }

    GLint MaxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureUnits);
    std::cout << "There are "<< MaxTextureUnits << " texture units supported by GPU. " << std::endl;
	return bError;
}

void CGameWindow::SetInputs(const GLFWcursorenterfun &cbfunEnter,
                            const GLFWcursorposfun &cbfunMouseMove,
                            const GLFWmousebuttonfun &cbfunMouseClick,
                            const GLFWscrollfun &cbfunMouseScroll,
                            const GLFWkeyfun &cbfunKey){
    
    //glfwSetWindowSizeCallback(m_window, ReshapeWindow);
    //glfwSetWindowShouldClose(m_window, GLUS_TRUE);
    
    glfwSetCursorEnterCallback(m_window, cbfunEnter);
    glfwSetCursorPosCallback(m_window, cbfunMouseMove);
    glfwSetMouseButtonCallback(m_window, cbfunMouseClick);
    glfwSetScrollCallback(m_window, cbfunMouseScroll);
    glfwSetKeyCallback(m_window, cbfunKey);
    
    // https://www.youtube.com/watch?v=L2aiuDDFNIk
    // what sticky keys is doing is it makes sure that the keys or keyboard events are polled, this means glfwPollEvents will handle these events
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);
    
    SetCursorVisible(true);
}

void CGameWindow::SetCursorVisible( const bool &isVisible )
{
    if( m_window == nullptr )
        return;

    // https://www.youtube.com/watch?v=EE5cS8EMT78
    // tell GLFW to capture our mouse
    if( isVisible ){
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //assert(glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL);
    }else{
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        //assert(glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN);
    }

    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // to remove cursor
}

//Check whether window should close
bool CGameWindow::ShouldClose(){
    return glfwWindowShouldClose(m_window);
}

// https://stackoverflow.com/questions/11335301/glfw-get-screen-height-width
void CGameWindow::CenterTheWindow(){
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(m_window, (mode->width - m_width) / 2, (mode->height - m_height) / 2);
}

//Get the Current framebuffer Size in pixels and Set the Viewport to it
void CGameWindow::SetViewport(){
    //glfwGetWindowSize(window, &windowWidth, &windowHeight);

    GLint width, height;

    // returns the framebuffer size, not the window size.
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport( 0, 0, width, height);

}

//Get the Current framebuffer Size in pixels and Set the Viewport to it
void CGameWindow::SetViewport(const int & width, const int & height){

    glViewport( 0, 0, width, height);
}


void CGameWindow::PreRendering(){
    
    // configure global opengl states
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color to black
    
    /* // https://www.khronos.org/opengl/wiki/Multisampling
     Multisampling, also known as multisample antialiasing (MSAA), is one method for achieving full-screen antialiasing (FSAA). With multisampling, each pixel at the edge of a polygon is sampled multiple times. For each sample-pass, a slight offset is applied to all screen coordinates.
     */
    
    /*   Multisample Anti-Aliasing  https://learnopengl.com/#!Advanced-OpenGL/Anti-Aliasing
     There are quite a few techniques out there called anti-aliasing techniques that fight exactly this aliasing (jagged edges) behavior to produce more smooth edges.
     
     At first we had a technique called super sample anti-aliasing (SSAA) that temporarily used a much higher resolution to render the scene in (super sampling) and when the visual output is updated in the framebuffer, the resolution was downsampled back to the normal resolution. This extra resolution was used to prevent these jagged edges. While it did provide us with a solution to the aliasing problem, it came with a major performance drawback since we had to draw a lot more fragments than usual. This technique therefore only had a short glory moment.
     
     This technique did gave birth to a more modern technique called multisample anti-aliasing or MSAA that borrows from the concepts behind SSAA while implementing a much more efficient approach.
     
     If we want to use MSAA in OpenGL we need to use a color buffer that is able to store more than one color value per pixel (since multisampling requires us to store a color per sample point). We thus need a new type of buffer that can store a given amount of multisamples and this is called a multisample buffer.
     
     Most windowing systems are able to provide us a multisample buffer instead of a default color buffer. GLFW also gives us this functionality and all we need to do is hint GLFW that we'd like to use a multisample buffer with N samples instead of a normal color buffer by calling glfwWindowHint before creating the window:
     
     
     glfwWindowHint(GLFW_SAMPLES, 4);
     
     When we now call glfwCreateWindow the rendering window is created, this time with a color buffer containing 4 subsamples per screen coordinate. GLFW also automatically creates a depth and stencil buffer with 4 subsamples per pixel. This does mean that the size of all the buffers is increased by 4.
     
     Now that we asked GLFW for multisampled buffers we need to enable multisampling by calling glEnable and enabling GL_MULTISAMPLE. On most OpenGL drivers, multisampling is enabled by default so this call is then a bit redundant, but it's usually a good idea to enable it anyways. This way all OpenGL implementations have multisampling enabled.
     
     
     glEnable(GL_MULTISAMPLE);
     Once the default framebuffer has multisampled buffer attachments, all we need to do to enable multisampling is just call glEnable and we're done. Because the actual multisampling algorithms are implemented in the rasterizer in your OpenGL drivers there's not much else we need to do.
     
     */
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_TEXTURE_2D);
    
    // https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
    glClearDepth(1.0f); // Set background depth to farthest
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    
    //https://learnopengl.com/#!Advanced-OpenGL/Depth-testing
    // Depth testing is done in screen space after the fragment shader has run (and after stencil testing has run). The screen space coordinates relate directly to the viewport defined by OpenGL's glViewport function and can be accessed via GLSL's built-in gl_FragCoord variable in the fragment shader. The x and y components of gl_FragCoord represent the fragment's screen-space coordinates (with (0,0) being the bottom-left corner). The gl_FragCoord also contains a z-component which contains the actual depth value of the fragment. This z value is the value that is compared to the depth buffer's content.
    // Depth testing is disabled by default so to enable depth testing we need to enable it with the GL_DEPTH_TEST option:
    glEnable(GL_DEPTH_TEST); // enable depth-testing, only draw fragments with a depth value of 1.
    //GL_DEPTH_TEST is to be enabled to avoid ugly artifacts that depend on the angle of view and drawing order (otherwise ie. if back of the cube is drawn last, if will appear "above" the front of the cube).
    //So yes, both GL_CULL_FACE and GL_DEPTH_TEST should be enabled.
    
    //OpenGL allows us to disable writing to the depth buffer by setting its depth mask to GL_FALSE: glDepthMask(GL_FALSE).
    // Note that this only has effect if depth testing is enabled.
    glDepthMask(GL_FALSE);
        
    //OpenGL allows us to modify the comparison operators it uses for the depth test. This allows us to control when OpenGL should pass or discard fragments and when to update the depth buffer. We can set the comparison operator (or depth function) by calling glDepthFunc:
    
    /*
     glEnable(GL_DEPTH_CLAMP);
     // When Using GL_DEPTH_CLAMP change the depth test from the default GL_LESS to GL_LEQUAL. This is cause when opengl clamps a poligon, it puts the poligon exactly in the far plane, so z=1 (ranging from -1 to 1). And the depth of the pixels of the polygon is 1 (from 0 to 1). And when you clear the depth buffer you fill it with 1. So the depth func must be GL_LEQUAL.
     glDepthFunc(GL_LEQUAL);
     */
    
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    //The function accepts several comparison operators that are listed in the table below:
    /*
     Function    Description
     GL_ALWAYS    The depth test always passes.
     GL_NEVER    The depth test never passes.
     GL_LESS    Passes if the fragment's depth value is less than the stored depth value.
     GL_EQUAL    Passes if the fragment's depth value is equal to the stored depth value.
     GL_LEQUAL    Passes if the fragment's depth value is less than or equal to the stored depth value.
     GL_GREATER    Passes if the fragment's depth value is greater than the stored depth value.
     GL_NOTEQUAL    Passes if the fragment's depth value is not equal to the stored depth value.
     GL_GEQUAL    Passes if the fragment's depth value is greater than or equal to the stored depth value.
     */
    
    // OpenGL allows us to disable writing to the depth buffer by setting its depth mask to GL_FALSE:
    // glDepthMask(GL_FALSE);
    
    
    
    /* Why we have znear and zfar
     
     The depth buffer contains depth values between 0.0 and 1.0 and it compares its content with z-value of all the objects in the scene as seen from the viewer. These z-values in view space can be any value between the projection frustum's near and far value. We thus need some way to transform these view-space z-values to the range of [0,1] and one way is to linearly transform them to the [0,1] range.
     
     */
    
    /*
     Once the fragment shader has processed the fragment a so called stencil test is executed that, just like the depth test, has the possibility of discarding fragments
     Then the remaining fragments get passed to the depth test that could possibly discard even more fragments. The stencil test is based on the content of yet another buffer called the stencil buffer that we're allowed to update during rendering to achieve interesting effects.
     
     When using stencil buffers you can get as crazy as you like, but the general outline is usually as follows:
     
     - Enable writing to the stencil buffer.
     - Render objects, updating the content of the stencil buffer.
     - Disable writing to the stencil buffer.
     - Render (other) objects, this time discarding certain fragments based on the content of the stencil buffer.
     
     */
    
    // https://learnopengl.com/#!Advanced-OpenGL/Stencil-testing
    // You can enable stencil testing by enabling GL_STENCIL_TEST. From that point on, all rendering calls will influence the stencil buffer in one way or another
    glEnable(GL_STENCIL_TEST);
    
    /*
     Also, just like the depth testing's glDepthMask function, there is an equivalent function for the stencil buffer. The function glStencilMask allows us to set a bitmask that is ANDed with the stencil value about to be written to the buffer. By default this is set to a bitmask of all 1s unaffecting the output, but if we were to set this to 0x00 all the stencil values written to the buffer end up as 0s. This is equivalent to depth testing's glDepthMask(GL_FALSE):
     
     */
    
    // glStencilMask(0xFF); // each bit is written to the stencil buffer as is
    // glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
    // Most of the cases you'll just be writing 0x00 or 0xFF as the stencil mask, but it's good to know there are options to set custom bit-masks.
    
    
    // There are a total of two functions we can use to configure stencil testing: glStencilFunc and glStencilOp.
    
    /*
     The glStencilFunc(GLenum func, GLint ref, GLuint mask) has three parameters:
     - func: sets the stencil test function. This test function is applied to the stored stencil value and the glStencilFunc's ref value. Possible options are: GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL and GL_ALWAYS. The semantic meaning of these is similar to the depth buffer's functions.
     - ref: specifies the reference value for the stencil test. The stencil buffer's content is compared to this value.
     - mask: specifies a mask that is ANDed with both the reference value and the stored stencil value before the test compares them. Initially set to all 1s.
     glStencilFunc(GL_EQUAL, 1, 0xFF);
     */
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // uncomment this for outline
    
    /*
     The glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) contains three options of which we can specify for each option what action to take:
     
     - sfail: action to take if the stencil test fails.
     - dpfail: action to take if the stencil test passes, but the depth test fails.
     - dppass: action to take if both the stencil and the depth test pass.
     
     Then for each of the options you can take any of the following actions:
     
     Action    Description
     GL_KEEP    The currently stored stencil value is kept.
     GL_ZERO    The stencil value is set to 0.
     GL_REPLACE    The stencil value is replaced with the reference value set with glStencilFunc.
     GL_INCR    The stencil value is increased by 1 if it is lower than the maximum value.
     GL_INCR_WRAP    Same as GL_INCR, but wraps it back to 0 as soon as the maximum value is exceeded.
     GL_DECR    The stencil value is decreased by 1 if it is higher than the minimum value.
     GL_DECR_WRAP    Same as GL_DECR, but wraps it to the maximum value if it ends up lower than 0.
     GL_INVERT    Bitwise inverts the current stencil buffer value.
     
     */
    
    // By default the glStencilOp function is set to (GL_KEEP, GL_KEEP, GL_KEEP) so whatever the outcome of any of the tests, the stencil buffer keeps its values. The default behavior does not update the stencil buffer, so if you want to write to the stencil buffer you need to specify at least one different action for any of the options.
    // glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    // If any of the tests fail we do nothing, we simply keep the currently stored value that is in the stencil buffer. If both the stencil test and the depth test succeed however, we want to replace the stored stencil value with the reference value set via glStencilFunc which we later set to 1.
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    
    /* Object Outlining technique
     
     Object outlining does exactly what it says it does. For each object (or only one) we're creating a small colored border around the (combined) objects. This is a particular useful effect when you want to select units in a strategy game for example and need to show the user which of the units were selected. The routine for outlining your objects is as follows:
     
     1- Set the stencil func to GL_ALWAYS before drawing the (to be outlined) objects, updating the stencil buffer with 1s wherever the objects' fragments are rendered.
     2- Render the objects.
     3- Disable stencil writing and depth testing.
     4- Scale each of the objects by a small amount.
     5- Use a different fragment shader that outputs a single (border) color.
     6- Draw the objects again, but only if their fragments' stencil values are not equal to 1.
     7- Enable stencil writing and depth testing again.
     */
    
    
    // https://learnopengl.com/#!Advanced-OpenGL/Blending
    //If you want blending to occur after the texture has been applied, then use the OpenGL blending feature. Use this: glEnable (GL_BLEND);
    // To render images with different levels of transparency we have to enable blending. Like most of OpenGL's functionality we can enable blending by enabling GL_BLEND:
    glEnable (GL_BLEND);
    
    /*
     Blending in OpenGL is done with the following equation:
     
     C¯result   =   C¯source    ∗   Fsource +    C¯destination  ∗ Fdestination
     
     C¯source:          the source color vector. This is the color vector that originates from the texture.
     C¯destination:     the destination color vector. This is the color vector that is currently stored in the color buffer.
     Fsource:           the source factor value. Sets the impact of the alpha value on the source color.
     Fdestination:      the destination factor value. Sets the impact of the alpha value on the destination color.
     
     After the fragment shader has run and all the tests have passed, this blend equation is let loose on the fragment's color output and with whatever is currently in the color buffer (previous fragment color stored before the current fragment). The source and destination colors will automatically be set by OpenGL, but the source and destination factor can be set to a value of our choosing.
     
     //You might want to use the alpha values that result from texture mapping in the blend function. If so, (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA) is always a good function to start with.
     // The glBlendFunc(GLenum sfactor, GLenum dfactor) function expects two parameters that set the option for the source and destination factor. OpenGL defined quite a few options for us to set of which we'll list the most common options below. Note that the constant color vector C¯constantC¯constant can be separately set via the glBlendColor function.
     
     Option                         Value
     GL_ZERO                        Factor is equal to 00.
     GL_ONE                         Factor is equal to 11.
     GL_SRC_COLOR                   Factor is equal to the source color vector C¯sourceC¯source.
     GL_ONE_MINUS_SRC_COLOR         Factor is equal to 11 minus the source color vector: 1−C¯source1−C¯source.
     GL_DST_COLOR                   Factor is equal to the destination color vector C¯destinationC¯destination
     GL_ONE_MINUS_DST_COLOR         Factor is equal to 11 minus the destination color vector: 1−C¯destination1−C¯destination.
     GL_SRC_ALPHA                   Factor is equal to the alphaalpha component of the source color vector C¯sourceC¯source.
     GL_ONE_MINUS_SRC_ALPHA         Factor is equal to 1−alpha1−alpha of the source color vector C¯sourceC¯source.
     GL_DST_ALPHA                   Factor is equal to the alphaalpha component of the destination color vector C¯destinationC¯destination.
     GL_ONE_MINUS_DST_ALPHA         Factor is equal to 1−alpha1−alpha of the destination color vector C¯destinationC¯destination.
     GL_CONSTANT_COLOR              Factor is equal to the constant color vector C¯constantC¯constant.
     GL_ONE_MINUS_CONSTANT_COLOR    Factor is equal to 11 - the constant color vector C¯constantC¯constant.
     GL_CONSTANT_ALPHA              Factor is equal to the alphaalpha component of the constant color vector C¯constantC¯constant.
     GL_ONE_MINUS_CONSTANT_ALPHA    Factor is equal to 1−alpha1−alpha of the constant color vector C¯constantC¯constant.
     
     */
    // To get the blending result we have to take the alpha of the source color vector for the source factor and 1−alpha for the destination factor. This translates to the glBlendFunc as follows:
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // It is also possible to set different options for the RGB and alpha channel individually using glBlendFuncSeparate:
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);  // This function sets the RGB components as we've set them previously, but only lets the resulting alpha component be influenced by the source's alpha value.
    
    
    /*
     OpenGL gives us even more flexibility by allowing us to change the operator between the source and destination part of the equation. Right now, the source and destination components are added together, but we could also subtract them if we want. glBlendEquation(GLenum mode) allows us to set this operation and has 3 possible options:
     
     - GL_FUNC_ADD: the default, adds both components to each other: C¯result = Src + Dst.
     - GL_FUNC_SUBTRACT: subtracts both components from each other: C¯result = Src − Dst.
     - GL_FUNC_REVERSE_SUBTRACT: subtracts both components, but reverses order: C¯result = Dst − Src.
     */
    
    /*
     // When drawing a scene with non-transparent and transparent objects the general outline is usually as follows:
     
     - Draw all opaque objects first.
     - Sort all the transparent objects.
     - Draw all the transparent objects in sorted order.
     One way of sorting the transparent objects is to retrieve the distance of an object from the viewer's perspective. This can be achieved by taking the distance between the camera's position vector and the object's position vector.
     */
    
    
    // https://learnopengl.com/#!Advanced-OpenGL/Face-culling
    // Winding order
    // When we define a set of triangle vertices we're defining them in a certain winding order that is either clockwise or counter-clockwise. Each triangle consists of 3 vertices and we specify those 3 vertices in a winding order as seen from the center of the triangle.
    
    /*
     1                          1
     |                          |
     | |                        | |
     |   |                      |   |
     |     |                     |    |
     |       |                   |      |
     |         |                 |        |
     |           |               |          |
     |             |             |            |
     |_______________|           |______________|
     3                2           2              3
     
     ClockWise                Counter-ClockWise
     1->2->3                      1->3->2
     */
    
    // Now that we know how to set the winding order of the vertices we can start using OpenGL's face culling option which is disabled by default.
    // To enable face culling we only have to enable OpenGL's GL_CULL_FACE option:
    
    glEnable(GL_CULL_FACE);//only front facing poligons are rendered, for faces having a clock wise or counter clock wise order
    // GL_CULL_FACE is to be enabled for performance reasons, as it easily removes half of the triangles to draw, normally without visual artifacts if your geometry is watertight, and CCW.
    
    // OpenGL allows us to change the type of face we want to cull as well. What if we want to cull front faces and not the back faces? We can define this behavior by calling glCullFace:
    // glCullFace(GL_FRONT);
    
    // Needed when rendering the shadow map. This will avoid artifacts.
    glPolygonOffset(1.0f, 0.0f);
    
    /*
     The glCullFace function has three possible options:
     
     GL_BACK: Culls only the back faces.
     GL_FRONT: Culls only the front faces.
     GL_FRONT_AND_BACK: Culls both the front and back faces.
     
     */
    // The initial default value of glCullFace is GL_BACK.
    glCullFace(GL_BACK);
    
    // Aside from the faces to cull we can also tell OpenGL we'd rather prefer clockwise faces as the front-faces instead of counter-clockwise faces via glFrontFace:
    glFrontFace(GL_CCW); // this allow us to render in counter clockwise maner, by default opengl renders in this counter colockwise manner.
    // The default value is GL_CCW that stands for counter-clockwise ordering with the other option being GL_CW which (obviously) stands for clockwise ordering.
    
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    
}


void CGameWindow::PostRendering()
{
    glfwPollEvents();
}

void CGameWindow::ClearBuffers(const ClearBuffersType &bufferType){

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    switch (bufferType) {
        case ClearBuffersType::COLORDEPTHSTENCIL:
        //// CLEAR Buffers, The default clear value for the depth is 1.0f, which is equal to the depth of your far clipping plane
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); ////<-- CLEAR WINDOW
        glClearDepth(1.0f); // same as glClear, we are simply specificaly clearing the depthbuffer
        
        // enable depth testing (is disabled for rendering screen-space quad post processing)
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        
        // comment out if stencil buffer is not used
        // glStencilMask(0xFF); // each bit is written to the stencil buffer as is
        // glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
        // Most of the cases you'll just be writing 0x00 or 0xFF as the stencil mask, but it's good to know there are options to set custom bit-masks.
        glStencilMask(0x00);
        break;
    case ClearBuffersType::COLORDEPTH:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ////<-- CLEAR WINDOW
        glClearDepth(1.0f); // same as glClear, we are simply specificaly clearing the depthbuffer
        
        glEnable(GL_DEPTH_TEST);
        break;
    case ClearBuffersType::COLORSTENCIL:
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); ////<-- CLEAR WINDOW
        break;
    case ClearBuffersType::DEPTH:
        glClear(GL_DEPTH_BUFFER_BIT); ////<-- CLEAR WINDOW
        glEnable(GL_DEPTH_TEST);
        break;
    }
}

//Swap front and back buffers
void CGameWindow::SwapBuffers(){
    //glfwSwapInterval(1);
    glfwSwapBuffers(m_window);
}

//Destroy the window, // Deinitialise the window and rendering context
void CGameWindow::DestroyWindow(){
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

// Deinitialise the window and rendering context
CGameWindow::~CGameWindow(){

    DestroyWindow();
    SetCursorVisible(true);
}

