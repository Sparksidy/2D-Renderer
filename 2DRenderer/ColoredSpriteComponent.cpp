#include "ColoredSpriteComponent.h"

#pragma region "Constructors/Destructor"

ColoredSpriteComponent::ColoredSpriteComponent(glm::vec3 const & color) : m_color(color)
{
}

ColoredSpriteComponent::~ColoredSpriteComponent()
{
}

#pragma endregion

#pragma region "Getters"

glm::vec3 const & ColoredSpriteComponent::GetColor() const
{
	return m_color;
}

#pragma endregion

#pragma region "Setters"

void ColoredSpriteComponent::SetColor(glm::vec3 const & color)
{
	m_color = color;
}

#pragma endregion