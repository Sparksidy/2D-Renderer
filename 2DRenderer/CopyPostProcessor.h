#pragma once

#include "PostProcessor.h"
#include "Program.h"

class CopyPostProcessor : public PostProcessor
{
public:

	CopyPostProcessor();
	~CopyPostProcessor();

	void Apply(Framebuffer * inputFramebuffer, Renderer & renderer);

private:
	
	Program m_program;

};

