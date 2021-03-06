#pragma once

#include <util/vao.h>

#include <glad/glad.h>

#include <memory>

class VAO;

class VBO
{
public:
    GLuint id;
    size_t size;
    VBO(std::shared_ptr<VAO> vao, GLfloat *verts, GLsizeiptr sz, GLenum use = GL_STATIC_DRAW);
    ~VBO();

    void bind();
    void unbind();
};