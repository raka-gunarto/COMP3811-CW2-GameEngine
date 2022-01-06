#pragma once

#include <scene/object/components/renderer/renderer.h>
#include <util/mesh.h>

#include <stdexcept>

class MeshRenderer : public Renderer {
public:
    MeshRenderer(std::shared_ptr<Object> obj, std::shared_ptr<Mesh> mesh = nullptr)
        : Renderer(obj), mesh(mesh)
    {
        name = "MeshRenderer";
    }
    MeshRenderer(const MeshRenderer& other, std::shared_ptr<Object> newObj)
        : MeshRenderer(newObj, other.mesh) {}

    void render(std::shared_ptr<Scene> s) override;
    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "MeshRenderer";
        if (mesh)
            emitter << YAML::Key << "mesh" << YAML::Value << mesh->name;
        
        emitter << YAML::EndMap;
        return emitter;
    }
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) {
        return std::shared_ptr<Component>(new MeshRenderer(*this, newObj));
    }

    std::shared_ptr<Mesh> mesh;
};