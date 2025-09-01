#pragma once  

#include <Nyx/Renderer/GL/Renderer.h>  
#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>  
#include "../Entity/Entity.h"  

namespace Aura { 
	class AURA_API ARenderer {
	public:
		  static void draw();
		  static ARenderer& Get();
		  // Rendering stuff for our engine  
		  static bool setSameProjectionForAll;
		  glm::mat4 defaultProjection = glm::mat4(1.0f);


		  static inline void setDefaultProjectionMatrix(glm::mat4 projection)
		  {
			  Get().defaultProjection = projection;
		  }
	private:
		ARenderer() = default;
		void Idraw();
		~ARenderer();
		Nyx::Renderer::GL::Renderer* m_Renderer;
	};
} // namespace Aura
