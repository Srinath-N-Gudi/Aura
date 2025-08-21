#pragma once


#include <Nyx/Renderer/GL/Renderer.h>
#include "../Entity/Entity.h"





namespace Aura {
	class ARenderer {
	public:
		static void draw();
		static ARenderer& Get();
			
	private:
		ARenderer();
		void Idraw();
		~ARenderer();
		Nyx::Renderer::GL::Renderer* m_Renderer;
	};
} // namespace Aura
