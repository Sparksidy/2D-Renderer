#pragma once

#include <glm/glm.hpp>

class AnimatedSpriteComponent
{
public:

	typedef enum SpriteSheetType {
		COLUMN_SPRITE_SHEET,
		ROW_SPRITE_SHEET
	} SpriteSheetType;

	//constructors/destructor
	AnimatedSpriteComponent(unsigned int const & spriteSheet, SpriteSheetType const & type, unsigned int const & framesPerElement, unsigned int const & frames);
	~AnimatedSpriteComponent();

	//getters
	unsigned int const		& GetSpriteSheet() const;
	SpriteSheetType const	& GetType() const;
	unsigned int const		& GetFramesPerElement() const;
	unsigned int const		& GetFrames() const;
	unsigned int const		& GetCurrentFrame() const;

	glm::vec2 const			& GetFrameSize() const;

	//setters
	void SetCurrentFrame(unsigned int const & frame);

	//methods
	void NextFrame();

private:

	unsigned int	m_spriteSheet;
	SpriteSheetType m_type;
	unsigned int	m_framesPerElement;
	unsigned int	m_frames;
	unsigned int	m_currentFrame;

	glm::vec2 m_frameSize;

};