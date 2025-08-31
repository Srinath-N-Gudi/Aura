#include "AShaderManager.h"
#include "../AuraLogger.h"
#include <filesystem>
#include <stdexcept>
#include <iostream>

namespace Aura {

    // Singleton access
    AShaderManager& AShaderManager::Get() {
        static AShaderManager instance;
        return instance;
    }

    // Load a shader by name
    Nyx::Renderer::GL::Shader* AShaderManager::Load(const std::string& name,
        const std::string& vertexPath,
        const std::string& fragmentPath)
    {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            return it->second.get(); // Already loaded
        }

        if (!std::filesystem::exists(vertexPath)) {
			AURA_CORE_FATAL("[AShaderManager] : Vertex shader file does not exist: {}", vertexPath);
            throw std::runtime_error("Vertex shader file does not exist: " + vertexPath);

        }
        if (!std::filesystem::exists(fragmentPath)) {
			AURA_CORE_FATAL("[AShaderManager] : Fragment shader file does not exist: {}", fragmentPath);
            throw std::runtime_error("Fragment shader file does not exist: " + fragmentPath);
        }

        auto shader = std::make_unique<Nyx::Renderer::GL::Shader>(vertexPath, fragmentPath);
        Nyx::Renderer::GL::Shader* shaderPtr = shader.get();
        shaders[name] = std::move(shader);

		AURA_CORE_INFO("[AShaderManager] : Loaded shader: {}", name);
        return shaderPtr;
    }

    // Retrieve a shader by name
    Nyx::Renderer::GL::Shader* AShaderManager::Get(const std::string& name) {
        auto it = shaders.find(name);
        if (it == shaders.end()) {
			AURA_CORE_FATAL("[AShaderManager] : Shader '{}' not loaded!", name);
            throw std::runtime_error("Shader '" + name + "' not loaded!");
        }
        return it->second.get();
    }

    // Check if a shader exists
    bool AShaderManager::Exists(const std::string& name) const {
        return shaders.find(name) != shaders.end();
    }

    // Remove a shader manually
    void AShaderManager::Remove(const std::string& name) {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            shaders.erase(it);
			AURA_CORE_INFO("[AShaderManager] : Removed shader: {}", name);
        }
    }

    // Cleanup all shaders
    void AShaderManager::Cleanup() {
        shaders.clear();
		AURA_CORE_INFO("[AShaderManager] : Cleared all shaders.");
		AURA_CORE_WARN("No Shaders exist, this might break the working of the Aura Engine");
    }

} // namespace Aura
