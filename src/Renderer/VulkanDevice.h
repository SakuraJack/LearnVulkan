#pragma once

#include "vulkan/vulkan.h"

class VulkanPhysicalDevice
{
public:
	struct QueueFamilyIndices
	{
		int32_t graphicsFamily = -1;
		int32_t presentFamily = -1;
		int32_t computeFamily = -1;
		int32_t transferFamily = -1;
	};
public:
	VulkanPhysicalDevice();
	~VulkanPhysicalDevice();

	bool IsExtensionSupport(const std::string& extensionName) const;

	VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
	const QueueFamilyIndices& GetQueueFamilyIndices() const { return m_QueueFamilyIndices; }

	static std::shared_ptr<VulkanPhysicalDevice> Select();
private:
	VkFormat FoundDepthFormat() const;
	QueueFamilyIndices FoundQueueFamilyIndices(int queueFlags);
private:
	QueueFamilyIndices m_QueueFamilyIndices;

	VkPhysicalDevice m_PhysicalDevice = nullptr;
	VkPhysicalDeviceProperties m_Properties;
	VkPhysicalDeviceFeatures m_Features;
	VkPhysicalDeviceMemoryProperties m_MemoryProperties;

	VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;

	std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
	std::unordered_set<std::string> m_SupportedExtensions;
	std::vector<VkDeviceQueueCreateInfo> m_QueueCreateInfos;

	friend class VulkanDevice;
};


class VulkanDevice
{
public:
	VulkanDevice(const std::shared_ptr<VulkanPhysicalDevice>& physicalDevice, VkPhysicalDeviceFeatures enabledFeatures);
	~VulkanDevice();

	static std::shared_ptr<VulkanDevice> Create(const std::shared_ptr<VulkanPhysicalDevice>& physicalDevice, VkPhysicalDeviceFeatures enabledFeatures);

	inline VkDevice GetDevice() const { return m_Device; }
	inline std::shared_ptr<VulkanPhysicalDevice> GetPhysicalDevice() const { return m_PhysicalDevice; }
private:
	VkDevice m_Device = nullptr;
	std::shared_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
	VkPhysicalDeviceFeatures m_EnabledFeatures;

	VkQueue m_GraphicsQueue;
	VkQueue m_ComputeQueue;
};