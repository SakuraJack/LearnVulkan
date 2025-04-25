#include "pch.h"
#include "VulkanVertexBuffer.h"
#include "Mesh.h"

VulkanVertexBuffer::~VulkanVertexBuffer()
{}

VulkanVertexBuffer* VulkanVertexBuffer::CreateVertexBuffer(int vertexCount, VkDevice& device)
{
	VulkanVertexBuffer* vertexBuffer = new VulkanVertexBuffer();
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(Vertex) * vertexCount;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	
	if(vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer->m_vertexBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vertex buffer!");
		return nullptr;
	}

	return vertexBuffer;
}
