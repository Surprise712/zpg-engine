#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertexNormal;
out vec3 baseColor;
out vec4 worldPosition; // PŘIDÁNO: Výstup pro fragment shader

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    // 1. Výpočet pozice ve světě (tu pošleme do fragment shaderu)
    worldPosition = modelMatrix * vec4(position, 1.0);

    // 2. Finální pozice na obrazovce
    gl_Position = projectionMatrix * viewMatrix * worldPosition;

    // 3. Správný převod normály do světového prostoru (využití bodu 1 z tvého zadání!)
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vertexNormal = normalMatrix * normal;

    // 4. Obarvení stromu (stejné jako jsi měl)
    if (gl_VertexID > 36443)
        baseColor = vec3(0.3, 0.45, 0.2);   // listi
    else
        baseColor = vec3(0.45, 0.25, 0.10); // kmen + vetve
}