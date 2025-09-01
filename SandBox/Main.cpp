#include <Aura.h>
#include <string>
#include <Scene/ASceneManager.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
void update(float deltaTime, Nyx::Window::Window* window)
{
	auto* scene = Aura::Scene::ASceneManager::GetScene();
	static float rotation = 0.0f;
	rotation += 50.0f * deltaTime; // Rotate 50 degrees per second
	if (rotation > 360.0f)
		rotation -= 360.0f;
	auto* cube = scene->getEntityManager().findEntityByName("Cube1");
	if (cube) {
		auto* transform = dynamic_cast<Aura::Components::ATransformComponent*>(cube);
		if (transform) {
			transform->rotation.y = rotation;
		}
	}

}
int main()
{
	AURA_CORE_INFO("Aura Engine Initialized");
	Aura::Window::AWindow window("Aura Engine Window", 800, 600);
	Aura::Scene::AScene scene;
	auto* cube = scene.AddObject<Aura::Objects::ACubeObject>("Cube1");
	auto* cube2 = scene.AddObject<Aura::Objects::ACubeObject>("Cube2");
	//Aura::Scene::ASceneManager::SetActiveScene(0);
	cube->position.z = -5.0f;
	cube->position.x = -1.0f;
	cube2->position.z = -5.0f;
	cube2->position.x = 1.0f;
	cube2->setVisible(false);
	window.update(update);
	return 0;
}