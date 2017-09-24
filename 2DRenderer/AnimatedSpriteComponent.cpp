#include "AnimatedSpriteComponent.h"

#include <GL/glew.h>

#pragma region "Constructors/Destructor"

AnimatedSpriteComponent::AnimatedSpriteComponent(unsigned int const & spriteSheet, SpriteSheetType const & type, unsigned int const & framesPerElement, unsigned int const & frames) : m_spriteSheet(spriteSheet), m_type(type), m_framesPerElement(framesPerElement), m_frames(frames), m_currentFrame(0), m_frameSize(0, 0)
{
	switch (m_type) {
	case COLUMN_SPRITE_SHEET:
		m_frameSize.x = 1.0f / (float)((m_frames + (m_framesPerElement - 1)) / m_framesPerElement);
		m_frameSize.y = 1.0f / (float)m_framesPerElement;
		break;
	case ROW_SPRITE_SHEET:
		m_frameSize.x = 1.0f / (float)m_framesPerElement;
		m_frameSize.y = 1.0f / (float)((m_frames + (m_framesPerElement - 1)) / m_framesPerElement);
		break;
	}
}


AnimatedSpriteComponent::~AnimatedSpriteComponent()
{
}

#pragma endregion

#pragma region "Getters"

unsigned int const & AnimatedSpriteComponent::GetSpriteSheet() const
{
	return m_spriteSheet;
}

AnimatedSpriteComponent::SpriteSheetType const & AnimatedSpriteComponent::GetType() const
{
	return m_type;
}

unsigned int const & AnimatedSpriteComponent::GetFramesPerElement() const
{
	return m_framesPerElement;
}

unsigned int const & AnimatedSpriteComponent::GetFrames() const
{
	return m_frames;
}

unsigned int const & AnimatedSpriteComponent::GetCurrentFrame() const
{
	return m_currentFrame;
}

glm::vec2 const & AnimatedSpriteComponent::GetFrameSize() const
{
	return m_frameSize;
}

#pragma endregion 

#pragma region "Setters"

void AnimatedSpriteComponent::SetCurrentFrame(unsigned int const & frame)
{
	m_currentFrame = frame % m_frames;
}

#pragma endregion

#pragma region "Public Methods"

void AnimatedSpriteComponent::NextFrame()
{
	m_currentFrame = (m_currentFrame + 1) % m_frames;
}

#pragma endregion