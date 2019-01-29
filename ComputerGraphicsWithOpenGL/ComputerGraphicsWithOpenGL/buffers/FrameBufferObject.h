#pragma once

#include "Texture.h"
#include "Shaders.h"

class CFrameBufferObject
{
public:
	CFrameBufferObject();
	~CFrameBufferObject();

    enum FBOType {
        Default,
        MultiSampling,
        DirectionalShadowMapping,
        PointShadowMapping,
        HighDynamicRangeMapping,
        HighDynamicRangeMultipleRenderTargets, //(MRT)
        PingPongRendering,
        DeferredRendering,
        SSAORendering,
        SSAOProcessing,
        SSAOBlur,
    };
    
	// Create a framebuffer object with a texture of a given size
	bool CreateFramebuffer(const int &a_iWidth, const int &a_iHeight, const FBOType &fboType = FBOType::Default);

	// Bind the FBO for rendering to texture
	void Bind(bool bSetFullViewport = true);
    
    // Bind the Ping Pong FBO for rendering to texture
    void BindPingPong(const GLuint &index, bool bSetFullViewport = true);

    // Bind the texture (usually on a 2nd or later pass in a multi-pass rendering technique)
    void BindTexture(GLuint iTextureUnit, const FBOType &fboType = FBOType::Default);

    // Binding the framebuffer normal texture so it is active
    void BindNormalTexture(GLuint iTextureUnit);

    // Binding the framebuffer Albedo specular texture so it is active
    void BindAlbedoTexture(GLuint iTextureUnit);
   
    // Binding the Multiple Render Targets framebuffer texture so it is active
    void BindMRTTexture(const GLuint &index, GLuint iTextureUnit);
    
    // Binding the framebuffer ping pong texture so it is active
    void BindPingPongTexture(const GLuint &index, GLuint iTextureUnit);
    
    // Bind the depth (usually on a 2nd or later pass in a multi-pass rendering technique)
    void BindDepth(GLuint iTextureUnit);
    
    // Bind the depth cubemap
    void BindDepthCubeMap(GLuint iTextureUnit);
    
    // Blit multisampled buffer(s) to the usual colorbuffer of intermediate FBO. Image is stored in colorBuffer texture
    void BlitToColorBuffer(GLuint colorbuffer);
    
    // Blit multisampled buffer(s) to the usual depthbuffer of intermediate FBO. Image is stored in depthBuffer texture
    void BlitToDepthBuffer(GLuint depthbuffer);
    
	// Delete the framebuffer
	void Release();

	// Set methods for the sampler object
	void SetSamplerObjectParameter(GLenum parameter, GLenum value);
	void SetSamplerObjectParameterf(GLenum parameter, float value);
    void SetSamplerObjectParameterfv(GLenum parameter, const GLfloat * value);

    GLuint GetWidth() const { return m_iWidth; }
    GLuint GetHeight() const { return m_iHeight; }
    GLuint GetFrameBuffer() const { return m_uiFramebuffer; }
    GLuint GetDepthTexture() const { return m_uiDepthTexture; }
    
    
private:

	GLuint m_iWidth, m_iHeight;
	GLuint m_uiFramebuffer;
    GLuint m_uiHdrColorTexture;
    GLuint m_uiColourTexture;
    GLuint m_uiColourTextureMultiSampled;
    GLuint m_uiNormalTexture;
    GLuint m_uiAlbedoSpecTexture;
	GLuint m_uiDepthTexture;
    GLuint m_uiDepthCubeMap;
    GLuint m_uiRboDepth;
    GLuint m_uiRboDepthStencil;
	GLuint m_uiSampler;

    GLuint m_uiHdrColorTextures[2];
    GLuint m_uiPingpongFramebuffers[2];
    GLuint m_uiPingpongColorTextures[2];

};
