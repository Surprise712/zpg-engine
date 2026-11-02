#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertexNormal;
out vec3 baseColor;
out vec4 worldPosition; 

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    worldPosition = modelMatrix * vec4(position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPosition;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vertexNormal = normalMatrix * normal;

    // Křoví bude celé tmavě zelené
    baseColor = vec3(0.15, 0.35, 0.1); 
}