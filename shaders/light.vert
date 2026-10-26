#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal; // Změněno z color na normal, 3D model potřebuje normály

out vec4 worldPosition;
out vec3 worldNormal;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

void main()
{
    // Výpočet pozice ve světě pro fragment shader
    worldPosition = modelMatrix * vec4(position, 1.0);
    
    // Transformace normály (nutné pro výpočet odrazu světla)
    worldNormal = mat3(modelMatrix) * normal; 
    
    // Finální pozice na obrazovce
    gl_Position = projectionMatrix * viewMatrix * worldPosition;
}