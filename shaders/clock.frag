
#version 330 core

in vec3 vertexColor;

out vec4 FragColor;

void main()
{
    vec3 color = vertexColor;

    if (color.r > 0.5 && color.g > 0.5 && color.b < 0.4)
        color = vec3(5.0, 195.0, 222.0) / 255.0;

    FragColor = vec4(color, 1.0);
}
