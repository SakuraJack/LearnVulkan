#pragma once
#include <vector>
#include "glm/glm/glm.hpp"

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;
};
class Mesh
{
public:
	Mesh() = default;
	Mesh(const std::vector<Vertex>& vertices);
	~Mesh();

	static std::shared_ptr<Mesh> Create(const std::vector<Vertex>& vertices);

private:
	std::vector<Vertex> m_vertices;
};
