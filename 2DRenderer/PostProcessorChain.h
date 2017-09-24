#pragma once

class Renderer;
class PostProcessor;
class CopyPostProcessor;

struct Framebuffer;

class PostProcessorChain
{
public:
	
	//constructors/destructor
	PostProcessorChain();
	~PostProcessorChain();

	//public methods
	void Initialize(struct Framebuffer * ping, struct Framebuffer * pong, CopyPostProcessor * copy);
	void Append(PostProcessor * postProcessor);
	void Apply(Renderer & renderer);

private:

	//private methods
	void PingPongFramebuffers();

	struct Framebuffer * m_pingFramebuffer;
	struct Framebuffer * m_pongFramebuffer;

	struct Framebuffer * m_currentInputFramebuffer;

	PostProcessor * m_head;
	PostProcessor * m_tail;
	int m_size;

};

