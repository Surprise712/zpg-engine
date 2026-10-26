#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;
in vec3 objectColor; 

out vec4 out_Color;

uniform vec3 viewPosition;

void main(void){
    
    vec3 lightPosition = vec3(0.0, 0.0, 0.0); 
    
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition.xyz);
    vec3 V = normalize(viewPosition - worldPosition.xyz);
    vec3 R = reflect(-L, N);
    
    // Ambientní složka
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    
    // Difúzní složka (využívá reálnou barvu planety)
    float diff = max(dot(N, L), 0.0);
    vec4 diffuse = diff * vec4(objectColor, 1.0);
    
    // Zrcadlová složka (odlesk moří atd.)
    float shininess = 32.0; 
    float spec = pow(max(dot(V, R), 0.0), shininess);
    vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0); 
    
    // Výsledná barva = ambient * barva + diffuse + specular
    out_Color = (ambient * vec4(objectColor, 1.0)) + diffuse + specular;
}