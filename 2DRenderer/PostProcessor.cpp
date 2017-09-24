#pragma once

#include "PostProcessor.h"

#pragma region "Constructors/Destructor"

PostProcessor::PostProcessor() : m_previous(nullptr), m_next(nullptr)
{

}

PostProcessor::~PostProcessor()
{

}

#pragma endregion

#pragma region "Getters"

bool const & PostProcessor::GetEnabled() const
{
	return m_enabled;
}

#pragma endregion

#pragma region "Setters"

void PostProcessor::SetEnabled(bool const & enabled)
{
	m_enabled = enabled;
}

#pragma endregion