#include "TransformComponent.h"

#pragma region "Constructors/Destructor"

TransformComponent::TransformComponent(glm::vec2 const & translation, glm::vec2 const & scale, float const & rotation) : m_translation(translation), m_scale(scale), m_rotation(rotation)
{

}

TransformComponent::~TransformComponent()
{

}

#pragma endregion

#pragma region "Getters"

glm::vec2 const & TransformComponent::GetTranslation() const
{
	return m_translation;
}

glm::vec2 const & TransformComponent::GetScale() const
{
	return m_scale;
}

float const & TransformComponent::GetRotation() const
{
	return m_rotation;
}

#pragma endregion

#pragma region "Setters"

void TransformComponent::SetTranslation(glm::vec2 const & translation)
{
	m_translation = translation;
}

void TransformComponent::SetScale(glm::vec2 const & scale)
{
	m_scale = scale;
}

void TransformComponent::SetRotation(float const & rotation)
{
	m_rotation = rotation;
}

#pragma endregion