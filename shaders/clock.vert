
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;

vec3 rotateZ(vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return vec3(
        c * p.x - s * p.y,
        s * p.x + c * p.y,
        p.z
    );
}

void main()
{
    vec3 p = position;

    bool secondHand = color.r > 0.7 && color.g < 0.2 && color.b < 0.2;
    bool minuteHand = color.g > 0.7 && color.r < 0.2 && color.b < 0.2;
    bool hourHand = color.b > 0.7 && color.r < 0.2 && color.g < 0.2;

    float secondAngle = -6.2831853 * time / 60.0;
    float minuteAngle = -6.2831853 * time / 3600.0;
    float hourAngle = -6.2831853 * time / 43200.0;

    if (secondHand)
    {
        p = rotateZ(p, secondAngle);
    }
    else if (minuteHand)
    {
        p = rotateZ(p, minuteAngle);
    }
    else if (hourHand)
    {
        p = rotateZ(p, hourAngle);
    }

    vertexColor = color;

    gl_Position = projectionMatrix
                * viewMatrix
                * modelMatrix
                * vec4(p, 1.0);
}