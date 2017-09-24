#include "TexturedSpriteComponent.h"

#pragma region "Constructors/Destructor"

TexturedSpriteComponent::TexturedSpriteComponent(unsigned int const & texture) : m_texture(texture)
{
}

TexturedSpriteComponent::~TexturedSpriteComponent()
{
}

#pragma endregion

#pragma region "Getters"

unsigned int const & TexturedSpriteComponent::GetTexture() const
{
	return m_texture;
}

#pragma endregion