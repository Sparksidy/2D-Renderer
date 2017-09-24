#pragma once

#include "Renderer.h"
#include "PostProcessorChain.h"
#include "Framebuffer.h"

class GraphicSystem
{
public:
	
	//constructors/destructor
	GraphicSystem(int const & width, int const & height);
	~GraphicSystem();


	//public methods
	void Initialize();
	void Finalize();
	
	void PrepareFrame();
	void RenderFrame();

	Renderer & GetRenderer();

private:

	int m_width;
	int m_height;

	unsigned int m_defaultFramebuffer;
		
	Framebuffer m_pingFramebuffer;
	Framebuffer m_pongFramebuffer;

	Renderer m_renderer;
	PostProcessorChain m_postProcessorChain;

};

