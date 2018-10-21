#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection1;
uniform mat4 view1;

void main()
{
    TexCoords = aPos;
    gl_Position =  projection1*view1*vec4(aPos, 1.0);
}  