#pragma once
#include <vulkan/vulkan.h>
#include <exception>
#include <stdexcept>
#include <optional>
#include "Window.h"
#include "Shader.h"

static const std::vector<const char*> swapchainDeviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

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

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
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
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateGraphicsPipeline();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffer();
	void CreateSyncObjects();
	void DrawFrame();
private:
	void OnWindowClose();
private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FoundQueueFamilies(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
private:
	bool m_Running = true;

	Window* m_Window;
	VkInstance m_Instance;
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_Device;
	VkQueue m_GraphicQueue;
	VkQueue m_PresentQueue;
	VkSurfaceKHR m_Surface;
	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;
	VkPipelineLayout m_PipelineLayout;
	VkRenderPass m_RenderPass;
	VkPipeline m_GraphicsPipeline;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	VkCommandPool m_CommandPool;
	VkCommandBuffer m_CommandBuffer;

	VkSemaphore m_ImageAvailableSemaphore;
	VkSemaphore m_RenderFinishedSemaphore;
	VkFence m_InFlightFence;
};


