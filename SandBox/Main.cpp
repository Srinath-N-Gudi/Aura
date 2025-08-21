#include <Aura.h>
#include <string>
#include <Entity/AEntityManager.h>

void update(float deltaTime)
{
	//AURA_INFO("Total Entities : {}", Aura::AEntityManager::getEntityCount());
}
int main()
{
	AURA_CORE_INFO("Aura Engine Initialized");
	Aura::Window::AWindow window("Aura Engine Window", 800, 600);
	Aura::Objects::CubeObject cube("Cube1");
	window.update(update);
	return 0;
}