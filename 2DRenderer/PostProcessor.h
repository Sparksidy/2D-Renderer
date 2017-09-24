#pragma once

#include "Framebuffer.h"

class Renderer;

class PostProcessor
{
public:

	friend class PostProcessorChain;

	//constructors/destructor
	PostProcessor();
	virtual ~PostProcessor();

	//public methods
	virtual void Apply(Framebuffer * inputFramebuffer, Renderer & renderer) = 0;

	//getters
	bool const & GetEnabled() const;

	//setters
	void SetEnabled(bool const & enabled);

private:

	PostProcessor * m_previous;
	PostProcessor * m_next;
	bool			m_enabled;

};

