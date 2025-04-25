#include "pch.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

static bool s_GLFWInitialized = false;

Window::Window(const WindowProperties& properties)
{
	Init();
}

Window::~Window()
{
	Shutdown();
}

void Window::OnUpdate()
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

	if (!s_GLFWInitialized)
	{
		int success = glfwInit();
		if (!success)
		{
			std::cout << "Could not initialize GLFW" << std::endl;
		}
		else
		{
			s_GLFWInitialized = true;
		}
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_Window = glfwCreateWindow(1920, 1440, "Vulkan", nullptr, nullptr);
	
	// 创建Context

	m_Context = VulkanContext::Create();
	m_Context->Init();

	// 创建SwapChain
	m_SwapChain = std::make_shared<VulkanSwapChain>();
	m_SwapChain->Init(m_Context->GetInstance(), m_Context->GetDevice());


	glfwSetWindowUserPointer(m_Window, &m_Data);

	glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

}

void Window::Shutdown()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}
