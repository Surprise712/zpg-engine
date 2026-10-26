#version 330 core

in vec3 objectColor;

out vec4 out_Color;

void main(void){
    
    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    vec4 ambient = vec4(1.0, 1.0, 1.0, 1.0);
    
    out_Color = ambient * objectColor;
}