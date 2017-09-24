#include "Program.h"

#include <GL/glew.h>
#include <cstdlib>
#include <cstdio>

#pragma region "Constructors/Destructor"

Program::Program() : m_handle(0)
{
	m_handle = glCreateProgram();
}


Program::~Program()
{
	glDeleteProgram(m_handle);
}

#pragma endregion

#pragma region "Public Methods"

void Program::Use()
{
	glUseProgram(m_handle);
}

void Program::Unuse()
{
	glUseProgram(0);
}

void Program::SetUniform(char const * name, glm::mat4 const & matrix)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniformMatrix4fv(m_handle, location, 1, GL_FALSE, &matrix[0][0]);
}

void Program::SetUniform(char const * name, glm::mat3 const & matrix)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniformMatrix3fv(m_handle, location, 1, GL_FALSE, &matrix[0][0]);
}

void Program::SetUniform(char const * name, glm::vec4 const & vector)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniform4fv(m_handle, location, 1, &vector[0]);
}

void Program::SetUniform(char const * name, glm::vec3 const & vector)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniform3fv(m_handle, location, 1, &vector[0]);
}

void Program::SetUniform(char const * name, glm::vec2 const & vector)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniform2fv(m_handle, location, 1, &vector[0]);
}

void Program::SetUniform(char const * name, float const & value)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniform1f(m_handle, location, value);
}

void Program::SetUniform(char const * name, int const & value)
{
	GLint location = glGetUniformLocation(m_handle, name);
	if (location != -1)
		glProgramUniform1i(m_handle, location, value);
}

void Program::AttachShader(ShaderType const & type, char const * sourcePath)
{
	FILE * file;
	fopen_s(&file, sourcePath, "rb");

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char * source = (char*)malloc(length+1);
	fread_s(source, length, 1, length, file);
	source[length] = 0;

	fclose(file);

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, &length);
	free(source);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_TRUE) {
		glAttachShader(m_handle, shader);
	}
	glDeleteShader(shader);
}

void Program::Link()
{
	glLinkProgram(m_handle);

	GLint status;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		//report error;
	}
}

#pragma endregion