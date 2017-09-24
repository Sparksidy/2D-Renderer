#pragma once
 
#include <glm/glm.hpp>

class ColoredSpriteComponent
{
public:
	
	ColoredSpriteComponent(glm::vec3 const & color);
	~ColoredSpriteComponent();

	//getters
	glm::vec3 const & GetColor() const;

	//setters
	void SetColor(glm::vec3 const & color);

private:

	glm::vec3 m_color;

	float m_depth;

};

