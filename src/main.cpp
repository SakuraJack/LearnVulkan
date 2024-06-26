#include <exception>
#include <iostream>
#include "Window.h"
#include "VulkanApplication.h"

int main()
{
	VulkanApplication app;
	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}