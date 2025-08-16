#include <glad/glad.h>
#include <Nyx/Window.h>
#include "AuraWindow.h"
#include "AuraLogger.h"
#include "Globals.h" 
#include "ARenderer/ARenderer.h"
#include <cassert>

namespace Aura {
	namespace Window {
		AWindow::AWindow(const char* title, int width, int height)
		{
			AURA_CORE_INFO("Created Aura Window: {} ({}x{})", title, width, height);
			m_Window = new Nyx::Window::Window(title, width, height, m_WindowConfig);
			AURA_CORE_INFO("Window name :  {}", m_Window->getWindowTitle());
		}

		AWindow::~AWindow() {
			delete m_Window;
			AURA_CORE_INFO("Destroyed Aura Window: {}", m_Window->getWindowTitle());
		}
		
		void AWindow::update(UpdateCallBack callback)
		{
			float prevTime = 0.0f;
			if (!gladLoadGL())
				AURA_CORE_FATAL("Failed to initialize OpenGL context with glad"); 
			glEnable(GL_DEPTH_TEST);
			while (!m_Window->windowClosed())
			{
				float currTime = glfwGetTime();
				float deltaTime = currTime - prevTime;
				prevTime = currTime;
				if (callback)
				{
					callback(deltaTime);
				}
				Renderer.draw();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set clear color to dark gray
				m_Window->update();


			}
		}
	} // namespace Window
} // namespace Aura