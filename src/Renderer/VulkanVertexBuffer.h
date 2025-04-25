#pragma once

#include <vulkan/vulkan.h>

class VulkanVertexBuffer
{
public:
	VulkanVertexBuffer() = default;
	~VulkanVertexBuffer();

	inline VkBuffer GetVertexBuffer() const { return m_vertexBuffer; }

	static VulkanVertexBuffer* CreateVertexBuffer(int vertexCount, VkDevice& device);
private:
	VkBuffer m_vertexBuffer;
};