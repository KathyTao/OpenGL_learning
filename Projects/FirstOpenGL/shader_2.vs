#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 shiftHorizontal;

out vec3 ourColor;

void main()
{
    vec3 newPos = aPos + shiftHorizontal;
    gl_Position = vec4(newPos, 1.0);
    ourColor = aColor;
}