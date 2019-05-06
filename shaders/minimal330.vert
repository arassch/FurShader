#version 330 core


layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in float in_Length;

out vec3 ex_Color;
out float ex_Length;

uniform mat4 transform;


void main()
{
    // gl_Position = vec4(in_Position, 1.0);

    vec4 p = vec4(in_Position, 1.0);
    // p.y += in_Length;
    p.z = sin(in_Position.x);
    gl_Position = p;
    // gl_Position = p;
    // ex_Direction = in_Direction;
    ex_Color = in_Color;
    ex_Length = in_Length;
}