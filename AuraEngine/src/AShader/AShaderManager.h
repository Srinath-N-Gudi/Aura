#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <Nyx/Renderer/GL/Shader.h>

namespace Aura {

    class AShaderManager {
    public:
        // Singleton access
        static AShaderManager& Get();

        // Delete copy/move
        AShaderManager(const AShaderManager&) = delete;
        AShaderManager& operator=(const AShaderManager&) = delete;

        // Load a shader by name (compiles only if not already loaded)
        Nyx::Renderer::GL::Shader* Load(const std::string& name,
            const std::string& vertexPath,
            const std::string& fragmentPath);

        // Retrieve a shader by name (throws if not loaded)
        Nyx::Renderer::GL::Shader* Get(const std::string& name);

        // Check if a shader exists
        bool Exists(const std::string& name) const;

        // Remove a shader manually
        void Remove(const std::string& name);

        // Cleanup all shaders (call on engine shutdown)
        void Cleanup();

    private:
        // Private constructor for singleton
        AShaderManager() = default;
        ~AShaderManager() = default;

        std::unordered_map<std::string, std::unique_ptr<Nyx::Renderer::GL::Shader>> shaders;
    };

} // namespace Aura
