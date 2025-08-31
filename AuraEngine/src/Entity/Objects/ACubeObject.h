#pragma once
#include "../Entity.h"
#include "ABaseObject.h"
#include <memory>
#include <string>
#include "../AuraAPI.h"
#include "../Meshes/MeshesManager.h"
#include <Nyx/Renderer/GL/VAO.h>
namespace Aura
{
	namespace Objects
	{
		class AURA_API ACubeObject  : public ABaseObject
		{
		
		public:
			ACubeObject() = delete;
			~ACubeObject() = default;
			ACubeObject(const std::string& name, const std::string& tag = "object");
			std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO() override {
				std::shared_ptr<Nyx::Renderer::GL::VAO> vao;
				Aura::Meshes::AMeshManager::ACubeMesh()->getVAO(vao);
				return vao;
			}
		};
	
	}
}
