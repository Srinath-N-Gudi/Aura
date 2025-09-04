#include <glad/glad.h>
#include <Nyx/Window.h>
#include "AuraWindow.h"
#include "AURA_PATHS.h"
#include "AuraLogger.h"
#include "ARenderer/ARenderer.h"
#include "Entity/AEntityManager.h"
#include "Entity/Meshes/MeshesManager.h"
#include "AShader/AShaderManager.h"
#include <cassert>
#include <glm/glm.hpp>




namespace Aura {
	namespace Window {
		AWindow::AWindow(const char* title, int width, int height)
		{
			AURA_CORE_INFO("Created Aura Window: {} ({}x{})", title, width, height);
			m_Window = new Nyx::Window::Window(title, width, height, m_WindowConfig);
			AURA_CORE_INFO("Window name :  {}", m_Window->getWindowTitle());
			Init();
		}

		AWindow::~AWindow() {

			delete m_Window;
			AURA_CORE_INFO("Destroyed Aura Window: ");


		}
		
		void AWindow::update(UpdateCallBack callback)
		{
			float prevTime = 0.0f;
			m_Window->setResizeCallback([](int width, int height) {
				glViewport(0, 0, width, height);
				});
			glEnable(GL_DEPTH_TEST);
			while (!m_Window->windowClosed())
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set clear color to dark gray

				float currTime = glfwGetTime();
				float deltaTime = currTime - prevTime;
				prevTime = currTime;
				if (callback)
				{
					callback(deltaTime, this->m_Window);
				}
				ARenderer::draw();
				m_Window->update();

			}
		}
		void AWindow::Init()
		{
			if (!gladLoadGL())
			{
				AURA_CORE_FATAL("Failed to initialize GLAD");
				return;
			}
			AURA_CORE_INFO("GLAD initialized successfully");
			AURA_CORE_INFO("OpenGL Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
			AURA_CORE_INFO("OpenGL Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
			AURA_CORE_INFO("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
			AURA_CORE_INFO("OpenGL GLSL Version: {}", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
			GLint texSize;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize);
			AURA_CORE_INFO("Maximum texture size : {}", texSize);
			GLint maxAttribs;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
			AURA_CORE_INFO("Maximum Vertex Array : {}", maxAttribs);
			GLint n;
			glGetIntegerv(GL_NUM_EXTENSIONS, &n);
			for (int i = 0; i < n; i++) {
				AURA_CORE_INFO("OpenGL Extension {} : {}", i + 1, reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
			}
			AURA_CORE_INFO("Loading Shaders from : {}", PATH(""));
			assert(AShaderManager::Get().Load("basic", PATH("basic.vert"), PATH("basic.frag")) && "Failed to load basic shader");
			ARenderer::setDefaultProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)m_Window->getWidth() / m_Window->getHeight(), 0.1f, 1000.0f));
			Meshes::AMeshManager::GenerateMeshes();
		}
	} // namespace Window
} // namespace Aura