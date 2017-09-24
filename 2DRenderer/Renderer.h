#pragma once

#include <Windows.h>
#include <glm/glm.hpp>

class Program;

class ColoredSpriteComponent;
class TexturedSpriteComponent;
class AnimatedSpriteComponent;
class TransformComponent;

class Renderer
{
public:

	//constructors/destructor
	Renderer();
	~Renderer();

	//public methods
	void Initialize();
	void Finalize();

	unsigned int LoadPNG(char const * path);

	void ClearBuffers();

	void RenderSprite(ColoredSpriteComponent const & sprite, TransformComponent const & transform);
	void RenderSprite(TexturedSpriteComponent const & sprite, TransformComponent const & transform);
	void RenderSprite(AnimatedSpriteComponent const & sprite, TransformComponent const & transform);

	void RenderScreenQuad(Program & program);

	//getters
	glm::mat4 const & GetProjectionMatrix() const;
	glm::mat4 const & GetViewMatrix() const;

	//setters
	void SetProjectionMatrix(glm::mat4 const & projection);
	void SetViewMatrix(glm::mat4 const & view);


	void SetClearColor(float const & r, float const & g, float const & b, float const & a);

private:

	static glm::mat4 ConstructTransformMatrix(TransformComponent const & transform);

	//private methods
	void RenderQuad();


	Program * m_program;

	unsigned int m_quadVAO;
	unsigned int m_quadVBO;

	//attributes
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

};