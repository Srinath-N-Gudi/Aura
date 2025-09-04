#include <Aura.h>
#include <Scene/ASceneManager.h>
#include <Entity/Components/AMaterialComponent.h>
#include <cstdlib>
#include <ctime>
#include <vector>

struct Orbit {
    Aura::Components::ATransformComponent* body;
    Aura::Components::ATransformComponent* parent;
    float radius;
    float speed; // degrees per second
    float angle;
    float tilt;
};

std::vector<Orbit> orbits;

void update(float deltaTime, Nyx::Window::Window* window) {
    for (auto& orbit : orbits) {
        orbit.angle += glm::radians(orbit.speed) * deltaTime;
        float x = orbit.parent->position.x + orbit.radius * cos(orbit.angle);
        float z = orbit.parent->position.z + orbit.radius * sin(orbit.angle);
        orbit.body->position = glm::vec3(x, 0.0f, z);
        orbit.body->rotation.y += glm::radians(30.0f) * deltaTime; // self rotation
    }
}

int main() {
    AURA_CORE_INFO("Aura Engine - Solar System");
    Aura::Window::AWindow window("Solar System in Aura", 1600, 900);

    Aura::Scene::AScene* scene = Aura::Scene::AScene::MakeScene();

    // Sun
    Aura::Components::AMaterial* sunMat = Aura::Components::AMaterial::MakeMaterial();
    sunMat->setAlbedoMap("C:/Users/Srinath/Downloads/sun.jpg");
    auto* sun = scene->AddObject<Aura::Objects::ASphereObject>("sun");
    sun->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*sunMat);
    auto* sunTransform = sun->getComponent<Aura::Components::ATransformComponent>();
    sunTransform->scale = glm::vec3(10.0f);
    sunTransform->position = glm::vec3(0.0f, 0.0f, -50.0f);

    // Planets setup
    struct PlanetDef {
        std::string name;
        std::string texture;
        float scale;
        float orbitRadius;
        float orbitSpeed;
    };

    std::vector<PlanetDef> planets = {
        {"mercury", "C:/Users/Srinath/Downloads/mercury.jpg", 0.4f, 15.0f, 47.0f},
        {"venus",   "C:/Users/Srinath/Downloads/venus.jpg",   0.95f, 20.0f, 35.0f},
        {"earth",   "C:/Users/Srinath/Downloads/earth.jpg",   1.0f, 30.0f, 29.0f},
        {"mars",    "C:/Users/Srinath/Downloads/mars.jpg",    0.53f, 40.0f, 24.0f},
        {"jupiter", "C:/Users/Srinath/Downloads/jupiter.jpg", 11.2f, 60.0f, 13.0f},
        {"saturn",  "C:/Users/Srinath/Downloads/saturn.jpg",  9.45f, 80.0f, 9.7f},
        {"uranus",  "C:/Users/Srinath/Downloads/uranus.jpg",  4.0f, 100.0f, 6.8f},
        {"neptune", "C:/Users/Srinath/Downloads/neptune.jpg", 3.9f, 120.0f, 5.4f}
    };

    for (auto& def : planets) {
        Aura::Components::AMaterial* mat = Aura::Components::AMaterial::MakeMaterial();
        mat->setAlbedoMap(def.texture);
        auto* planet = scene->AddObject<Aura::Objects::ASphereObject>(def.name);
        planet->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*mat);
        auto* transform = planet->getComponent<Aura::Components::ATransformComponent>();
        transform->scale = glm::vec3(def.scale);
        transform->position = glm::vec3(sunTransform->position.x + def.orbitRadius, 0.0f, sunTransform->position.z);

        orbits.push_back({ transform, sunTransform, def.orbitRadius, def.orbitSpeed, 0.0f, 0.0f });
    }

    Aura::Scene::ASceneManager::SetActiveScene(0);
    window.update(update);
    return 0;
}
