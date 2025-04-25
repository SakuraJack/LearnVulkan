#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#include "Events/Event.h"
#include "Renderer/VulkanContext.h"
#include "Renderer/VulkanSwapChain.h"
#include "Renderer/VulkanDevice.h"

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
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProperties& properties);
	~Window();

	void OnUpdate();

	void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

	std::pair<uint32_t, const char**> GetRequiredExtensions();

	inline GLFWwindow* GetNativeWindow() { return m_Window; }

	static Window* Create(const WindowProperties& prop = WindowProperties());
private:
	void Init();
	void Shutdown();

private:
	GLFWwindow* m_Window;
	WindowProperties m_Properties;
	struct WindowData
	{
		std::string Title;
		uint32_t Width, Height;
		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
	std::shared_ptr<VulkanContext> m_Context;
	std::shared_ptr<VulkanSwapChain> m_SwapChain;
};