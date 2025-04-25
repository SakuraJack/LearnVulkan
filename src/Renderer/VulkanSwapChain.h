#pragma once

#include "vulkan/vulkan.h"
#include "VulkanDevice.h"
#include "GLFW/glfw3.h"

class VulkanSwapChain
{
public:
	VulkanSwapChain() = default;

	void Init(VkInstance instance, const std::shared_ptr<VulkanDevice>& device);
	void InitSurface(GLFWwindow* window);
	void Create(uint32_t width, uint32_t height, bool vsync);
private:
	void FindImageFormatAndColorSpace();
private:
	VkInstance m_Instance;
	std::shared_ptr<VulkanDevice> m_Device;
	bool m_Vsync;

	uint32_t m_QueueFamilyIndex;

	VkSurfaceKHR m_Surface;
	VkFormat m_ColorFormat;
	VkColorSpaceKHR m_ColorSpace;
};
