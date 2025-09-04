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
		class AURA_API ASphereObject : public ABaseObject
		{
		public:
			ASphereObject() = delete;
			~ASphereObject() = default;
			ASphereObject(const std::string& name, const std::string& tag = "object");
			std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO() override {
				std::shared_ptr<Nyx::Renderer::GL::VAO> vao;
				Aura::Meshes::AMeshManager::ASphereMesh()->getVAO(vao);
				return vao;
			}
		};

	}
}
