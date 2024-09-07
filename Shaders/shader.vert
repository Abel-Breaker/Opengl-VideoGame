#version 430 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord; //Coordenadas de las texturas

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;


void main()
{
   gl_Position = projection * view * transform * vec4(aPos, 1.0);
in vec3 ourColor;
in vec3 ourColor;
   ourColor = vec3(1.0, 1.0, 1.0);

   TexCoord = aTexCoord;
}