#pragma once
#include <vulkan/vulkan.h>
#include <exception>
#include <stdexcept>
#include <optional>
#include "Window.h"

class VulkanApplication
{
private:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
public:
	VulkanApplication();
	~VulkanApplication();

	void Run();
private:
	void InitWindow();
	void InitVulkan();
	void Cleanup();
	
private:
	void CreateInstance();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface();
private:
	void OnWindowClose();
private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FoundQueueFamilies(VkPhysicalDevice device);
private:
	bool m_Running = true;

	Window* m_Window;
	VkInstance m_Instance;
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_Device;
	VkQueue m_GraphicQueue;
	VkQueue m_PresentQueue;
	VkSurfaceKHR m_Surface;
};


