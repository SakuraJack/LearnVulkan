#pragma once

enum class ShaderType
{
	Vertex,
	Fragment,
	Geometry,
	Compute
};

class Shader
{
public:
	Shader(const std::string& path);
	~Shader();

	inline ShaderType GetType() { return m_Type; }
	inline const ShaderType GetType() const { return m_Type; }
	inline std::string& GetName() { return m_Name; }
	inline const std::string& GetName() const { return m_Name; }

	inline const std::vector<char>& GetShaderCode() const { return m_ShaderCode; }

	static std::shared_ptr<Shader> Create(const std::string& path);
private:
	void ReadFile(const std::string& path);
	void SetShaderType(const std::string& extension);
private:
	std::string m_Name;
	ShaderType m_Type;
	std::vector<char> m_ShaderCode;
};