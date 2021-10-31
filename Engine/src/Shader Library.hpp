#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <unordered_map>

class ShaderLibrary {
public:
	ShaderLibrary(const ShaderLibrary&) = delete;

	static ShaderLibrary& Get();

	void Clear();

	void LoadShader(const std::string& name, const std::string& fragment, const std::string& vertex);

	sf::Shader& GetShader(const std::string& name);

private:
	ShaderLibrary() {}
	~ShaderLibrary() {}

	std::unordered_map<std::string, sf::Shader*> m_Shaders;

	static ShaderLibrary s_instance;
};