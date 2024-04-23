#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include <string>
#include <functional>

struct WindowProperties
{
	const uint32_t Width;
	const uint32_t Height;
	const std::string Title;

	WindowProperties(const std::string& title = "Window", uint32_t width = 800, uint32_t height = 600)
		: Title(title), Width(width), Height(height)
	{}
};

class Window
{
public:
	using EventCloseCallback = std::function<void()>;

	Window(const WindowProperties& properties);
	~Window();

	void Update();

	void SetCloseCallback(const EventCloseCallback& callback) { m_Data.CloseCallback = callback; }

	std::pair<uint32_t, const char**> GetRequiredExtensions();

	inline GLFWwindow* GetNativeWindow() { return m_Window; }

	static Window* Create(const WindowProperties& prop = WindowProperties());
private:
	struct WindowData
	{
		EventCloseCallback CloseCallback;
	};

	void Init();
	void Shutdown();
private:
	GLFWwindow* m_Window;
	WindowProperties m_Properties;
	WindowData m_Data;
};