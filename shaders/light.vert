#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 worldPosition;
out vec3 worldNormal;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main()
{
    worldPosition = modelMatrix * vec4(position, 1.0);
    
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    
    worldNormal = normalize(normalMatrix * normal);
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}