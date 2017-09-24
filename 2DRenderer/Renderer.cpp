#include "Renderer.h"
#include "Window.h"
#include "Program.h"
#include "TransformComponent.h"
#include "ColoredSpriteComponent.h"
#include "TexturedSpriteComponent.h"
#include "AnimatedSpriteComponent.h"

#include <GL/glew.h>
#include <png/png.h>
#include <cstdio>
#include <cstdlib>

#pragma region "Static Data and Functions"

glm::mat4 Renderer::ConstructTransformMatrix(TransformComponent const & transform)
{
	float const & rotation = transform.GetRotation();
	glm::vec2 const & scale = transform.GetScale();
	return glm::mat4(glm::vec4(scale.x * cosf(rotation), scale.x * sinf(rotation), 0.0f, 0.0f), 
			glm::vec4(-scale.y * sinf(rotation), scale.y * cosf(rotation), 0.0f, 0.0f), 
			glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), 
			glm::vec4(transform.GetTranslation(), 0.0f, 1.0f));
}

#pragma endregion

#pragma region "Constructors/Destructor"

Renderer::Renderer() : m_program(nullptr), m_quadVAO(0), m_quadVBO(0), m_projectionMatrix(1.0f), m_viewMatrix(1.0f)
{

}


Renderer::~Renderer()
{
	delete m_program;
}

#pragma endregion

#pragma region "Public Methods"

void Renderer::Initialize()
{
	//create program
	m_program = new Program();
	m_program->AttachShader(Program::VERTEX_SHADER_TYPE, "Generic.vs");
	m_program->AttachShader(Program::FRAGMENT_SHADER_TYPE, "Generic.fs");
	m_program->Link();

	glGenVertexArrays(1, &m_quadVAO);
	glBindVertexArray(m_quadVAO);

	float quadVertices[] = {
		-0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f
	};

	glGenBuffers(1, &m_quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (GLvoid*)(sizeof(float) * 2));

	glBindVertexArray(0);

	m_program->SetUniform("projectionMatrix", m_projectionMatrix);
	m_program->SetUniform("viewMatrix", m_viewMatrix);
	m_program->SetUniform("transformMatrix", glm::mat4(1.0f));
	m_program->SetUniform("sampler", 0);

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0, 0, 0, 1);
}

void Renderer::Finalize()
{
	glDeleteBuffers(1, &m_quadVBO);
	glDeleteVertexArrays(1, &m_quadVAO);
}

unsigned int Renderer::LoadPNG(char const * path)
{
	png_byte header[8];

	FILE *fp;
	fopen_s(&fp, path, "rb");

	if (fp == 0)
	{
		return false;
	}

	// read the header
	fread(header, 1, 8, fp);

	if (png_sig_cmp(header, 0, 8))
	{
		fclose(fp);
		return false;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf(stderr, "error: png_create_read_struct returned 0.\n");
		fclose(fp);
		return 0;
	}

	// create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	// create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		fprintf(stderr, "error: png_create_info_struct returned 0.\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return 0;
	}

	// the code in this if statement gets called if libpng encounters an error
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "error from libpng\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return 0;
	}

	// init png reading
	png_init_io(png_ptr, fp);

	// let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	// variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 temp_width, temp_height;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// glTexImage2d requires rows to be 4-byte aligned
	rowbytes += 3 - ((rowbytes - 1) % 4);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte * image_data;
	image_data = (png_byte*)malloc(rowbytes * temp_height * sizeof(png_byte) + 15);
	if (image_data == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG image data\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		return false;
	}

	// row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep * row_pointers = (png_bytep*)malloc(temp_height * sizeof(png_bytep));
	if (row_pointers == NULL)
	{
		fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(fp);
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	int i;
	for (i = 0; i < temp_height; i++)
	{
		row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
	}

	// read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	// Generate the OpenGL texture object
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_width, temp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// clean up
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(fp);
	return texture;
}

void Renderer::ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderSprite(ColoredSpriteComponent const & sprite, TransformComponent const & transform)
{
	m_program->SetUniform("mode", 0);
	m_program->SetUniform("transformMatrix", ConstructTransformMatrix(transform));
	m_program->SetUniform("color", sprite.GetColor());

	m_program->Use();
	
	RenderQuad();

	m_program->Unuse();
}

void Renderer::RenderSprite(TexturedSpriteComponent const & sprite, TransformComponent const & transform)
{
	m_program->SetUniform("mode", 1);
	m_program->SetUniform("transformMatrix", ConstructTransformMatrix(transform));
	
	m_program->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite.GetTexture());

	RenderQuad();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_program->Unuse();
}

void Renderer::RenderSprite(AnimatedSpriteComponent const & sprite, TransformComponent const & transform)
{
	m_program->SetUniform("mode", 2);
	m_program->SetUniform("transformMatrix", ConstructTransformMatrix(transform));

	//construct texture matrix
	glm::vec2 const & scale = sprite.GetFrameSize();
	float x, y;

	switch (sprite.GetType())
	{
		case AnimatedSpriteComponent::COLUMN_SPRITE_SHEET:
			x = (float)(sprite.GetCurrentFrame() / sprite.GetFramesPerElement()) * scale.x;
			y = 1.0f - ((float)(((sprite.GetCurrentFrame() % sprite.GetFramesPerElement())) + 1) * scale.y);
			break;
		case AnimatedSpriteComponent::ROW_SPRITE_SHEET:
			x = (float)(sprite.GetCurrentFrame() % sprite.GetFramesPerElement()) * scale.x;
			y = 1.0f - ((float)(((sprite.GetCurrentFrame() / sprite.GetFramesPerElement())) + 1) * scale.y);
			break;
	}

	glm::mat3 textureMatrix(glm::vec3(scale.x, 0.0f, 0.0f), glm::vec3(0.0f, scale.y, 0.0f), glm::vec3(x, y, 1.0f));
	m_program->SetUniform("textureMatrix", textureMatrix);

	m_program->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite.GetSpriteSheet());

	RenderQuad();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_program->Unuse();
}

void Renderer::RenderScreenQuad(Program & program)
{
	program.SetUniform("transformMatrix", glm::mat4(glm::vec4(2, 0, 0, 0), glm::vec4(0, 2, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1)));
	program.Use();
	RenderQuad();
	program.Unuse();
}

#pragma endregion

#pragma region "Getters"

glm::mat4 const & Renderer::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

glm::mat4 const & Renderer::GetViewMatrix() const
{
	return m_viewMatrix;
}

#pragma endregion

#pragma region "Setters"

void Renderer::SetProjectionMatrix(glm::mat4 const & projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
	m_program->SetUniform("projectionMatrix", m_projectionMatrix);
}

void Renderer::SetViewMatrix(glm::mat4 const & viewMatrix)
{
	m_viewMatrix = viewMatrix;
	m_program->SetUniform("viewMatrix", m_viewMatrix);
}

void Renderer::SetClearColor(float const & r, float const & g, float const & b, float const & a)
{
	glClearColor(r, g, b, a);
}

#pragma endregion

#pragma region "Private Methods"

void Renderer::RenderQuad()
{
	glBindVertexArray(m_quadVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

#pragma endregion