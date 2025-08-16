#include <Aura.h>
#include <string>
void update(float deltaTime)
{
}
int main()
{
	Aura::Logger::Init();
	AURA_CORE_INFO("Aura Engine Initialized");
	Aura::Window::AWindow window("Aura Engine Window", 800, 600);
	window.update(update);

	return 0;
}