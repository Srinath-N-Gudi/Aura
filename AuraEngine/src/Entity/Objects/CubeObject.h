#pragma once
#include "../Entitiy.h"
#include <memory>
#include <string>
#include "../AuraAPI.h"
#include "../Globals.h"
#include "../Meshes/CubeMesh.h"
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
				ACubeMesh.getVAO(vao);
				return vao;
			}
		};
	
	}
}
