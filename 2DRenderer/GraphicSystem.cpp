#include "GraphicSystem.h"
#include "CopyPostProcessor.h"

#include <GL/glew.h>

#pragma region "Constructors/Destructor"

GraphicSystem::GraphicSystem(int const & width, int const & height) : m_width(width), m_height(height), m_defaultFramebuffer(0), m_pingFramebuffer({ 0,0,0,0 }), m_pongFramebuffer({ 0, 0, 0, 0 }), m_renderer(), m_postProcessorChain()
{

}


GraphicSystem::~GraphicSystem()
{

}

#pragma endregion

#pragma region "Public Methods"

void GraphicSystem::Initialize()
{
	if (glewInit() != GLEW_OK)
	{
		//report error
		MessageBox(NULL, "Failed to initialize glew", "Error!", MB_OK | MB_ICONHAND);
	}

	glGenFramebuffers(1, &m_pingFramebuffer.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_pingFramebuffer.framebuffer);

	glGenRenderbuffers(1, &m_pingFramebuffer.depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_pingFramebuffer.depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_pingFramebuffer.depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenTextures(1, &m_pingFramebuffer.colorBuffer0);
	glBindTexture(GL_TEXTURE_2D, m_pingFramebuffer.colorBuffer0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingFramebuffer.colorBuffer0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_pingFramebuffer.colorBuffer1);
	glBindTexture(GL_TEXTURE_2D, m_pingFramebuffer.colorBuffer1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pingFramebuffer.colorBuffer1, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		//report error
	}

	glGenFramebuffers(1, &m_pongFramebuffer.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_pongFramebuffer.framebuffer);

	glGenRenderbuffers(1, &m_pongFramebuffer.depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_pongFramebuffer.depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_pongFramebuffer.depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenTextures(1, &m_pongFramebuffer.colorBuffer0);
	glBindTexture(GL_TEXTURE_2D, m_pongFramebuffer.colorBuffer0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pongFramebuffer.colorBuffer0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_pongFramebuffer.colorBuffer1);
	glBindTexture(GL_TEXTURE_2D, m_pongFramebuffer.colorBuffer1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_pongFramebuffer.colorBuffer1, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		//report error
	}

	m_renderer.Initialize();
	m_postProcessorChain.Initialize(&m_pingFramebuffer, &m_pongFramebuffer, new CopyPostProcessor());
}

void GraphicSystem::Finalize()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteTextures(1, &m_pongFramebuffer.colorBuffer1);
	glDeleteTextures(1, &m_pongFramebuffer.colorBuffer0);
	glDeleteRenderbuffers(1, &m_pongFramebuffer.depthBuffer);
	glDeleteFramebuffers(1, &m_pongFramebuffer.framebuffer);

	glDeleteTextures(1, &m_pingFramebuffer.colorBuffer1);
	glDeleteTextures(1, &m_pingFramebuffer.colorBuffer0);
	glDeleteRenderbuffers(1, &m_pingFramebuffer.depthBuffer);
	glDeleteFramebuffers(1, &m_pingFramebuffer.framebuffer);

	m_renderer.Finalize();
}

void GraphicSystem::PrepareFrame()
{
	//clear all framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_renderer.ClearBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, m_pongFramebuffer.framebuffer);
	m_renderer.ClearBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, m_pingFramebuffer.framebuffer);
	m_renderer.ClearBuffers();

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);
}

void GraphicSystem::RenderFrame()
{
	//render frame
	//probably have to traverse a scene graph and create a render queue

	//apply post-processing effects
	m_postProcessorChain.Apply(m_renderer);
}

Renderer & GraphicSystem::GetRenderer()
{
	return m_renderer;
}

#pragma endregion