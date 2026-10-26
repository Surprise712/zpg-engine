#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 objectColor; // Přijímáme barvu planety z vertex shaderu

out vec4 out_Color;

uniform vec3 viewPosition; // Nutné poslat z C++ (pozice kamery)

void main(void){
    // Světlo uprostřed (Slunce)
    vec3 lightPosition = vec3(0.0, 0.0, 0.0); 
    
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition.xyz);
    vec3 V = normalize(viewPosition - worldPosition.xyz);
    
    // Výpočet half-vektoru pro Blinn-Phong
    vec3 H = normalize(L + V);
    
    // Ambientní složka
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    
    // Difúzní složka s využitím barvy modelu (objectColor)
    float diff = max(dot(N, L), 0.0);
    vec4 diffuse = diff * vec4(objectColor, 1.0);
    
    // Specular s využitím half-vektoru
    float shininess = 32.0; 
    float spec = pow(max(dot(N, H), 0.0), shininess);
    vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0); 
    
    // Výsledná barva = (ambient * barva planety) + diffuse + specular
    out_Color = (ambient * vec4(objectColor, 1.0)) + diffuse + specular;
}