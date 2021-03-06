#pragma once

#include <scene/object/object.h>
#include <scene/object/components/camera.h>
#include <ui/window.h>
#include <util/shader.h>
#include <util/mesh.h>
#include <util/texture.h>
#include <util/fbo.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class Object;
class Window;
class Shader;
class Camera;
class Light;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    Scene(GLFWwindow* window) : window(window) {
        glfwSetWindowUserPointer(window, this);
        glfwSetScrollCallback(window, [](GLFWwindow* w, double x, double y) {
            Scene* s = (Scene*)glfwGetWindowUserPointer(w);
            s->scrollX += x;
            s->scrollY += y;
            });
        sunShadowBuffer = std::shared_ptr<FBO>(new FBO(2048, 2048, GL_DEPTH_ATTACHMENT));
    }
    // WARNING: THIS MUST BE CALLED AFTER CONSTRUCTOR!
    //          (relies on shared_from_this())
    void loadAssets(const char* path = "./res");

    void update();
    void render();
    void renderUI();

    static const int MAX_LIGHTS = 16;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Light> dirLight;
    std::shared_ptr<FBO> sunShadowBuffer;
    glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);

    std::shared_ptr<Camera> activeCamera;
    std::shared_ptr<Object> inspectedObject;

    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Window>> windowUIs;

    std::vector<std::shared_ptr<Shader>> shaders;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Object>> blueprints;

    void save(std::string filename = "my.scene");
    void load(std::string filename = "my.scene");

    GLFWwindow* window;

    bool vsync = true;
    glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float ambientIntensity = 0.0f;
    double dTime = 0.0;

    // scroll values because glfw handles scroll with callbacks
    float scrollX = 0.0f;
    float scrollY = 0.0f;
};