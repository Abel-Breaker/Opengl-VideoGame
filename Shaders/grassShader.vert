#version 430 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout(std430, binding = 0) buffer grassPositions {
    float positions[];
};

out vec2 TexCoord; //Coordenadas de las texturas

uniform mat4 view;
uniform mat4 projection;


void main()
{

    mat4 myTransform = mat4(
        cos(45*(gl_InstanceID % 3)), 0.0, -sin(45*(gl_InstanceID % 3)), 0.0,
        0.0, 1.0, 0.0, 0.0,
        sin(45*(gl_InstanceID % 3)), 0.0, cos(45*(gl_InstanceID % 3)), 0.0,
        positions[(gl_InstanceID / 3) * 3], positions[(gl_InstanceID / 3) * 3 + 1], positions[(gl_InstanceID / 3) * 3 + 2], 1.0
    );


   gl_Position = projection * view * myTransform * vec4(aPos, 1.0);

   TexCoord = aTexCoord;
}