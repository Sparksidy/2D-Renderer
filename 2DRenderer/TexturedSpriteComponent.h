#pragma once
class TexturedSpriteComponent
{
public:
	
	//constructores/destructor
	TexturedSpriteComponent(unsigned int const & texture);
	~TexturedSpriteComponent();

	//getters
	unsigned int const & GetTexture() const;

private:

	unsigned int m_texture;

};

