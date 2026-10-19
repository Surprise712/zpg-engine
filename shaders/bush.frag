
#version 330 core

in vec3 vertexNormal;

out vec4 FragColor;

void main()
{
    vec3 color = vec3(0.05, 0.55, 0.08);
    float shade = 0.3 + 0.7 * abs(normalize(vertexNormal).y);

    FragColor = vec4(shade * color, 1.0);
}
