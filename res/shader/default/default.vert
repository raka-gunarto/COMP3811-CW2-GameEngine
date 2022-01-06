#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoords;

out vec3 cPos;
out vec3 normal;

uniform mat4 cameraMat;
uniform mat4 model;
uniform mat3 normalMat;

void main()
{
    cPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = cameraMat * vec4(cPos, 1.0f);
    normal = normalize(normalMat * aNormal);
    texCoords = aTexCoords;
}