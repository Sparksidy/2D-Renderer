#include "CopyPostProcessor.h"
#include "Renderer.h"

#include <GL/glew.h>

CopyPostProcessor::CopyPostProcessor() : m_program()
{
	m_program.AttachShader(Program::VERTEX_SHADER_TYPE, "CopyPostProcessor.vs");
	m_program.AttachShader(Program::FRAGMENT_SHADER_TYPE, "CopyPostProcessor.fs");
	m_program.Link();
}


CopyPostProcessor::~CopyPostProcessor()
{
}

void CopyPostProcessor::Apply(Framebuffer * inputFramebuffer, Renderer & renderer)
{
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, inputFramebuffer->colorBuffer0);
	m_program.SetUniform("inputTexture", 10);
	
	renderer.RenderScreenQuad(m_program);
}