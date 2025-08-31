#include <Aura.h>
#include <string>
#include <Entity/AEntityManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
void update(float deltaTime, Nyx::Window::Window* window)
{
}
int main()
{
	AURA_CORE_INFO("Aura Engine Initialized");
	Aura::Window::AWindow window("Aura Engine Window", 800, 600);
	Aura::Objects::ACubeObject cube("Cube1");
	cube.position.z = -2.0f;
	cube.rotation.x = 45.0f;
	window.update(update);
	return 0;
}