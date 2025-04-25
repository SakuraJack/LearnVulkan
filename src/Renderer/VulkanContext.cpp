#include "pch.h"
#include "VulkanContext.h"
#include <GLFW/glfw3.h>

static bool s_Validation = false;

static bool CheckDriverAPIVersionSupport(uint32_t minimumSupportedVersion)
{
	uint32_t instanceVersion;
	vkEnumerateInstanceVersion(&instanceVersion);

	if(instanceVersion < minimumSupportedVersion)
	{
		std::cout << "Vulkan driver version incompetible!" << std::endl;
		std::cout << "You have Vulkan version " << VK_VERSION_MAJOR(instanceVersion) << "." << VK_VERSION_MINOR(instanceVersion) << "." << VK_VERSION_PATCH(instanceVersion) << std::endl;
		std::cout << "You need Vulkan version " << VK_VERSION_MAJOR(minimumSupportedVersion) << "." << VK_VERSION_MINOR(minimumSupportedVersion) << "." << VK_VERSION_PATCH(minimumSupportedVersion) << std::endl;

		return false;
	}

	return true;
}

VulkanContext::VulkanContext()
{}

VulkanContext::~VulkanContext()
{}

void VulkanContext::Init()
{
	if (!glfwVulkanSupported())
	{
		throw std::runtime_error("Vulkan is not supported");
	}

	if(!CheckDriverAPIVersionSupport(VK_API_VERSION_1_2))
	{
		throw std::runtime_error("Vulkan driver version incompetible!");
	}

	// -----------------------------Application info--------------------------------
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";
	appInfo.pEngineName = "Vulkan Engine";
	appInfo.apiVersion = VK_API_VERSION_1_2;

	// -----------------------------Extensions & Validations-------------------------
#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
	std::vector<char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	if(s_Validation)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
	}

	VkValidationFeatureEnableEXT enables[] = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT };
	VkValidationFeaturesEXT features = {};
	features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
	features.enabledValidationFeatureCount = 1;
	features.pEnabledValidationFeatures = enables;

	// -----------------------------Instance info-----------------------------------------
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceCreateInfo.pNext = nullptr;

	if (s_Validation)
	{
		const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
		uint32_t instanceLayerCount;
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
		std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
		vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
		bool validationLayerPresent = false;
		for (const auto& layer : instanceLayerProperties)
		{
			if(strcmp(layer.layerName, validationLayerName) == 0)
			{
				validationLayerPresent = true;
				break;
			}
		}
		
		if (validationLayerPresent)
		{
			instanceCreateInfo.enabledLayerCount = 1;
			instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
		}
		else
		{
			std::cout << "Validation layer not present!" << std::endl;
		}
	}

	// -----------------------------Create instance--------------------------------------
	if(vkCreateInstance(&instanceCreateInfo, nullptr, &s_Instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}

	if (s_Validation)
	{

	}

	// -----------------------------Create physical device---------------------------------------
	m_PhysicalDevice = VulkanPhysicalDevice::Select();

	VkPhysicalDeviceFeatures deviceFeatures = {};
	m_Device = VulkanDevice::Create(m_PhysicalDevice, deviceFeatures);
}

std::shared_ptr<VulkanContext> VulkanContext::Create()
{
	return std::make_shared<VulkanContext>();
}
