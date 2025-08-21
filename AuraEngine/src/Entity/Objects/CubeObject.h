#pragma once
#include "../Entity.h"
#include <memory>
#include <string>
#include "../AuraAPI.h"
#include "../Meshes/MeshesManager.h"
#include <Nyx/Renderer/GL/VAO.h>
namespace Aura
{
	namespace Objects
	{
		class AURA_API CubeObject : public Entity
		{
		
		public:
			CubeObject() = default;
			~CubeObject();
			CubeObject(const std::string& name, const std::string& tag = "object");
			CubeObject(const std::string& name);
			std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO() override {
				std::shared_ptr<Nyx::Renderer::GL::VAO> vao;
				Aura::Meshes::AMeshManager::ACubeMesh()->getVAO(vao);
				return vao;
			}
		};
	
	}
}
