#pragma once

#include <scene/scene.h>
#include <scene/object/object.h>

#include <memory>
#include <string>

class Scene;
class Object;

class Script {
public:
    Script(std::string name, std::shared_ptr<Object> obj) : name(name), object(obj) {
        start();
    }
    virtual void start() {};
    virtual void update(std::shared_ptr<Scene> s) = 0;
    virtual void renderInspector() = 0;
protected:
    std::string name;
    std::shared_ptr<Object> object;
};