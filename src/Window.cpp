#include "Window.h"
#include <iostream>

Window::Window(const WindowProperties& properties)
{
	Init();
}

Window::~Window()
{
	Shutdown();
}

void Window::Update()
{
	glfwPollEvents();
}

std::pair<uint32_t, const char**> Window::GetRequiredExtensions()
{
	uint32_t count = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);

	for (uint32_t i = 0; i < count; i++)
	{
		std::cout << "Extension name: " << extensions[i] << std::endl;
	}
	return std::make_pair(count, extensions);
}

Window* Window::Create(const WindowProperties& prop)
{
	return new Window(prop);
}

void Window::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(1920, 1440, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(m_Window, &m_Data);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.CloseCallback();
	});
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
