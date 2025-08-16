#pragma once
#include <Nyx/Window.h>
#include "AuraAPI.h"
#include <functional>
namespace Aura
{
	namespace Window
	{
		class AURA_API AWindow
		{
		private:
			Nyx::Window::WindowConfig m_WindowConfig = {
				.glMajorVersion = 4,
				.glMinorVersion = 5,
				.coreProfile = true,
				.debugContext = false,
				.resizable = true
			};
			Nyx::Window::Window* m_Window;
			using UpdateCallBack = std::function<void(float deltaTime)>;
			
		public:
			AWindow(const char* title, int width, int height);
			~AWindow();
			// write update function defintion which takes the UpdateCallBack as a parameter
			
			void update(UpdateCallBack callback);

			inline Nyx::Window::Window* getNyxWindow() const { return m_Window; }
			inline int getWidth() const { return m_Window->getWidth(); }
			inline int getHeight() const { return m_Window->getHeight(); }
			inline const char* getTitle() const { return m_Window->getWindowTitle(); }
			inline bool isClosed() const { return m_Window->windowClosed(); }
			inline bool isMinimized() const { return m_Window->isMinimized(); }
		};

	}


}

