#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Camera.h"
#include "World.h"
#include "Block.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const int RENDER_DISTANCE = 6;
const int WORLD_SEED = 42;

Camera camera(glm::vec3(0, 40, 0));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursorLocked = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

World* world = NULL;

BlockType selectedBlock = BlockType::Grass;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aColor;

uniform mat4 uProjection;
uniform mat4 uView;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoord;
out vec3 vColor;

void main() {
    vFragPos = aPos;
    vNormal = aNormal;
    vTexCoord = aTexCoord;
    vColor = aColor;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;
in vec3 vColor;

uniform vec3 uLightDir;
uniform vec3 uViewPos;

void main() {
    vec3 color = vColor;
    vec3 ambient = 0.4 * color;
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color;
    float dist = length(uViewPos - vFragPos);
    float fog = clamp((dist - 80.0) / 60.0, 0.0, 1.0);
    vec3 fogColor = vec3(0.6, 0.75, 0.95);
    vec3 result = ambient + diffuse;
    result = mix(result, fogColor, fog);
    FragColor = vec4(result, 1.0);
}
)";

const char* highlightVertexSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 uProjection;
uniform mat4 uView;
uniform vec3 uBlockPos;
void main() {
    vec3 pos = aPos * 1.005 + uBlockPos;
    gl_Position = uProjection * uView * vec4(pos, 1.0);
}
)";

const char* highlightFragmentSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 0.3);
}
)";

const char* crosshairVertSource = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* crosshairFragSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 0.8);
}
)";

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint createProgram(const char* vertSrc, const char* fragSrc) {
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Program linking failed:\n" << infoLog << std::endl;
    }
    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (!cursorLocked) return;
    float xpos = (float)xposIn;
    float ypos = (float)yposIn;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action != GLFW_PRESS) return;
    glm::vec3 hitPos, prevPos;
    if (world->Raycast(camera.Position, camera.GetForward(), 8.0f, hitPos, prevPos)) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            world->SetBlock((int)hitPos.x, (int)hitPos.y, (int)hitPos.z, BlockType::Air);
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (world->GetBlock((int)prevPos.x, (int)prevPos.y, (int)prevPos.z) == BlockType::Air) {
                world->SetBlock((int)prevPos.x, (int)prevPos.y, (int)prevPos.z, selectedBlock);
            }
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cursorLocked = !cursorLocked;
        if (cursorLocked)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_1: selectedBlock = BlockType::Grass; break;
            case GLFW_KEY_2: selectedBlock = BlockType::Dirt; break;
            case GLFW_KEY_3: selectedBlock = BlockType::Stone; break;
            case GLFW_KEY_4: selectedBlock = BlockType::Wood; break;
            case GLFW_KEY_5: selectedBlock = BlockType::Planks; break;
            case GLFW_KEY_6: selectedBlock = BlockType::Cobblestone; break;
            case GLFW_KEY_7: selectedBlock = BlockType::Glass; break;
            case GLFW_KEY_8: selectedBlock = BlockType::Brick; break;
            case GLFW_KEY_9: selectedBlock = BlockType::Sand; break;
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.FOV -= (float)yoffset;
    if (camera.FOV < 30.0f) camera.FOV = 30.0f;
    if (camera.FOV > 120.0f) camera.FOV = 120.0f;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aura Minecraft", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE);

    GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    GLuint highlightProgram = createProgram(highlightVertexSource, highlightFragmentSource);
    GLuint crosshairProgram = createProgram(crosshairVertSource, crosshairFragSource);

    float cubeVerts[] = {
        0,0,0, 1,0,0, 1,1,0, 0,1,0,
        0,0,1, 1,0,1, 1,1,1, 0,1,1,
    };
    unsigned int cubeIndices[] = {
        0,1,2, 2,3,0,
        4,6,5, 6,4,7,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        0,1,5, 5,4,0,
        3,7,6, 6,2,3
    };

    GLuint cubeVAO, cubeVBO, cubeIBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeIBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    float crosshairVerts[] = {
        -0.02f, 0.0f,
         0.02f, 0.0f,
         0.0f, -0.02f,
         0.0f,  0.02f,
    };
    unsigned int crosshairIndices[] = { 0, 1, 2, 3 };

    GLuint crosshairVAO, crosshairVBO, crosshairIBO;
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glGenBuffers(1, &crosshairIBO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVerts), crosshairVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crosshairIndices), crosshairIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    world = new World(RENDER_DISTANCE, WORLD_SEED);
    camera.Position = glm::vec3(0, 50, 0);

    std::cout << "\n=== CONTROLS ===" << std::endl;
    std::cout << "WASD        - Move" << std::endl;
    std::cout << "SPACE       - Fly up" << std::endl;
    std::cout << "CTRL        - Fly down" << std::endl;
    std::cout << "SHIFT       - Sprint" << std::endl;
    std::cout << "Mouse       - Look around" << std::endl;
    std::cout << "Left Click  - Break block" << std::endl;
    std::cout << "Right Click - Place block" << std::endl;
    std::cout << "1-9         - Select block type" << std::endl;
    std::cout << "Scroll      - Zoom in/out" << std::endl;
    std::cout << "ESC         - Toggle cursor lock" << std::endl;
    std::cout << "================\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        camera.ProcessKeyboard(window, deltaTime);

        world->Update(camera.Position);

        glClearColor(0.6f, 0.75f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (float)width / (float)height;

        glm::mat4 projection = camera.GetProjectionMatrix(aspect);
        glm::mat4 view = camera.GetViewMatrix();

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uView"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(shaderProgram, "uLightDir"), 0.5f, 1.0f, 0.3f);
        glUniform3f(glGetUniformLocation(shaderProgram, "uViewPos"), camera.Position.x, camera.Position.y, camera.Position.z);

        world->Render();

        glm::vec3 hitPos, prevPos;
        if (world->Raycast(camera.Position, camera.GetForward(), 8.0f, hitPos, prevPos)) {
            glUseProgram(highlightProgram);
            glUniformMatrix4fv(glGetUniformLocation(highlightProgram, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(highlightProgram, "uView"), 1, GL_FALSE, glm::value_ptr(view));
            glUniform3f(glGetUniformLocation(highlightProgram, "uBlockPos"), hitPos.x, hitPos.y, hitPos.z);
            glDisable(GL_CULL_FACE);
            glDepthFunc(GL_LEQUAL);
            glBindVertexArray(cubeVAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
            glEnable(GL_CULL_FACE);
        }

        glUseProgram(crosshairProgram);
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(crosshairVAO);
        glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        static float lastPrint = 0;
        if (currentFrame - lastPrint > 2.0f) {
            std::cout << "\rPos: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ") | Block: " << BlockRegistry::Get(selectedBlock).name << "    " << std::flush;
            lastPrint = currentFrame;
        }

        glfwSwapBuffers(window);
    }

    delete world;
    glfwTerminate();
    return 0;
}
