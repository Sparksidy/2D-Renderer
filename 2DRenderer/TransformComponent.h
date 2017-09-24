#pragma once

#include <glm/glm.hpp>

class TransformComponent
{
public:

	//constructors/destructor
	TransformComponent(glm::vec2 const & translation, glm::vec2 const & scale, float const & rotation);
	~TransformComponent();

	//getters
	glm::vec2 const & GetTranslation() const;
	glm::vec2 const & GetScale() const;
	float const & GetRotation() const;

	//setters
	void SetTranslation(glm::vec2 const & translation);
	void SetScale(glm::vec2 const & scale);
	void SetRotation(float const & rotation);

private:

	glm::vec2	m_translation;
	glm::vec2	m_scale;
	float		m_rotation;

};

