#version 330 core

in vec3 vertexNormal;
in vec3 baseColor;

out vec4 FragColor;

void main()
{
    vec3 color = baseColor;

    if (baseColor.g > 0.5)
    {
        float shade = 0.3 + 0.7 * abs(normalize(vertexNormal).y);
        color = shade * baseColor;
    }

    FragColor = vec4(color, 1.0);
}