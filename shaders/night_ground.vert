#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inColor; // U země jsi do druhého parametru nedal normálu, ale barvu

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

    // Země je rovná, normála směřuje vždy přímo nahoru (osa Y)
    vertexNormal = vec3(0.0, 1.0, 0.0); 
    
    // Barvu převezmeme z tvého pole z C++
    baseColor = inColor; 
}