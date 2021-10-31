#include "Shader Library.hpp"

ShaderLibrary& ShaderLibrary::Get() {
	return s_instance;
}

void ShaderLibrary::Clear() {
	for (const auto& s : m_Shaders)
		delete s.second;
}

void ShaderLibrary::LoadShader(const std::string& name, const std::string& fragment, const std::string& vertex) {
	sf::Shader* shader = new sf::Shader();
	shader->loadFromFile(vertex, fragment);

	m_Shaders[name] = shader;
}

sf::Shader& ShaderLibrary::GetShader(const std::string& name) {
	return *m_Shaders[name];
}

ShaderLibrary ShaderLibrary::s_instance;