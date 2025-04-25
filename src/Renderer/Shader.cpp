#include "pch.h"
#include "Shader.h"

Shader::Shader(const std::string& path)
{
	std::cout << "Loading shader: " << path << std::endl;

	size_t found = path.find_last_of("/\\");
	m_Name = found != std::string::npos ? path.substr(found + 1) : path;
	found = path.find_last_of(".");
	m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;

	found = path.find_last_of("/\\");
	std::string extension = path.substr(found + 1);
	extension = extension.substr(extension.find_last_of("_") + 1);
	found = extension.find_last_of(".");
	extension = extension.substr(0, found);
	SetShaderType(extension);
	ReadFile(path);

	std::cout << "Shader loaded: " << m_Name << std::endl;
}

Shader::~Shader()
{

}

std::shared_ptr<Shader> Shader::Create(const std::string& path)
{
	return std::make_shared<Shader>(path);
}

void Shader::ReadFile(const std::string& path)
{
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	if(!file.is_open())
	{
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	size_t fileSize = (size_t)file.tellg();
	m_ShaderCode.resize(fileSize);

	file.seekg(0);
	file.read(m_ShaderCode.data(), fileSize);
	file.close();
}

void Shader::SetShaderType(const std::string& extension)
{
	if(extension == "vert")
		m_Type = ShaderType::Vertex;
	else if(extension == "frag")
		m_Type = ShaderType::Fragment;
	else if(extension == "comp")
		m_Type = ShaderType::Compute;
	else
		std::cout << "Unknown shader type" << std::endl;
}
