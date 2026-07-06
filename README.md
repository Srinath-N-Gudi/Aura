# Aura Engine

A lightweight 3D game engine built in C++20 on OpenGL 4.5, designed for simplicity and extensibility.

**License:** MIT | **Author:** Srinath-N-Gudi | **Platform:** Windows (MSVC 2022)

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Getting Started](#getting-started)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Window](#window)
  - [Scene Management](#scene-management)
  - [Entity System](#entity-system)
  - [Components](#components)
  - [Objects](#objects)
  - [Materials](#materials)
  - [Shaders](#shaders)
  - [Meshes](#meshes)
  - [Logging](#logging)
- [Writing Custom Shaders](#writing-custom-shaders)
- [Tutorial: Building a Solar System](#tutorial-building-a-solar-system)
- [Roadmap](#roadmap)
- [License](#license)

---

## Overview

Aura is a 3D game engine that wraps the Nyx abstraction library to provide a high-level, easy-to-use API for creating 3D applications. It handles windowing, OpenGL context setup, shader compilation, mesh generation, entity management, and rendering — so you can focus on building your game.

Aura is built as a **Windows DLL** (`AuraEngine.dll`), letting client applications link against it dynamically.

### Technology Stack

| Layer | Technology |
|---|---|
| **Engine** | Aura (C++20) |
| **Abstraction** | Nyx (submodule) |
| **Graphics** | OpenGL 4.5 Core Profile |
| **Windowing** | GLFW 3 |
| **Math** | GLM |
| **GL Loader** | GLAD |
| **Build System** | Visual Studio 2022 |

---

## Features

- **OpenGL 4.5 Core Profile** rendering with GLAD-loaded function pointers
- **Entity-Component System** — attach `TransformComponent`, `MaterialComponent`, or custom components to any entity
- **Built-in primitives** — procedural Cube and UV-Sphere meshes
- **Texture mapping** — load JPEG/PNG textures onto materials
- **Solid color materials** — assign RGB albedo colors without textures
- **Shader management** — load, cache, and retrieve GLSL shaders by name
- **Scene management** — create multiple scenes, switch active scene at runtime
- **Discrete GPU forcing** — automatically selects NVIDIA/AMD dedicated GPU on hybrid systems
- **Thread-safe logging** — color-coded, timestamped console output with log levels
- **Delta-time game loop** — automatic frame timing for smooth updates

---

## Architecture

Aura uses a **three-layer wrapper pattern**:

```
Your Game Code
      |
      v
  Aura Engine         (high-level API: Window, Scene, Entity, Component, Renderer)
      |
      v
  Nyx Library         (mid-level: OpenGL wrappers for VAO, VBO, IBO, Shader, Texture2D)
      |
      v
  OpenGL / GLFW / GLAD (low-level graphics and windowing)
```

### Design Patterns

| Pattern | Usage |
|---|---|
| **Singleton** | `ARenderer`, `AShaderManager`, `AMeshManager`, `ASceneManager` |
| **Entity-Component** | `Entity` + `AComponentHelper` with type-erased storage |
| **Factory Method** | `AScene::AddObject<T>()`, `AMaterial::MakeMaterial()`, `AScene::MakeScene()` |
| **Facade** | `AWindow` wraps `Nyx::Window::Window` behind a simplified interface |
| **Manager** | Dedicated managers for scenes, entities, shaders, and meshes |

### Class Hierarchy

```
ABaseComponent
  |-- ATransformComponent
  |-- AMaterialComponent

AComponentHelper (mixin — provides add/get/remove components)
  |
  Entity (name, tag, ID, visibility, lifecycle)
    |-- ABaseObject (auto-adds TransformComponent)
        |-- ACubeObject (renders CubeMesh)
        |-- ASphereObject (renders SphereMesh)
```

---

## Getting Started

### Prerequisites

- **Visual Studio 2022** (v143 toolset, C++20 support)
- **Windows SDK** (for Windows.h, DllMain)
- **Git** (with submodule support)

### Building

1. **Clone the repository with submodules:**

   ```bash
   git clone --recurse-submodules https://github.com/Srinath-N-Gudi/Aura.git
   cd Aura
   ```

   If you already cloned without submodules:

   ```bash
   git submodule update --init --recursive
   ```

2. **Open the solution in Visual Studio 2022:**

   Open the `.sln` file (not tracked in git — create it in Visual Studio by adding the existing projects).

3. **Ensure `AURA_PATHS.h` exists:**

   Create a file at `AuraEngine/src/AURA_PATHS.h` that defines the `PATH` macro used to locate shader files at runtime:

   ```cpp
   #pragma once
   #define PATH(x) "C:/Programs/Projects/Aura/AuraEngine/src/shaders/" x
   ```

   Adjust the path to match your local setup.

4. **Build the solution:**

   - Build configuration: **Release** or **Debug**
   - Platform: **x64**
   - Output: `AuraEngine.dll` + `AuraEngine.lib` (import library)

### Project Structure

```
Aura/
├── AuraEngine/          # Engine DLL source
│   └── src/
│       ├── Aura.h               # Master include header
│       ├── Aura.cpp             # DllMain entry point
│       ├── AuraAPI.h            # DLL export/import macros
│       ├── AuraWindow.h/.cpp    # Window management
│       ├── AuraLogger.h/.cpp    # Logging system
│       ├── DGPU.h               # Discrete GPU selection
│       ├── ARenderer/           # Rendering subsystem
│       ├── AShader/             # Shader management
│       ├── Entity/              # ECS: entities, components, meshes, objects
│       ├── Scene/               # Scene management
│       └── shaders/             # GLSL shader source files
├── SandBox/             # Demo application (Solar System)
│   └── Main.cpp
├── AuraEditor/          # Editor (planned, currently empty)
│   └── Source.cpp
├── Vendors/             # Third-party libraries
│   ├── include/         # Headers: GLFW, GLAD, GLM, KHR, ImGui
│   └── libs/            # Pre-compiled GLFW libs for MSVC 2022
├── LICENSE
└── README.md
```

---

## Quick Start

Here is the minimal code to open a window and render a spinning textured cube:

```cpp
#include <Aura.h>
#include <Scene/ASceneManager.h>
#include <Entity/Components/AMaterialComponent.h>

void update(float deltaTime, Nyx::Window::Window* window) {
    // Game logic goes here
}

int main() {
    // Create a window (initializes OpenGL, GLAD, shaders, meshes)
    Aura::Window::AWindow window("My Game", 1280, 720);

    // Create a scene (auto-registers with SceneManager)
    Aura::Scene::AScene* scene = Aura::Scene::AScene::MakeScene();

    // Spawn a cube
    auto* cube = scene->AddObject<Aura::Objects::ACubeObject>("my_cube");
    cube->getComponent<Aura::Components::ATransformComponent>()->position = glm::vec3(0, 0, -5);

    // Give it a red color
    auto* mat = Aura::Components::AMaterial::MakeMaterial();
    mat->albedoColor = glm::vec3(1.0f, 0.0f, 0.0f);
    cube->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*mat);

    // Set active scene and run
    Aura::Scene::ASceneManager::SetActiveScene(0);
    window.update(update);
    return 0;
}
```

---

## API Reference

### Window

**Header:** `AuraWindow.h` | **Namespace:** `Aura::Window`

The `AWindow` class creates and manages an OpenGL window. On construction it initializes GLAD, loads the default shader, sets up a perspective projection, and generates built-in meshes.

```cpp
namespace Aura::Window {
    class AWindow {
    public:
        // Create a window with title, width, and height
        AWindow(const char* title, int width, int height);
        ~AWindow();

        // Start the render loop. The callback receives deltaTime and the Nyx window pointer.
        using UpdateCallBack = std::function<void(float deltaTime, Nyx::Window::Window* window)>;
        void update(UpdateCallBack callback);

        // Accessors
        Nyx::Window::Window* getNyxWindow() const;
        int getWidth() const;
        int getHeight() const;
        const char* getTitle() const;
        bool isClosed() const;
        bool isMinimized() const;
    };
}
```

**Usage:**

```cpp
Aura::Window::AWindow window("My Game", 1920, 1080);
window.update([](float dt, Nyx::Window::Window* w) {
    // Called every frame before rendering
});
```

The `update()` method runs the main game loop:
1. Clears the framebuffer
2. Computes delta time
3. Calls your callback
4. Calls `ARenderer::draw()` to render all visible entities
5. Swaps buffers and polls events

---

### Scene Management

**Headers:** `Scene/Scene.h`, `Scene/ASceneManager.h` | **Namespace:** `Aura::Scene`

Scenes are containers for entities. You can create multiple scenes and switch between them at runtime.

#### AScene

```cpp
class AScene {
public:
    static AScene* MakeScene();  // Factory — auto-registers with SceneManager

    // Add an object of the given type to the scene
    template <typename ObjectType>
    ObjectType* AddObject(const std::string& name, std::string tag = "object");

    AEntityManager& getEntityManager();
    int getID() const;
};
```

#### ASceneManager (Singleton)

```cpp
class ASceneManager {
public:
    static ASceneManager& Get();

    static void AddScene(AScene* scene);       // Register a scene
    static void SetActiveScene(int id);         // Set which scene is rendered
    static int GetActiveScene();                // Get active scene ID
    static AScene* GetScene(int id);            // Get scene by ID
    static AScene* GetScene();                  // Get currently active scene
};
```

**Usage:**

```cpp
// Create scenes
Aura::Scene::AScene* scene1 = Aura::Scene::AScene::MakeScene();
Aura::Scene::AScene* scene2 = Aura::Scene::AScene::MakeScene();

// Add objects to scene1
auto* cube = scene1->AddObject<Aura::Objects::ACubeObject>("cube");

// Switch to scene1
Aura::Scene::ASceneManager::SetActiveScene(0);  // scene1 has ID 0
```

---

### Entity System

**Headers:** `Entity/Entity.h`, `Entity/AEntityManager.h` | **Namespace:** `Aura`

Entities are the fundamental game objects. They have a name, tag, unique ID, visibility flag, and can hold components.

#### Entity

```cpp
class Entity : public AComponentHelper {
public:
    virtual ~Entity() = default;

    // Override to provide custom geometry (returns nullptr by default)
    virtual std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO();

    // Properties
    std::string getName() const;
    void setName(const std::string& name);
    std::string getTag() const;
    void setTag(const std::string& tag);
    int getID() const;
    bool isVisible() const;
    bool isAlive() const;
    void setVisible(bool value);

    // Component methods (inherited from AComponentHelper)
    template <typename T> T* getComponent();
    template <typename T> T* addComponent();
    template <typename T> void removeComponent();
    template <typename T> bool hasComponent();
};
```

#### AEntityManager (Singleton)

```cpp
class AEntityManager {
public:
    void addEntity(Entity* entity);
    Entity* findEntityByID(int id);
    Entity* findEntityByName(const std::string& name);
    std::vector<Entity*> findEntitiesByTag(const std::string& tag);
    void clearEntities();

    std::unordered_map<int, Entity*> m_Entities;  // ID -> Entity
};
```

Each entity gets an auto-incrementing ID when added to the manager.

---

### Components

**Header:** `Entity/Components/AComponentHelper.h` | **Namespace:** `Aura`

Components are attached to entities to give them data and behavior. The engine provides two built-in components:

#### AComponentHelper (Mixin)

This mixin is inherited by `Entity` and provides the component storage system:

```cpp
// On any Entity instance:
auto* transform = entity->getComponent<Aura::Components::ATransformComponent>();
auto* material = entity->addComponent<Aura::Components::AMaterialComponent>();
entity->removeComponent<Aura::Components::AMaterialComponent>();
bool has = entity->hasComponent<Aura::Components::ATransformComponent>();
```

Components are stored internally using `std::type_index` as the key, allowing type-safe access without virtual dispatch.

#### ATransformComponent

Controls position, rotation, and scale of an entity.

```cpp
class ATransformComponent : public ABaseComponent {
public:
    glm::vec3 position;  // World position (default: 0,0,0)
    glm::vec3 rotation;  // Euler angles in degrees: pitch(X), yaw(Y), roll(Z) (default: 0,0,0)
    glm::vec3 scale;     // Scale factors (default: 1,1,1)
};
```

The model matrix is computed as: **Translate -> Rotate(X) -> Rotate(Y) -> Rotate(Z) -> Scale**

Every `ABaseObject` automatically gets a `ATransformComponent` when constructed.

**Usage:**

```cpp
auto* transform = entity->getComponent<Aura::Components::ATransformComponent>();
transform->position = glm::vec3(5.0f, 0.0f, -10.0f);
transform->rotation = glm::vec3(0.0f, 45.0f, 0.0f);  // 45 degrees around Y
transform->scale = glm::vec3(2.0f);  // Uniform scale
```

#### AMaterialComponent

Controls the visual appearance of an entity — either a solid albedo color or a texture map.

```cpp
class AMaterialComponent : public ABaseComponent {
public:
    AMaterialComponent();

    glm::vec3 getAlbedoColor() const;
    void setAlbedoColor(const glm::vec3& color);

    std::shared_ptr<Nyx::Renderer::GL::Texture2D> getAlbedoMap() const;
    void setAlbedoMap(const std::string& pathToTextureFile);
    void setAlbedoMap(std::shared_ptr<Nyx::Renderer::GL::Texture2D>& texture);

    void setMaterial(const AMaterial& material);
    AMaterial getMaterial() const;
};
```

---

### Objects

**Header:** `Entity/Objects/` | **Namespace:** `Aura::Objects`

Objects are concrete entity types that provide specific geometry.

#### ABaseObject

The base class for all renderable objects. Automatically adds a `ATransformComponent` in its constructor.

```cpp
class ABaseObject : public Entity {
public:
    ABaseObject();           // Adds ATransformComponent
    virtual ~ABaseObject();  // Marks entity as dead
    void setVisible(bool value);
};
```

#### ACubeObject

Renders a unit cube (24 vertices, 36 indices).

```cpp
class ACubeObject : public ABaseObject {
public:
    ACubeObject(const std::string& name, const std::string& tag = "object");
    std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO() override;  // Returns CubeMesh VAO
};
```

#### ASphereObject

Renders a UV sphere (procedurally generated, 32 segments).

```cpp
class ASphereObject : public ABaseObject {
public:
    ASphereObject(const std::string& name, const std::string& tag = "object");
    std::shared_ptr<Nyx::Renderer::GL::VAO> getVAO() override;  // Returns SphereMesh VAO
};
```

**Usage:**

```cpp
auto* cube = scene->AddObject<Aura::Objects::ACubeObject>("player", "entity");
auto* sphere = scene->AddObject<Aura::Objects::ASphereObject>("sun", "celestial");
```

---

### Materials

**Header:** `Entity/Components/AMaterialComponent.h` | **Namespace:** `Aura::Components`

The `AMaterial` struct holds the raw material data:

```cpp
struct AMaterial {
    glm::vec3 albedoColor = glm::vec3(1.0f);               // White by default
    std::shared_ptr<Nyx::Renderer::GL::Texture2D> albedoMap; // Optional texture

    void setAlbedoMap(const std::string& pathToTextureFile);
    static AMaterial* MakeMaterial();  // Factory
};
```

**Usage — Solid Color:**

```cpp
auto* mat = Aura::Components::AMaterial::MakeMaterial();
mat->albedoColor = glm::vec3(0.0f, 1.0f, 0.0f);  // Green
entity->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*mat);
```

**Usage — Texture:**

```cpp
auto* mat = Aura::Components::AMaterial::MakeMaterial();
mat->setAlbedoMap("C:/path/to/texture.jpg");
entity->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*mat);
```

If a texture is set, it takes priority over the albedo color. Supported formats depend on the Nyx image loader (typically JPEG, PNG, BMP, TGA).

---

### Shaders

**Header:** `AShader/AShaderManager.h` | **Namespace:** `Aura`

The shader manager handles loading, compiling, and caching GLSL shader programs.

#### AShaderManager (Singleton)

```cpp
class AShaderManager {
public:
    static AShaderManager& Get();

    // Load and compile a shader from vertex/fragment GLSL files
    Nyx::Renderer::GL::Shader* Load(
        const std::string& name,
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

    // Retrieve a previously loaded shader by name
    Nyx::Renderer::GL::Shader* Get(const std::string& name);

    // Check if a shader is loaded
    bool Exists(const std::string& name) const;

    // Remove a shader
    void Remove(const std::string& name);

    // Cleanup all shaders
    void Cleanup();
};
```

The engine auto-loads a shader named `"basic"` on startup from `basic.vert` and `basic.frag`.

**Usage — Loading a Custom Shader:**

```cpp
// Load a shader
Aura::AShaderManager::Get().Load("my_shader", "path/to/vert.glsl", "path/to/frag.glsl");

// Use it later
auto* shader = Aura::AShaderManager::Get().Get("my_shader");
shader->bind();
shader->setUniform3f("myColor", 1.0f, 0.0f, 0.0f);
```

---

### Meshes

**Header:** `Entity/Meshes/MeshesManager.h` | **Namespace:** `Aura::Meshes`

The mesh manager generates and holds built-in geometry. Meshes are generated automatically when `AWindow` is created.

#### AMeshManager (Singleton)

```cpp
class AMeshManager {
public:
    static AMeshManager& Get();
    static void GenerateMeshes();    // Creates CubeMesh and SphereMesh
    static CubeMesh* ACubeMesh();    // Access the cube mesh
    static SphereMesh* ASphereMesh(); // Access the sphere mesh
};
```

#### CubeMesh

A unit cube with 24 vertices (4 per face) and 36 indices. Vertex layout: position (3 floats) + normal (3 floats) + UV (2 floats) = 8 floats per vertex.

#### SphereMesh

A procedurally generated UV sphere with configurable segments (default: 32). Same interleaved vertex layout as the cube.

**Vertex Layout (both meshes):**

| Offset | Attribute | Size |
|---|---|---|
| 0 | Position (x, y, z) | 3 floats |
| 3 | Normal (nx, ny, nz) | 3 floats |
| 6 | Texture Coord (u, v) | 2 floats |

Total stride: 32 bytes (8 floats x 4 bytes)

---

### Logging

**Header:** `AuraLogger.h` | **Namespace:** `Aura`

Thread-safe, color-coded, timestamped logging with C++20 `std::format` syntax.

#### Log Levels

| Level | Macro (Core) | Macro (App) | Color |
|---|---|---|---|
| Trace | `AURA_CORE_TRACE` | `AURA_TRACE` | Dark Gray |
| Info | `AURA_CORE_INFO` | `AURA_INFO` | Green |
| Warn | `AURA_CORE_WARN` | `AURA_WARN` | Yellow |
| Error | `AURA_CORE_ERROR` | `AURA_ERROR` | Red |
| Fatal | `AURA_CORE_FATAL` | `AURA_FATAL` | Dark Red |

Use `AURA_CORE_*` for engine-internal messages and `AURA_*` for your game logic.

**Usage:**

```cpp
AURA_CORE_INFO("Engine initialized");
AURA_INFO("Player {} spawned at ({}, {}, {})", name, pos.x, pos.y, pos.z);
AURA_WARN("Low health: {}", health);
AURA_ERROR("Failed to load texture: {}", path);
```

---

## Writing Custom Shaders

The engine's `basic` shader supports two rendering modes controlled by uniforms:

### Vertex Shader (`basic.vert`)

```glsl
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 AProjection;  // Set by the engine (perspective projection)
uniform mat4 AModel;       // Set per-entity (transform component model matrix)

out vec2 TexCoord;

void main() {
    TexCoord = aTexCoord;
    gl_Position = AProjection * AModel * vec4(aPos, 1.0);
}
```

### Fragment Shader (`basic.frag`)

```glsl
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D u_Texture;  // Texture slot 0
uniform int useTexture;       // 1 = use texture, 0 = use solid color
uniform vec3 albedoColor;     // Solid color fallback

void main() {
    vec3 pixelColor;
    if (useTexture == 1)
        pixelColor = vec3(texture(u_Texture, TexCoord));
    else
        pixelColor = albedoColor;
    FragColor = vec4(pixelColor, 1.0);
}
```

### Custom Shader Guide

To write your own shaders:

1. Create `.vert` and `.frag` files in `AuraEngine/src/shaders/`
2. Load them via `AShaderManager::Get().Load("name", vertPath, fragPath)`
3. Use the built-in vertex layout:
   - `layout(location = 0)` — position (`vec3`)
   - `layout(location = 1)` — normal (`vec3`)
   - `layout(location = 2)` — texture coordinate (`vec2`)

To add uniforms, use the shader pointer:

```cpp
auto* shader = Aura::AShaderManager::Get().Get("my_shader");
shader->bind();
shader->setUniform1f("time", currentTime);
shader->setUniform3f("lightDir", 0.5f, 1.0f, 0.3f);
shader->setUniformMat4fv("viewMatrix", glm::value_ptr(view));
```

---

## Tutorial: Building a Solar System

This walkthrough recreates the Solar System demo from `SandBox/Main.cpp`.

### Step 1: Create the Window

```cpp
#include <Aura.h>
#include <Scene/ASceneManager.h>
#include <Entity/Components/AMaterialComponent.h>

int main() {
    Aura::Window::AWindow window("Solar System", 1600, 900);
```

### Step 2: Create a Scene and Sun

```cpp
    Aura::Scene::AScene* scene = Aura::Scene::AScene::MakeScene();

    // Create the sun
    auto* sun = scene->AddObject<Aura::Objects::ASphereObject>("sun");
    auto* sunMat = Aura::Components::AMaterial::MakeMaterial();
    sunMat->setAlbedoMap("C:/textures/sun.jpg");
    sun->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*sunMat);

    auto* sunTransform = sun->getComponent<Aura::Components::ATransformComponent>();
    sunTransform->scale = glm::vec3(10.0f);
    sunTransform->position = glm::vec3(0.0f, 0.0f, -50.0f);
```

### Step 3: Create Planets with Orbit Data

```cpp
    struct PlanetDef {
        std::string name;
        std::string texture;
        float scale;
        float orbitRadius;
        float orbitSpeed;
    };

    std::vector<PlanetDef> planets = {
        {"earth",   "C:/textures/earth.jpg",   1.0f,  30.0f, 29.0f},
        {"mars",    "C:/textures/mars.jpg",     0.53f, 40.0f, 24.0f},
        {"jupiter", "C:/textures/jupiter.jpg",  11.2f, 60.0f, 13.0f},
    };

    std::vector<Orbit> orbits;

    for (auto& def : planets) {
        auto* planet = scene->AddObject<Aura::Objects::ASphereObject>(def.name);
        auto* mat = Aura::Components::AMaterial::MakeMaterial();
        mat->setAlbedoMap(def.texture);
        planet->addComponent<Aura::Components::AMaterialComponent>()->setMaterial(*mat);

        auto* transform = planet->getComponent<Aura::Components::ATransformComponent>();
        transform->scale = glm::vec3(def.scale);
        transform->position = glm::vec3(def.orbitRadius, 0.0f, -50.0f);

        orbits.push_back({ transform, sunTransform, def.orbitRadius, def.orbitSpeed, 0.0f });
    }
```

### Step 4: Write the Update Loop

```cpp
    auto update = [&](float deltaTime, Nyx::Window::Window* w) {
        for (auto& orbit : orbits) {
            orbit.angle += glm::radians(orbit.speed) * deltaTime;
            float x = orbit.parent->position.x + orbit.radius * cos(orbit.angle);
            float z = orbit.parent->position.z + orbit.radius * sin(orbit.angle);
            orbit.body->position = glm::vec3(x, 0.0f, z);
            orbit.body->rotation.y += glm::radians(30.0f) * deltaTime;
        }
    };
```

### Step 5: Run

```cpp
    Aura::Scene::ASceneManager::SetActiveScene(0);
    window.update(update);
    return 0;
}
```

---

## Roadmap

### Current Limitations

- **No camera system** — projection is fixed; no view matrix or camera class
- **No lighting** — normals are in vertex data but the fragment shader has no lighting calculations
- **No input handling API** — beyond raw GLFW callbacks
- **No physics system**
- **No audio system**
- **No serialization** — scenes cannot be saved/loaded
- **Editor** — ImGui headers vendored but not yet integrated

### Planned Features

- [ ] Camera system (FPS, orbit, free-look)
- [ ] Directional, point, and spot lights with Phong/PBR shading
- [ ] Input manager abstraction
- [ ] Physics integration (rigid body, collision)
- [ ] Audio subsystem
- [ ] Scene serialization (JSON)
- [ ] ImGui-based editor
- [ ] Additional mesh primitives (plane, cylinder, torus)
- [ ] Skeletal animation
- [ ] Particle system
- [ ] Cross-platform support (Linux, macOS)

---

## License

MIT License. Copyright (c) 2025 Srinath-N-Gudi.

See [LICENSE](LICENSE) for details.
