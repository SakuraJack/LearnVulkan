#include "pch.h"
#include "Mesh.h" 

Mesh::Mesh(const std::vector<Vertex>& vertices)
	: m_vertices(vertices)
{

}

Mesh::~Mesh()
{

}

std::shared_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices)
{
	return std::make_shared<Mesh>(vertices);
}
