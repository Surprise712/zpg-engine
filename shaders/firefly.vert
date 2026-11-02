#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition; // Tady přijde vypočítaná pozice z C++

void main()
{
    // Aplikujeme měřítko a fixní transformace (Scale)
    vec4 worldPos = modelMatrix * vec4(position, 1.0);
    
    // TRIK: Světlušku posuneme ve světě jednoduše přičtením pozice
    worldPos.xyz += lightPosition;
    
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}