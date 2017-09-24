#include "PostProcessorChain.h"

#include "Framebuffer.h"
#include "PostProcessor.h"
#include "CopyPostProcessor.h"

#include <GL/glew.h>

#pragma region "Constructors/Destructor"

PostProcessorChain::PostProcessorChain() : m_pingFramebuffer(nullptr), m_pongFramebuffer(nullptr), m_currentInputFramebuffer(nullptr), m_head(nullptr), m_tail(nullptr), m_size(0)
{

}


PostProcessorChain::~PostProcessorChain()
{

}

#pragma endregion

#pragma region "Public Methods"

void PostProcessorChain::Initialize(Framebuffer * pingFramebuffer, Framebuffer * pongFramebuffer, CopyPostProcessor * copy)
{
	m_pingFramebuffer = m_currentInputFramebuffer = pingFramebuffer;
	m_pongFramebuffer = pongFramebuffer;

	m_head = copy;
	m_tail = copy;
}

void PostProcessorChain::Append(PostProcessor * postProcessor)
{
	if (m_size == 1)
	{
		m_head = postProcessor;
		m_head->m_next = m_tail;
		m_tail->m_previous = m_head;
	}
	else if (m_size == 2)
	{
		m_head->m_previous = postProcessor;
		postProcessor->m_next = m_head;
		m_head = postProcessor;
	}
	else
	{
		m_tail->m_previous->m_next = postProcessor;
		postProcessor->m_previous = m_tail->m_previous;
		postProcessor->m_next = m_tail;
		m_tail->m_previous = postProcessor;
	}
	m_size++;
}

void PostProcessorChain::Apply(Renderer & renderer)
{
	//start with the framebuffer where the scene was renderered to
	m_currentInputFramebuffer = m_pingFramebuffer;

	//walk the linked list and apply each post processing effect, if active
	PostProcessor * current = m_head;
	while (current->m_next)
	{
		if (current->GetEnabled())
		{
			current->Apply(m_currentInputFramebuffer, renderer);
			PingPongFramebuffers();
			current = current->m_next;
		}
	}

	//last post processing effect copies result to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLenum buffers[] = { GL_BACK_LEFT };
	glDrawBuffers(1, buffers);
	current->Apply(m_currentInputFramebuffer, renderer);
}

#pragma endregion

#pragma region "Private Methods"

//swap input and output framebuffers
void PostProcessorChain::PingPongFramebuffers()
{
	if (m_currentInputFramebuffer == m_pingFramebuffer)
		m_currentInputFramebuffer = m_pongFramebuffer;
	else
		m_currentInputFramebuffer = m_pingFramebuffer;
}

#pragma endregion