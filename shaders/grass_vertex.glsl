#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform float uTime;

void main()
{
    //float sway = 0.5f * sin(uTime) + 2.5f;

    vec3 newPosition = aPos;
    //newPosition.z += smoothstep(0.0f, sway, aPos.y);

    gl_Position = projection * view * model * vec4(newPosition, 1.0f);
}
