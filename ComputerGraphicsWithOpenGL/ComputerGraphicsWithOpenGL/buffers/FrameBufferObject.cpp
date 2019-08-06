#include "FrameBufferObject.h"

// https://learnopengl.com/#!Advanced-OpenGL/Framebuffers
// https://learnopengl.com/#!Advanced-OpenGL/Anti-Aliasing
// https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping

CFrameBufferObject::CFrameBufferObject()
{
	m_uiFramebuffer = 0;
    m_uiHdrColorTexture = 0;
    m_uiPositionTexture = 0;
    m_uiDepthTexture = 0;
    m_uiDepthCubeMap = 0;
    m_uiColourTexture = 0;
    m_uiAlbedoSpecTexture = 0;
    m_uiRboDepthStencil = 0;
    m_uiRboDepth = 0;
    
    for (unsigned int i = 0; i < 2; i++){
        m_uiHdrColorTextures[i] = 0;
    }
    
    for (unsigned int i = 0; i < 2; i++){
        m_uiPingpongFramebuffers[i] = 0;
        m_uiPingpongColorTextures[i] = 0;
    }
}

CFrameBufferObject::~CFrameBufferObject()
{
	Release();
}

// Create an FBO with a texture attachment:  note this code comes mostly from the Superbible, 6th edition, page 367
bool CFrameBufferObject::CreateFramebuffer(const int &a_iWidth, const int &a_iHeight, const FrameBufferType &fboType)
{
    m_iWidth = a_iWidth;
    m_iHeight = a_iHeight;
        
    /// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    if(m_uiFramebuffer != 0) return false;
    
    /*  https://learnopengl.com/#!Advanced-OpenGL/Framebuffers
     
    // So far we've used several types of screen buffers: a color buffer for writing color values, a depth buffer to write depth information and finally a stencil buffer that allows us to discard certain fragments based on some condition. The combination of these buffers is called a framebuffer and is stored somewhere in memory. OpenGL gives us the flexibility to define our own framebuffers and thus define our own color and optionally a depth and stencil buffer.
    
    // The rendering operations we've done so far were all done on top of the render buffers attached to the default framebuffer. The default framebuffer is created and configured when you create your window (GLFW does this for us). By creating our own framebuffer we can get an additional means to render to.
    // The application of framebuffers might not immediately make sense, but rendering your scene to a different framebuffer allows us to create mirrors in a scene or do cool post-processing effects for example. 
    */
    
    switch(fboType) {
        case FrameBufferType::DepthMapping: {
            
            // configure depth map FBO
            // -----------------------
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // create depth texture
            // Depth texture for Shadow Mapping https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
            // we create a 2D texture that we'll use as the framebuffer's depth buffer:
            glGenTextures(1, &m_uiDepthTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
            
            //We give the texture a width and height of 1024: this is the resolution of the depth map.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            
            glGenerateMipmap(GL_TEXTURE_2D);
            glGenSamplers(1, &m_uiSampler);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            // With the generated depth texture we can attach it as the framebuffer's depth buffer.
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiDepthTexture, 0);
            
            // We only need the depth information when rendering the scene from the light's perspective so there is no need for a color buffer. A framebuffer object however is not complete without a color buffer so we need to explicitly tell OpenGL we're not going to render any color data. We do this by setting both the read and draw buffer to GL_NONE with glDrawBuffer and glReadbuffer.
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            
            // Check completeness
            bool depthMapFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( depthMapFramebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: Depth Mapping Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return depthMapFramebufferComplete;
        }
            
        case FrameBufferType::DirectionalShadowMapping: {
            
            // configure depth map FBO
            // -----------------------
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // create depth texture
            // Depth texture for Shadow Mapping https://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
            // we create a 2D texture that we'll use as the framebuffer's depth buffer:
            glGenTextures(1, &m_uiDepthTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
            
            //We give the texture a width and height of 1024: this is the resolution of the depth map.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            
            glGenerateMipmap(GL_TEXTURE_2D);
            glGenSamplers(1, &m_uiSampler);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
            SetSamplerObjectParameterfv(GL_TEXTURE_BORDER_COLOR, borderColor);
            
            // With the generated depth texture we can attach it as the framebuffer's depth buffer.
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiDepthTexture, 0);
            
            // We only need the depth information when rendering the scene from the light's perspective so there is no need for a color buffer. A framebuffer object however is not complete without a color buffer so we need to explicitly tell OpenGL we're not going to render any color data. We do this by setting both the read and draw buffer to GL_NONE with glDrawBuffer and glReadbuffer.
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            
            // Check completeness
            bool directionalShadowMapFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( directionalShadowMapFramebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: Depth Mapping Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return directionalShadowMapFramebufferComplete;
        }
            
        case FrameBufferType::OmnidirectionalShadowMapping: {
            
            // configure depth map FBO
            // -----------------------
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            
            // create depth cubemap texture
            // cubemap Depth texture for point Shadow Mapping https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/3.2.1.point_shadows/point_shadows.cpp
            
            glGenTextures(1, &m_uiDepthCubeMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiDepthCubeMap);
            
            for (unsigned int i = 0; i < 6; ++i) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            }
            
            glGenSamplers(1, &m_uiSampler);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            
            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiDepthCubeMap, 0);
            
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            
            // Check completeness
            bool omnidirectionalShadowMapFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( omnidirectionalShadowMapFramebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: Cube Map Shadow Mapping Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return omnidirectionalShadowMapFramebufferComplete;
        }
       
        case FrameBufferType::HighDynamicRangeLighting: {
            
            /*  https://learnopengl.com/#!Advanced-Lighting/HDR
             To implement high dynamic range rendering we need some way to prevent color values getting clamped after each fragment shader run. When framebuffers use a normalized fixed-point color format (like GL_RGB) as their colorbuffer's internal format OpenGL automatically clamps the values between 0.0 and 1.0 before storing them in the framebuffer. This operation holds for most types of framebuffer formats, except for floating point formats that are used for their extended range of values.
             
             When the internal format of a framebuffer's colorbuffer is specified as GL_RGB16F, GL_RGBA16F, GL_RGB32F or GL_RGBA32F the framebuffer is known as a floating point framebuffer that can store floating point values outside the default range of 0.0 and 1.0. This is perfect for rendering in high dynamic range!
             
             To create a floating point framebuffer the only thing we need to change is its colorbuffer's internal format parameter:
             
             
             glBindTexture(GL_TEXTURE_2D, colorBuffer);
             glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
             The default framebuffer of OpenGL (by default) only takes up 8 bits per color component. With a floating point framebuffer with 32 bits per color component (when using GL_RGB32F or GL_RGBA32F) we're using 4 times more memory for storing color values. As 32 bits isn't really necessary unless you need a high level of precision using GL_RGBA16F will suffice
             
             
             */
            
            /// Create a framebuffer object and bind it with
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // To bind the framebuffer we use glBindFramebuffer:
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // create floating point color buffer
            glGenTextures(1, &m_uiHdrColorTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiHdrColorTexture);
            
            /*
             When the internal format of a framebuffer's colorbuffer is specified as GL_RGB16F, GL_RGBA16F, GL_RGB32F or GL_RGBA32F the framebuffer is known as a floating point framebuffer that can store floating point values outside the default range of 0.0 and 1.0. This is perfect for rendering in high dynamic range!
             */
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_iWidth, m_iHeight, 0, GL_BGRA, GL_FLOAT, nullptr);
            
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // Create a sampler object and set texture properties.  Note here, we're mipmapping
            glGenSamplers(1, &m_uiSampler);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // create depth buffer (renderbuffer)
            glGenRenderbuffers(1, &m_uiRboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uiRboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_iWidth, m_iHeight);
            
            // attach and bind buffers
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiHdrColorTexture, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiRboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            
            // Check completeness
            bool HDRFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( HDRFramebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: HDR Framebuffer is not complete!" << std::endl;
                
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return HDRFramebufferComplete;
        }
            
        case FrameBufferType::HighDynamicRangeRendering: {
            /*  https://learnopengl.com/#!Advanced-Lighting/Bloom
             High Dynamic Range Multiple Render Targets for extracting bright colors
             
             Extracting bright color
             The first step requires us to extract two images from a rendered scene. We could render the scene twice, both rendering to a different framebuffer with different shaders, but we can also use a neat little trick called Multiple Render Targets (MRT) that allows us to specify more than one fragment shader output; this gives us the option to extract the first two images in a single render pass. By specifying a layout location specifier before a fragment shader's output we can control to which colorbuffer a fragment shader writes to:
             
             
             layout (location = 0) out vec4 FragColor;
             layout (location = 1) out vec4 BrightColor;
             
             This only works however if we actually have multiple places to write to. As a requirement for using multiple fragment shader outputs we need multiple colorbuffers attached to the currently bound framebuffer object. You might remember from the framebuffers tutorial that we can specify a color attachment when linking a texture as a framebuffer's colorbuffer. Up until now we've always used GL_COLOR_ATTACHMENT0, but by also using GL_COLOR_ATTACHMENT1 we can have have two colorbuffers attached to a framebuffer object:
             
             
             */
            // configure (floating point) framebuffers
            // ---------------------------------------
            /// Create a framebuffer object and bind it with
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // To bind the framebuffer we use glBindFramebuffer:
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
            glGenTextures(2, m_uiHdrColorTextures);
            for (unsigned int i = 0; i < 2; i++)
            {
                glBindTexture(GL_TEXTURE_2D, m_uiHdrColorTextures[i]);
                /*
                 When the internal format of a framebuffer's colorbuffer is specified as GL_RGB16F, GL_RGBA16F, GL_RGB32F or GL_RGBA32F the framebuffer is known as a floating point framebuffer that can store floating point values outside the default range of 0.0 and 1.0. This is perfect for rendering in high dynamic range!
                 */
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
                
                glGenerateMipmap(GL_TEXTURE_2D);
                
                // Create a sampler object and set texture properties.  Note here, we're mipmapping
                glGenSamplers(1, &m_uiSampler);
                SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// we clamp to the edge as the blur filter would otherwise sample repeated texture values!
                SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glBindTexture(GL_TEXTURE_2D, 0);
                
                // attach texture to framebuffer
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_uiHdrColorTextures[i], 0);
                
            }
            
            // create and attach depth buffer (renderbuffer)
            glGenRenderbuffers(1, &m_uiRboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uiRboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_iWidth, m_iHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiRboDepth);
            
            // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
            // We do have to explicitly tell OpenGL we're rendering to multiple colorbuffers via glDrawBuffers as otherwise OpenGL only renders to a framebuffer's first color attachment ignoring all others. We can do this by passing an array of color attachment enums that we'd like to render to in subsequent operations:
            unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            glDrawBuffers(2, attachments);
            
            // Check completeness
            bool HDRframebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

            if( HDRframebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: HDR Rendering Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return HDRframebufferComplete;
        }
        case FrameBufferType::PingPongRendering: {
            
            // configure ping pong map FBO
            // ping-pong-framebuffer for blurring
            // -----------------------
            glGenFramebuffers(2, m_uiPingpongFramebuffers);
            glGenTextures(2, m_uiPingpongColorTextures);
            
            bool pingpongFramebufferComplete = false;
            for (unsigned int i = 0; i < 2; i++)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, m_uiPingpongFramebuffers[i]);
                
                glBindTexture(GL_TEXTURE_2D, m_uiPingpongColorTextures[i]);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
                
                glGenerateMipmap(GL_TEXTURE_2D);
                
                // Create a sampler object and set texture properties.  Note here, we're mipmapping
                glGenSamplers(1, &m_uiSampler);
                SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
                //glBindTexture(GL_TEXTURE_2D, 0);
                
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiPingpongColorTextures[i], 0);
                
                // also check if framebuffers are complete (no need for depth buffer)
                pingpongFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
                if( pingpongFramebufferComplete == false ){
                    std::cout << "ERROR::FRAMEBUFFER:: ping-pong Framebuffer is not complete!" << std::endl;
                    break;
                }
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return pingpongFramebufferComplete;
        }
        case FrameBufferType::DeferredRendering: {

            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // To bind the framebuffer we use glBindFramebuffer:
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
            glGenTextures(2, m_uiHdrColorTextures);
            for (unsigned int i = 0; i < 2; i++)
            {
                glBindTexture(GL_TEXTURE_2D, m_uiHdrColorTextures[i]);
                /*
                 When the internal format of a framebuffer's colorbuffer is specified as GL_RGB16F, GL_RGBA16F, GL_RGB32F or GL_RGBA32F the framebuffer is known as a floating point framebuffer that can store floating point values outside the default range of 0.0 and 1.0. This is perfect for rendering in high dynamic range!
                 */
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
                
                glGenerateMipmap(GL_TEXTURE_2D);
                
                // Create a sampler object and set texture properties.  Note here, we're mipmapping
                glGenSamplers(1, &m_uiSampler);
                // SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture(GL_TEXTURE_2D, 0);
                
                // attach texture to framebuffer
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_uiHdrColorTextures[i], 0);
            }
            
            // - position buffer
            /*
             To get the position texture, we can use it to obtain depth values for each of the kernel samples. Note that we store the positions in a floating point data format; this way position values aren't clamped to [0.0,1.0]. Also note the texture wrapping method of GL_CLAMP_TO_EDGE. This ensures we don't accidentally oversample position/depth values in screen-space outside the texture's default coordinate region.
             */
            glGenTextures(1, &m_uiPositionTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiPositionTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_uiPositionTexture, 0);
            
            // - normal buffer
            // Now, create a normal texture for the FBO
            glGenTextures(1, &m_uiNormalTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_uiNormalTexture, 0);
            
            // - color + specular color buffer
            // Now, create a colot spec texture for the FBO
            glGenTextures(1, &m_uiAlbedoSpecTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiAlbedoSpecTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, m_uiAlbedoSpecTexture, 0);
            
            // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
            unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
            glDrawBuffers(5, attachments);
            
            // create and attach depth buffer (renderbuffer)
            glGenRenderbuffers(1, &m_uiRboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uiRboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_iWidth, m_iHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiRboDepth);
            
            // also check if framebuffers are complete (no need for depth buffer)
            bool deferredRenderingFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( deferredRenderingFramebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: Geometry Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return deferredRenderingFramebufferComplete;
        }
        case FrameBufferType::SSAO: {
            // configure framebuffer to hold SSAO processing stage
            // -----------------------------------------------------
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // attach depth texture as FBO's depth buffer
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // - position color buffer
            /// The texture we're going to render to
            // Create a texture for our colour buffer
            glGenTextures(1, &m_uiColourTexture);
            
            /// "Bind" the newly created texture : all future texture functions will modify this texture
            glBindTexture(GL_TEXTURE_2D, m_uiColourTexture);
            
            /// Give an empty image to OpenGL ( the last "0" )
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // Create a sampler object and set texture properties.  Note here, we're mipmapping
            glGenSamplers(1, &m_uiSampler);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiColourTexture, 0);
            
            // Check completeness
            bool ssaoMapFramebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( ssaoMapFramebufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: Screen Space Ambient Occlusion Mapping Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return ssaoMapFramebufferComplete;
        }
        case FrameBufferType::GeometryBuffer: {
            
            //The G-buffer is the collective term of all textures used to store lighting-relevant data for the final lighting pass.
            /*  https://learnopengl.com/#!Advanced-Lighting/Bloom
             High Dynamic Range Multiple Render Targets for extracting bright colors
             
             Extracting bright color
             The first step requires us to extract two images from a rendered scene. We could render the scene twice, both rendering to a different framebuffer with different shaders, but we can also use a neat little trick called Multiple Render Targets (MRT) that allows us to specify more than one fragment shader output; this gives us the option to extract the first two images in a single render pass. By specifying a layout location specifier before a fragment shader's output we can control to which colorbuffer a fragment shader writes to:
             
             
             layout (location = 0) out vec4 FragColor;
             layout (location = 1) out vec4 BrightColor;
             
             This only works however if we actually have multiple places to write to. As a requirement for using multiple fragment shader outputs we need multiple colorbuffers attached to the currently bound framebuffer object. You might remember from the framebuffers tutorial that we can specify a color attachment when linking a texture as a framebuffer's colorbuffer. Up until now we've always used GL_COLOR_ATTACHMENT0, but by also using GL_COLOR_ATTACHMENT1 we can have have two colorbuffers attached to a framebuffer object:
             
             
             */
            // configure (floating point) framebuffers
            // ---------------------------------------
            /// Create a framebuffer object and bind it with
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // To bind the framebuffer we use glBindFramebuffer:
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
            glGenTextures(2, m_uiHdrColorTextures);
            for (unsigned int i = 0; i < 2; i++)
            {
                glBindTexture(GL_TEXTURE_2D, m_uiHdrColorTextures[i]);
                /*
                 When the internal format of a framebuffer's colorbuffer is specified as GL_RGB16F, GL_RGBA16F, GL_RGB32F or GL_RGBA32F the framebuffer is known as a floating point framebuffer that can store floating point values outside the default range of 0.0 and 1.0. This is perfect for rendering in high dynamic range!
                 */
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
                
                glGenerateMipmap(GL_TEXTURE_2D);
                
                // Create a sampler object and set texture properties.  Note here, we're mipmapping
                glGenSamplers(1, &m_uiSampler);
               // SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture(GL_TEXTURE_2D, 0);
                
                // attach texture to framebuffer
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_uiHdrColorTextures[i], 0);
            }
            
            // - position buffer
            /*
             To get the position texture, we can use it to obtain depth values for each of the kernel samples. Note that we store the positions in a floating point data format; this way position values aren't clamped to [0.0,1.0]. Also note the texture wrapping method of GL_CLAMP_TO_EDGE. This ensures we don't accidentally oversample position/depth values in screen-space outside the texture's default coordinate region.
             */
            glGenTextures(1, &m_uiPositionTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiPositionTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_uiPositionTexture, 0);
            
            // - normal buffer
            // Now, create a normal texture for the FBO
            glGenTextures(1, &m_uiNormalTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_FLOAT, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, m_uiNormalTexture, 0);
            
            // - color + specular color buffer
            // Now, create a colot spec texture for the FBO
            glGenTextures(1, &m_uiAlbedoSpecTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiAlbedoSpecTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, m_uiAlbedoSpecTexture, 0);
            
            // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
            unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
            glDrawBuffers(5, attachments);
            
            // create and attach depth buffer (renderbuffer)
            glGenRenderbuffers(1, &m_uiRboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, m_uiRboDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_iWidth, m_iHeight);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiRboDepth);
            
            // also check if framebuffers are complete (no need for depth buffer)
            bool gBufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            if( gBufferComplete == false ){
                std::cout << "ERROR::FRAMEBUFFER:: Geometry Framebuffer is not complete!" << std::endl;
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
            return gBufferComplete;
        }
    
        case FrameBufferType::Default: {
     
            // first we create a framebuffer object, bind it as the active framebuffer, do some operations and unbind the framebuffer.
            
            /// Create a framebuffer object and bind it with
            glGenFramebuffers(1, &m_uiFramebuffer);
            
            // To bind the framebuffer we use glBindFramebuffer:
            glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
            
            /*
             Unfortunately, we can't use our framebuffer yet because it is not complete. For a framebuffer to be complete the following requirements have to be satisfied:
             
             - We have to attach at least one buffer (color, depth or stencil buffer).
             - There should be at least one color attachment.
             - All attachments should be complete as well (reserved memory).
             - Each buffer should have the same number of samples.
             
             */
            
            
            // Now before the completeness check is executed we need to attach one or more attachments to the framebuffer. An attachment is a memory location that can act as a buffer for the framebuffer, think of it as an image. When creating an attachment we have two options to take: textures or renderbuffer objects.
            
            /////// Texture attachments //////
            /*
                 When attaching a texture to a framebuffer, all rendering commands will write to the texture as if it was a normal color/depth or stencil buffer. The advantage of using textures is that the result of all rendering operations will be stored as a texture image that we can then easily use in our shaders
             
            
            // Now that we've created a texture the last thing we need to do is actually attach it to the framebuffer:
             glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
             
             The glFrameBufferTexture2D has the following parameters:
             - target: the framebuffer type we're targeting (draw, read or both).
             - attachment: the type of attachment we're going to attach. Right now we're attaching a color attachment. Note that the 0 at the end suggests we can attach more than 1 color attachment. We'll get to that in a later tutorial.
             - textarget: the type of the texture you want to attach.
             - texture: the actual texture to attach.
             - level: the mipmap level. We keep this at 0.
             It is also possible to attach both a depth buffer and a stencil buffer as a single texture. Each 32 bit value of the texture then consists for 24 bits of depth information and 8 bits of stencil information. To attach a depth and stencil buffer as one texture we use the GL_DEPTH_STENCIL_ATTACHMENT type and configure the texture's formats to contain combined depth and stencil values. An example of attaching a depth and stencil buffer as one texture to the framebuffer is given below:
             
             
             glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
             
             glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
             */
            
            
            // - position color buffer
            /// The texture we're going to render to
            // Create a texture for our colour buffer
            glGenTextures(1, &m_uiColourTexture);
            
            /// "Bind" the newly created texture : all future texture functions will modify this texture
            glBindTexture(GL_TEXTURE_2D, m_uiColourTexture);
            
            /// Give an empty image to OpenGL ( the last "0" )
            // glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, a_iWidth, a_iHeight); // The Superbible suggests this, but it is OpenGL4.2 feature
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_iWidth, m_iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            // Create a sampler object and set texture properties.  Note here, we're mipmapping
            glGenSamplers(1, &m_uiSampler);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // - depth color buffer
            // Now, create a depth texture for the FBO
            glGenTextures(1, &m_uiDepthTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
            // glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, a_iWidth, a_iHeight);  // The Superbible suggests this, but it is OpenGL4.2 feature
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
            SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // - normal color buffer
            // Now, create a normal texture for the FBO
            glGenTextures(1, &m_uiNormalTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiNormalTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            // - color + specular color buffer
            // Now, create a colot spec texture for the FBO
            glGenTextures(1, &m_uiAlbedoSpecTexture);
            glBindTexture(GL_TEXTURE_2D, m_uiAlbedoSpecTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            /*
             The glFrameBufferTexture2D has the following parameters:
                 target: the framebuffer type we're targeting (draw, read or both).
                 attachment: the type of attachment we're going to attach. Right now we're attaching a color attachment. Note that the 0 at the end suggests we can attach more than 1 color attachment. We'll get to that in a later tutorial.
                 textarget: the type of the texture you want to attach.
                 texture: the actual texture to attach.
                 level: the mipmap level. We keep this at 0.
             */
            // Now we attach the colour and depth textures to the FBO
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_uiColourTexture, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  m_uiDepthTexture, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_uiNormalTexture, 0);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, m_uiAlbedoSpecTexture, 0);
            
            
            ///// Renderbuffer object attachments ////
            /*
             Renderbuffer objects were introduced to OpenGL after textures as a possible type of framebuffer attachments, so textures were the only attachments used in the good old days. Just like a texture image, a renderbuffer object is an actual buffer e.g. an array of bytes, integers, pixels or whatever. A renderbuffer object has the added advantage though that it stores its data in OpenGL's native rendering format making it optimized for off-screen rendering to a framebuffer.
             
             their data is already in its native format they are quite fast when writing data or simply copying their data to other buffers. Operations like switching buffers are thus quite fast when using renderbuffer objects. The glfwSwapBuffers function we've been using at the end of each render iteration might as well be implemented with renderbuffer objects: we simply write to a renderbuffer image, and swap to the other one at the end. Renderbuffer objects are perfect for these kind of operations.
             
             */
            
            // Creating a renderbuffer object looks similar to the framebuffer's code:
            glGenRenderbuffers(1, &m_uiRboDepthStencil);
            
            // And similarly we want to bind the renderbuffer object so all subsequent renderbuffer operations affect the current rbo:
            glBindRenderbuffer(GL_RENDERBUFFER, m_uiRboDepthStencil);
            
            // Since renderbuffer objects are generally write-only they are often used as depth and stencil attachments, since most of the time we don't really need to read values from the depth and stencil buffers but still care about depth and stencil testing. We need the depth and stencil values for testing, but don't need to sample these values so a renderbuffer object suits this perfectly. When we're not sampling from these buffers, a renderbuffer object is generally preferred since it's more optimized.
            // Creating a depth and stencil renderbuffer object is done by calling the glRenderbufferStorage function:
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_iWidth, m_iHeight);
            
            // Once we've allocated enough memory for the renderbuffer object we can unbind the renderbuffer.
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            
            // Creating a renderbuffer object is similar to texture objects, the difference being that this object is specifically designed to be used as an image, instead of a general purpose data buffer like a texture. Here we've chosen the GL_DEPTH24_STENCIL8 as the internal format, which holds both the depth and stencil buffer with 24 and 8 bits respectively.
            
            // Last thing left to do is actually attach the renderbuffer object:
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiRboDepthStencil);
            
            // Tell OpenGL that we want to draw into the framebuffer's colour attachment
            // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
            GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT };
            glDrawBuffers(4, attachments);
            
            // Check completeness
            bool framebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
            
            
            //All subsequent rendering operations will now render to the attachments of the currently bound framebuffer. Since our framebuffer is not the default framebuffer, the rendering commands will have no impact on the visual output of your window. For this reason it is called off-screen rendering while rendering to a different framebuffer.
            
            // As a final measure we want to check if the framebuffer is actually complete and if it's not, we print an error message.
            if( framebufferComplete == false) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            }
            
            // Then also be sure to unbind the framebuffer to make sure we're not accidentally rendering to the wrong framebuffer.
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            /*
            // So, to draw the scene to a single texture we'll have to take the following steps:
            
                 - Render the scene as usual with the new framebuffer bound as the active framebuffer.
                 - Bind to the default framebuffer.
                 - Draw a quad that spans the entire screen with the new framebuffer's color buffer as its texture.
            */
            
            return framebufferComplete;
            }
    }
    return false;
}

// Bind the FBO so we can render to it
void CFrameBufferObject::Bind(bool bSetFullViewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
    
    if(bSetFullViewport)glViewport(0, 0, m_iWidth, m_iHeight);

    glm::vec4 clearColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    float one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, &clearColour.r);
    glClearBufferfv(GL_DEPTH, 0, &one);
}

// Bind the Ping Pong FBO for rendering to texture
void CFrameBufferObject::BindPingPong(const GLuint &index, bool bSetFullViewport) {
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiPingpongFramebuffers[index]);
    if(bSetFullViewport)glViewport(0, 0, m_iWidth, m_iHeight);
    
    glm::vec4 clearColour = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    float one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, &clearColour.r);
    glClearBufferfv(GL_DEPTH, 0, &one);
}

// Binding the framebuffer color texture so it is active
void CFrameBufferObject::BindTexture(GLuint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_uiColourTexture);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Binding the HDR framebuffer color texture so it is active
void CFrameBufferObject::BindHDRTexture(GLuint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_uiHdrColorTexture);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Binding the HDR Render Targets framebuffer texture so it is active
void CFrameBufferObject::BindHDRTexture(const GLuint &index, GLuint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    
    glBindTexture(GL_TEXTURE_2D, m_uiHdrColorTextures[index]);
    
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Binding the framebuffer ping pong texture so it is active
void CFrameBufferObject::BindPingPongTexture(const GLuint &index, GLuint iTextureUnit)
{
    
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_uiPingpongColorTextures[index]);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}


void CFrameBufferObject::BindPositionTexture(GLuint iTextureUnit) {
    glActiveTexture(GL_TEXTURE0 + iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_uiPositionTexture);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Binding the framebuffer normal texture so it is active
void CFrameBufferObject::BindNormalTexture(GLuint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_uiNormalTexture);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Binding the framebuffer Albedo specular texture so it is active
void CFrameBufferObject::BindAlbedoTexture(GLuint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0+iTextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_uiAlbedoSpecTexture);
    glBindSampler(iTextureUnit, m_uiSampler);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// Binding the depth framebuffer texture so it is active
void CFrameBufferObject::BindDepthTexture(GLuint iTextureUnit)
{
	glActiveTexture(GL_TEXTURE0 + iTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_uiDepthTexture);
	glBindSampler(iTextureUnit, m_uiSampler);
}

// Binding the depth cube map framebuffer texture so it is active
void CFrameBufferObject::BindDepthCubeMap(GLuint iTextureUnit)
{
    glActiveTexture(GL_TEXTURE0 + iTextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiDepthCubeMap);
    glBindSampler(iTextureUnit, m_uiSampler);
}

// Blit multisampled buffer(s) to the usual colorbuffer of intermediate FBO. Image is stored in colorBuffer texture
void CFrameBufferObject::BlitToColorBuffer(GLuint colorbuffer)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_uiFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, colorbuffer);
    glBlitFramebuffer(0, 0, m_iWidth, m_iHeight, 0, 0, m_iWidth, m_iHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Blit multisampled buffer(s) to the usual depthbuffer of intermediate FBO. Image is stored in depthBuffer texture
void CFrameBufferObject::BlitToDepthBuffer(GLuint depthbuffer)
{
    // copy content of geometry's depth buffer to default framebuffer's depth buffer
    // ----------------------------------------------------------------------------------
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_uiFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depthbuffer); // write to default framebuffer at 0
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    glBlitFramebuffer(0, 0, m_iWidth, m_iHeight, 0, 0, m_iWidth, m_iHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

// Delete the framebuffer and all attachments
void CFrameBufferObject::Release()
{
	if(m_uiFramebuffer)
	{
		glDeleteFramebuffers(1, &m_uiFramebuffer);
		m_uiFramebuffer = 0;
	}
    
    glDeleteFramebuffers(2, m_uiPingpongFramebuffers);
    for (unsigned int i = 0; i < 2; i++){
        m_uiPingpongFramebuffers[i] = 0;
    }
	
	glDeleteSamplers(1, &m_uiSampler);
    glDeleteTextures(1, &m_uiHdrColorTexture);
    glDeleteTextures(1, &m_uiPositionTexture);
	glDeleteTextures(1, &m_uiColourTexture);
    glDeleteTextures(1, &m_uiAlbedoSpecTexture);
    glDeleteTextures(1, &m_uiNormalTexture);
	glDeleteTextures(1, &m_uiDepthTexture);
    glDeleteTextures(1, &m_uiDepthCubeMap);
    glDeleteTextures(2, m_uiHdrColorTextures);
    glDeleteTextures(2, m_uiPingpongColorTextures);
    
    glDeleteRenderbuffers(1, &m_uiRboDepthStencil);
    glDeleteRenderbuffers(1, &m_uiRboDepth);
}

void CFrameBufferObject::SetSamplerObjectParameter(GLenum parameter, GLenum value)
{
    //void glSamplerParameteri(    GLuint sampler,GLenum pname, GLint param);
	glSamplerParameteri(m_uiSampler, parameter, value);
}

void CFrameBufferObject::SetSamplerObjectParameterf(GLenum parameter, float value)
{
    //void glSamplerParameterf(    GLuint sampler, GLenum pname, GLfloat param);
	glSamplerParameterf(m_uiSampler, parameter, value);
}

void CFrameBufferObject::SetSamplerObjectParameterfv(GLenum parameter, const GLfloat * value)
{
    //void glSamplerParameterfv(    GLuint sampler, GLenum pname, const GLfloat * params);
    glSamplerParameterfv(m_uiSampler, parameter, value);
}
