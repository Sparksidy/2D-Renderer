#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "GraphicSystem.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "ColoredSpriteComponent.h"
#include "TexturedSpriteComponent.h"
#include "AnimatedSpriteComponent.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window(800, 600, "2D Renderer");
	window.MakeCurrent();

	GraphicSystem graphicSystem(800, 600);

	graphicSystem.Initialize();

	graphicSystem.GetRenderer().SetProjectionMatrix(glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f));
	graphicSystem.GetRenderer().SetViewMatrix(glm::mat4(1.0f));

	unsigned int texture1 = graphicSystem.GetRenderer().LoadPNG("explosion.png");
	unsigned int texture2 = graphicSystem.GetRenderer().LoadPNG("runningcat.png");
	unsigned int texture3 = graphicSystem.GetRenderer().LoadPNG("background.png");

	TransformComponent transform1(glm::vec2(200.0f, 0.0f), glm::vec2(100.0f, 100.0f), 0.0f);
	AnimatedSpriteComponent sprite1(texture1, AnimatedSpriteComponent::ROW_SPRITE_SHEET, 5, 23);

	TransformComponent transform2(glm::vec2(-200.0f, 0.0f), glm::vec2(200.0f, 100.0f), 0.0f);
	AnimatedSpriteComponent sprite2(texture2, AnimatedSpriteComponent::COLUMN_SPRITE_SHEET, 4, 8);

	TransformComponent transform3(glm::vec2(-200.0f, 200.0f), glm::vec2(100.0f, 100.0f), 0.0f);
	TexturedSpriteComponent sprite3(texture2);

	TransformComponent transform4(glm::vec2(200.0f, 200.0f), glm::vec2(100.0f, 100.0f), 0.0f);
	TexturedSpriteComponent sprite4(texture1);

	TransformComponent transform5(glm::vec2(0.0f, 0.0f), glm::vec2(800.0f, 600.0f), 0.0f);
	TexturedSpriteComponent sprite5(texture3);

	MSG msg;
	std::memset(&msg, 0, sizeof(MSG));

	int counter = 0;
	while(msg.message != WM_QUIT)
	{

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		graphicSystem.PrepareFrame();
		 
		graphicSystem.GetRenderer().RenderSprite(sprite5, transform5);
		graphicSystem.GetRenderer().RenderSprite(sprite1, transform1);
		graphicSystem.GetRenderer().RenderSprite(sprite2, transform2);
		graphicSystem.GetRenderer().RenderSprite(sprite3, transform3);
		graphicSystem.GetRenderer().RenderSprite(sprite4, transform4);
		
		graphicSystem.RenderFrame();

		if (counter++ == 200)
		{
			sprite1.NextFrame();
			sprite2.NextFrame();
			counter = 0;
		}

		window.SwapBuffers();

	}

	graphicSystem.Finalize();

	return 0;
}