#include "pch.h"
#include "Core/Window.h"
#include "Core/Application.h"

int main()
{
	Application* app = new Application();
	try
	{
		app->Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	delete app;

	return EXIT_SUCCESS;
}