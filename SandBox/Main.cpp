#include <Aura.h>
#include <string>
#include <Scene/ASceneManager.h>

void update(float deltaTime, Nyx::Window::Window* window)
{
	auto* scene = Aura::Scene::ASceneManager::GetScene();
	static float rotation = 0.0f;
	rotation += 50.0f * deltaTime; // Rotate 50 degrees per second
	if (rotation > 360.0f)
		rotation -= 360.0f;
	auto* cube = scene->getEntityManager().findEntityByName("Cube1");
	if (cube) {
		auto* transform = cube->getComponent<Aura::Components::ATransformComponent>();
		if (transform) {
			transform->rotation.y = rotation;
		}
	}

}
int main()
{
	AURA_CORE_INFO("Aura Engine Initialized");
	Aura::Window::AWindow window("Aura Engine Window", 800, 600);
	Aura::Scene::AScene* scene = Aura::Scene::AScene::MakeScene();
	auto* cube = scene->AddObject<Aura::Objects::ACubeObject>("Cube1");
	auto* cube2 = scene->AddObject<Aura::Objects::ACubeObject>("Cube2");
	Aura::Scene::AScene* scene2 = Aura::Scene::AScene::MakeScene();
	auto* cube21 = scene2->AddObject<Aura::Objects::ACubeObject>("Cube1");
	auto* cube22 = scene2->AddObject<Aura::Objects::ACubeObject>("Cube2");
	auto* cube1Transform = cube21->getComponent<Aura::Components::ATransformComponent>();
	auto* cube2Transform = cube22->getComponent<Aura::Components::ATransformComponent>();
	cube1Transform->position.z = -5.0f;
	cube1Transform->position.x = -1.0f;
	cube2Transform->position.z = -5.0f;
	cube2Transform->position.x = 1.0f;
	Aura::Scene::ASceneManager::SetActiveScene(1);
	cube2->setVisible(true);
	window.update(update);
	return 0;
}