#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertexNormal;
out vec3 baseColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

    vertexNormal = normal;

    if (gl_VertexID > 36443)
        baseColor = vec3(0.3, 0.45, 0.2);   // listi
    else
        baseColor = vec3(0.45, 0.25, 0.10); // kmen + vetve
}