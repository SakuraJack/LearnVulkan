#include "pch.h"
#include "VulkanSwapChain.h"

void VulkanSwapChain::Init(VkInstance instance, const std::shared_ptr<VulkanDevice>& device)
{
	m_Instance = instance;
	m_Device = device;

	VkDevice vkDevice = m_Device->GetDevice();
}

void VulkanSwapChain::InitSurface(GLFWwindow* window)
{
	VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetPhysicalDevice();
	glfwCreateWindowSurface(m_Instance, window, nullptr, &m_Surface);

	// 获取队列族属性
	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueProperties(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProperties.data());

	// 查找支持图形和显示的队列族
	std::vector<VkBool32> supportsPresent(queueCount);
	for (uint32_t i = 0; i < queueCount; i++)
	{
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supportsPresent[i]);
	}

	// 查找支持图形和显示的队列族索引
	uint32_t graphicsQueueIndex = UINT32_MAX;
	uint32_t presentQueueIndex = UINT32_MAX;

	for (uint32_t i = 0; i < queueCount; i++)
	{
		if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			if (graphicsQueueIndex == UINT32_MAX)
			{
				graphicsQueueIndex = i;
			}

			if (supportsPresent[i])
			{
				graphicsQueueIndex = i;
				presentQueueIndex = i;
				break;
			}
		}
	}

	if (presentQueueIndex == UINT32_MAX)
	{
		for (uint32_t i = 0; i < queueCount; i++)
		{
			if (supportsPresent[i])
			{
				presentQueueIndex = i;
				break;
			}
		}
	}

	m_QueueFamilyIndex = graphicsQueueIndex;
	FindImageFormatAndColorSpace();
}

void VulkanSwapChain::Create(uint32_t width, uint32_t height, bool vsync)
{
	m_Vsync = vsync;

	VkDevice vkDevice = m_Device->GetDevice();
	VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetPhysicalDevice();

	VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE;

	// 获取表面能力
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &surfaceCapabilities);

	// 获取表面格式和颜色空间
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, nullptr);
	if (presentModeCount <= 0)
		throw std::runtime_error("Failed to find present modes!");
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, presentModes.data());

	VkExtent2D swapChainExtent = {};
}

void VulkanSwapChain::FindImageFormatAndColorSpace()
{
	VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice()->GetPhysicalDevice();

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr);
	if (formatCount <= 0)
		throw std::runtime_error("Failed to find formats!");

	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, surfaceFormats.data());

	// 如果表面格式列表只包含一个未定义的格式，则声明为B8G8R8A8_UNORM
	if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
		m_ColorSpace = surfaceFormats[0].colorSpace;
	}
	else
	{
		// 否则，查找是否有B8G8R8A8_UNORM格式
		bool found_B8G8R8A8_UNORM = false;
		for (const auto& surfaceFormat : surfaceFormats)
		{
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
			{
				m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
				m_ColorSpace = surfaceFormat.colorSpace;
				found_B8G8R8A8_UNORM = true;
				break;
			}
		}

		// 如果没有找到B8G8R8A8_UNORM格式，则选择第一个格式
		if (!found_B8G8R8A8_UNORM)
		{
			m_ColorFormat = surfaceFormats[0].format;
			m_ColorSpace = surfaceFormats[0].colorSpace;
		}

	}
}
