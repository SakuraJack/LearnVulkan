#include "pch.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"

VulkanPhysicalDevice::VulkanPhysicalDevice()
{

	// ---------------------Vulkan physical device---------------------
	auto vkInstance = VulkanContext::GetInstance();

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
	if(deviceCount <= 0)
	{
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

	VkPhysicalDevice selectedPhysicalDevice = VK_NULL_HANDLE;
	for(VkPhysicalDevice device : devices)
	{
		vkGetPhysicalDeviceProperties(device, &m_Properties);
		if(m_Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			selectedPhysicalDevice = device;
			break;
		}
	}

	if (!selectedPhysicalDevice)
	{
		selectedPhysicalDevice = devices.back();
	}

	if (!selectedPhysicalDevice)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
	m_PhysicalDevice = selectedPhysicalDevice;

	vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
	vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);
	if (queueFamilyCount <= 0)
	{
		throw std::runtime_error("Failed to find queue families!");
	}
	m_QueueFamilyProperties.resize(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, m_QueueFamilyProperties.data());

	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);
	if (extensionCount > 0)
	{
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		if (vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, availableExtensions.data()) == VK_SUCCESS)
		{
			for (const auto& ext : availableExtensions)
			{
				m_SupportedExtensions.emplace(ext.extensionName);
			}
		}
	}

	// ---------------------Queue family indices---------------------
	static const float defaultQueuePriority = 0.0f;
	int requesetQueueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
	m_QueueFamilyIndices = FoundQueueFamilyIndices(requesetQueueFlags);

	// Graphics queue
	if (requesetQueueFlags & VK_QUEUE_GRAPHICS_BIT)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.graphicsFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &defaultQueuePriority;	
		m_QueueCreateInfos.push_back(queueCreateInfo);
	}

	// Compute queue
	if (requesetQueueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		if (m_QueueFamilyIndices.computeFamily != m_QueueFamilyIndices.graphicsFamily)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.computeFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &defaultQueuePriority;
			m_QueueCreateInfos.push_back(queueCreateInfo);
		}
	}

	// Transfer queue
	if (requesetQueueFlags & VK_QUEUE_TRANSFER_BIT)
	{
		if (m_QueueFamilyIndices.transferFamily != m_QueueFamilyIndices.graphicsFamily && m_QueueFamilyIndices.transferFamily != m_QueueFamilyIndices.computeFamily)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = m_QueueFamilyIndices.transferFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &defaultQueuePriority;
			m_QueueCreateInfos.push_back(queueCreateInfo);
		}
	}

	m_DepthFormat = FoundDepthFormat();
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}

bool VulkanPhysicalDevice::IsExtensionSupport(const std::string & extensionName) const
{
	for (auto& extension : m_SupportedExtensions)
	{
		if (extension == extensionName) return true;
	}
	return false;
}

std::shared_ptr<VulkanPhysicalDevice> VulkanPhysicalDevice::Select()
{
	return std::make_shared<VulkanPhysicalDevice>();
}

VkFormat VulkanPhysicalDevice::FoundDepthFormat() const
{
	std::vector<VkFormat> depthFormats = {
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	for (auto& format : depthFormats)
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &formatProperties);
		if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			return format;
		}
	}
	return VkFormat();
}

VulkanPhysicalDevice::QueueFamilyIndices VulkanPhysicalDevice::FoundQueueFamilyIndices(int queueFlags)
{
	QueueFamilyIndices indices;

	if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++)
		{
			auto& queueFamilyProperties = m_QueueFamilyProperties[i];
			if ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) && (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
			{
				indices.computeFamily = i;
				break;
			}
		}
	}

	if (queueFlags & VK_QUEUE_TRANSFER_BIT)
	{
		for (uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++)
		{
			auto& queueFamilyProperties = m_QueueFamilyProperties[i];
			if ((queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) && 
				(queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 &&
				(queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
			{
				indices.transferFamily = i;
				break;
			}
		}
	}

	for (uint32_t i = 0; i < m_QueueFamilyProperties.size(); i++)
	{
		if ((queueFlags & VK_QUEUE_COMPUTE_BIT) && indices.computeFamily == -1)
		{
			indices.computeFamily = i;
		}

		if ((queueFlags & VK_QUEUE_TRANSFER_BIT) && indices.transferFamily == -1)
		{
			indices.transferFamily = i;
		}

		if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			if (m_QueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}
		}
	}

	return indices;
}

VulkanDevice::VulkanDevice(const std::shared_ptr<VulkanPhysicalDevice>& physicalDevice, VkPhysicalDeviceFeatures enabledFeatures)
	: m_PhysicalDevice(physicalDevice), m_EnabledFeatures(enabledFeatures)
{
	std::vector<const char*> deviceExtensions;
	if (!m_PhysicalDevice->IsExtensionSupport(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		throw std::runtime_error("Do not support swap chain!");
	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	if (m_PhysicalDevice->IsExtensionSupport(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME))
		deviceExtensions.push_back(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME);
	if (m_PhysicalDevice->IsExtensionSupport(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME))
		deviceExtensions.push_back(VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME);

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = physicalDevice->m_QueueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = physicalDevice->m_QueueCreateInfos.data();
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

	if (deviceExtensions.size() > 0)
	{
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	}

	VkResult result = vkCreateDevice(physicalDevice->m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(m_Device, physicalDevice->m_QueueFamilyIndices.graphicsFamily, 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_Device, physicalDevice->m_QueueFamilyIndices.computeFamily, 0, &m_ComputeQueue);
}

VulkanDevice::~VulkanDevice()
{}

std::shared_ptr<VulkanDevice> VulkanDevice::Create(const std::shared_ptr<VulkanPhysicalDevice>&physicalDevice, VkPhysicalDeviceFeatures enabledFeatures)
{
	return std::make_shared<VulkanDevice>(physicalDevice, enabledFeatures);
}
