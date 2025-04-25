#pragma once

#include "vulkan/vulkan.h"
#include "VulkanDevice.h"

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

	void Init();

	static VkInstance GetInstance() { return s_Instance; }
	std::shared_ptr<VulkanDevice> GetDevice() { return m_Device; }

	static std::shared_ptr<VulkanContext> Create();

private:
	inline static VkInstance s_Instance;

	std::shared_ptr<VulkanPhysicalDevice> m_PhysicalDevice;
	std::shared_ptr<VulkanDevice> m_Device;
};