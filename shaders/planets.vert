#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color; // Přidána barva z indexu 2

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec3 objectColor; // Posíláme barvu do fragment shaderu

void main(void){
    worldPosition = modelMatrix * vec4(position, 1.0);
    
    // Transformace normály
    worldNormal = mat3(modelMatrix) * normal; 
    
    // Předání barvy z datového pole dál
    objectColor = color; 
    
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}