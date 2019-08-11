#pragma once

#include "../BuffersBase.h"

class CFrameBufferObject
{
public:
	CFrameBufferObject();
	~CFrameBufferObject();

	// Create a framebuffer object with a texture of a given size
	bool CreateFramebuffer(const int &a_iWidth, const int &a_iHeight, const FrameBufferType &fboType = FrameBufferType::Default);

	// Bind the FBO for rendering to texture
	void Bind(bool bSetFullViewport = true);
    
    // Bind the Ping Pong FBO for rendering to texture
    void BindPingPong(const GLuint &index, bool bSetFullViewport = true);

    // Bind the texture (usually on a 2nd or later pass in a multi-pass rendering technique)
    void BindTexture(GLuint iTextureUnit);
    
    // Binding the framebuffer position texture so it is active
    void BindPositionTexture(GLuint iTextureUnit);
    
    // Binding the framebuffer normal texture so it is active
    void BindNormalTexture(GLuint iTextureUnit);

    // Binding the framebuffer Albedo specular texture so it is active
    void BindAlbedoTexture(GLuint iTextureUnit);
   
    // Binding the HDR framebuffer color texture so it is active
    void BindHDRTexture(GLuint iTextureUnit);
    
    // Binding the HDR Render Targets framebuffer texture so it is active
    void BindHDRTexture(const GLuint &index, GLuint iTextureUnit);
    
    // Binding the ping pong framebuffer texture so it is active
    void BindPingPongTexture(const GLuint &index, GLuint iTextureUnit);
    
    // Bind the depth (usually on a 2nd or later pass in a multi-pass rendering technique)
    void BindDepthTexture(GLuint iTextureUnit);
    
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
    GLuint m_uiColourTexture;
    GLuint m_uiHdrColorTexture;
    GLuint m_uiPositionTexture;
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
