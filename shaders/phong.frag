#version 330 core

in vec4 worldPosition;
in vec3 worldNormal;

out vec4 out_Color;

uniform vec3 viewPosition; // Nutné poslat z C++ (pozice kamery)

void main(void){
    vec3 lightPosition = vec3(0.0, 0.0, 0.0); 
    
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition.xyz);
    vec3 V = normalize(viewPosition - worldPosition.xyz);
    vec3 R = reflect(-L, N);
    
    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    
    float diff = max(dot(N, L), 0.0);
    vec4 diffuse = diff * vec4(0.385, 0.647, 0.812, 1.0);
    
    // Specular (Zrcadlová složka)
    float shininess = 32.0; // Míra lesklosti
    float spec = pow(max(dot(V, R), 0.0), shininess);
    vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0); // Bílá barva odlesku
    
    out_Color = ambient + diffuse + specular;
}