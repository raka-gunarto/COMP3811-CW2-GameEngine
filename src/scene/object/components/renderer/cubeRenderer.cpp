#include "cubeRenderer.h"

#include <scene/scene.h>
#include <scene/object/components/transform.h>
#include <scene/object/components/light.h>

#include <imgui.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <memory>

bool CubeRenderer::initialised = false;
std::shared_ptr<VBO> CubeRenderer::cubeVBO;
std::shared_ptr<VAO> CubeRenderer::cubeVAO;
std::shared_ptr<EBO> CubeRenderer::cubeEBO;

CubeRenderer::CubeRenderer(std::shared_ptr<Object> obj) : Renderer(obj)
{
    // ensure vertex data is initialised
    CubeRenderer::initVertexData();

    name = "CubeRenderer";

    // default mode flat color, white
    mode = CubeRenderer::Mode::MATERIAL;
    diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
    specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
}

void CubeRenderer::initVertexData() {
    if (initialised) return;
    // initialise buffers
    // the vertices can be constant, it can be transformed using
    // the transform component. Vertices are for a cube

    // each face is 4 vertices, drawn with two triangle primitives
    // defined with the index buffer
    GLfloat verts[] = {
        // vertices           // normals           // texcoords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0
        0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 0.0f,  // 1
        0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f,  // 2
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,  // 3

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,  // 4
        0.5f, -0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  1.0f, 0.0f,  // 5
        0.5f,  0.5f,  0.5f,   0.0f,  0.0f, 1.0f,  1.0f, 1.0f,  // 6
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,  // 7

        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,  // 8
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 1.0f,  // 12
        0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  // 16
        0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,  // 20
        0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    };
    GLuint elements[] = {
        0, 1, 2, 2, 3, 0, // first face
        4, 5, 6, 6, 7, 4, // second face
        8, 9, 10, 10, 11, 8, // third face
        12, 13, 14, 14, 15, 12, // fourth face
        16, 17, 18, 18, 19, 16, // fifth face
        20, 21, 22, 22, 23, 20, // sixth face
    };
    cubeVAO = std::shared_ptr<VAO>(new VAO());
    cubeVBO = std::shared_ptr<VBO>(new VBO(cubeVAO, verts, sizeof(verts)));
    cubeEBO = std::shared_ptr<EBO>(new EBO(cubeVAO, elements, sizeof(elements)));

    // set buffer attributes
    // vertex positions
    cubeVAO->link(cubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0); // vertex coords
    // normals 
    cubeVAO->link(cubeVBO, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float))); // vertex coords
    // texcoords 
    cubeVAO->link(cubeVBO, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float))); // vertex coords

    initialised = true;
}

void CubeRenderer::render(std::shared_ptr<Scene> s, std::shared_ptr<Shader> shaderOverride)
{
    // set default shader
    if (!shader)
        shader = object->getScene()->shaders[0];

    // find object transform
    std::shared_ptr<Transform> t = object->getComponent<Transform>();

    // cannot find transform, don't render anything
    if (t == nullptr)
    {
        std::cout << "WARN::" << object->getName() << "::cubeRenderer::cannot find transform" << std::endl;
        return;
    }

    // activate the renderer's shader
    if (shaderOverride)
        shaderOverride->activate();
    else
        shader->activate();

    // model mat and normal mat
    glm::mat4 modelMat = t->modelMatrix();
    glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(modelMat)));
    glUniformMatrix4fv(glGetUniformLocation(shader->id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix3fv(glGetUniformLocation(shader->id, "normalMat"), 1, GL_FALSE, glm::value_ptr(normalMat));

    // load color / texture
    switch (mode)
    {
    case CubeRenderer::Mode::MATERIAL:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"),
            diffuseColor.r,
            diffuseColor.g,
            diffuseColor.b
        );
        glUniform3f(glGetUniformLocation(shader->id, "specularColor"),
            specularColor.r,
            specularColor.g,
            specularColor.b
        );
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUniform1f(glGetUniformLocation(shader->id, "shininess"), shininess);
        break;
    case CubeRenderer::Mode::TEX_MAP:
        glUniform3f(glGetUniformLocation(shader->id, "diffuseColor"), 0, 0, 0);
        glUniform3f(glGetUniformLocation(shader->id, "specularColor"), 0, 0, 0);
        glUniform1f(glGetUniformLocation(shader->id, "shininess"), shininess);
        if (diffuseTex)
            diffuseTex->bind(GL_TEXTURE0);
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        if (specularTex)
            specularTex->bind(GL_TEXTURE1);
        else {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glUniform1i(glGetUniformLocation(shader->id, "diffuseTex"), 0);
        glUniform1i(glGetUniformLocation(shader->id, "specularTex"), 1);
        break;

    }
    cubeVAO->bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
    cubeVAO->unbind();
}

void CubeRenderer::renderInspector()
{
    ImGui::Text("Cube Renderer");
    ImGui::Text("Shader: "); ImGui::SameLine();
    ImGui::Button(shader->name.c_str());
    if (ImGui::BeginDragDropTarget())
        if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("SHADER"))
            shader = (*((Shader**)p->Data))->shared_from_this();
    ImGui::Text("Mode"); ImGui::SameLine();
    if (ImGui::RadioButton("Material", mode == MATERIAL)) { mode = MATERIAL; } ImGui::SameLine();
    if (ImGui::RadioButton("Texture Map", mode == TEX_MAP)) { mode = TEX_MAP; }
    switch (mode) {
    case CubeRenderer::Mode::MATERIAL:
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(diffuseColor));
        ImGui::ColorEdit3("Specular", glm::value_ptr(specularColor));
        ImGui::DragFloat("Shininess", &shininess, 0.1f, 0.0f);
        break;
    case CubeRenderer::Mode::TEX_MAP:
        ImGui::Text("Diffuse Map");
        if (diffuseTex)
            ImGui::Image((void*)(intptr_t)diffuseTex->ID, ImVec2(50.0f, 50.0f), ImVec2(1, 1), ImVec2(0, 0));
        else
            ImGui::Button("Drop Texture Here", ImVec2(50.0f, 50.0f));
        if (ImGui::BeginDragDropTarget())
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("TEXTURE"))
                diffuseTex = (*(Texture**)p->Data)->shared_from_this();

        ImGui::NewLine();

        ImGui::Text("Specular Map");
        if (specularTex)
            ImGui::Image((void*)(intptr_t)specularTex->ID, ImVec2(50.0f, 50.0f), ImVec2(1, 1), ImVec2(0, 0));
        else
            ImGui::Button("Drop Texture Here", ImVec2(50.0f, 50.0f));
        if (ImGui::BeginDragDropTarget())
            if (const ImGuiPayload* p = ImGui::AcceptDragDropPayload("TEXTURE"))
                specularTex = (*(Texture**)p->Data)->shared_from_this();
        ImGui::DragFloat("Shininess", &shininess, 0.1f, 0.0f);
        break;
    }
    ImGui::Separator();
}