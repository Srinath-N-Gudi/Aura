#pragma once


#include <Nyx/Renderer/GL/Renderer.h>
#include "../Entity/Entitiy.h"





namespace Aura {
	class ARenderer {
	public:
		ARenderer();
		~ARenderer();
		void draw();
	private:
		Nyx::Renderer::GL::Renderer* m_Renderer;
	};
} // namespace Aura
