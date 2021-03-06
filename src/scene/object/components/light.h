#pragma once

#include <scene/object/components/component.h>

#include <glm/glm.hpp>

#include <memory>

class Transform;

class Light : public Component
{
public:
    Light(std::shared_ptr<Object> obj, glm::vec3 color = glm::vec3(1.0f), float linearAttenuation = 0, float quadAttenuation = 0);
    Light(const Light& other, std::shared_ptr<Object> newObj) : Light(newObj, other.color, other.linearAttenuation, other.quadAttenuation) {}
    std::shared_ptr<Component> clone(std::shared_ptr<Object> newObj) override {
        return std::shared_ptr<Component>(new Light(*this, newObj));
    }

    enum Type {
        POINT,
        DIRECTIONAL
    };

    void renderInspector() override;
    YAML::Emitter& serialise(YAML::Emitter& emitter) override
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << "Light";
        emitter << YAML::Key << "type" << YAML::Value << type;
        emitter << YAML::Key << "linearAttenuation" << YAML::Value << linearAttenuation;
        emitter << YAML::Key << "quadAttenuation" << YAML::Value << quadAttenuation;
        emitter << YAML::Key << "color" << YAML::Value << YAML::Flow << YAML::BeginSeq
            << color.r << color.g << color.b
            << YAML::EndSeq;
        emitter << YAML::EndMap;
        return emitter;
    }
    void _deserialise(const YAML::Node& componentNode) override
    {
        // do the reverse of serialise duh
        type = (Type) componentNode["type"].as<int>();
        linearAttenuation = componentNode["linearAttenuation"].as<float>();
        quadAttenuation = componentNode["quadAttenuation"].as<float>();
        color = glm::vec3(
            componentNode["color"][0].as<float>(),
            componentNode["color"][1].as<float>(),
            componentNode["color"][2].as<float>()
        );
    }

    float linearAttenuation = 0;
    float quadAttenuation = 0;
    glm::vec3 color = glm::vec3(1.0f);
    Type type = POINT;

    std::shared_ptr<Transform> transform;
};