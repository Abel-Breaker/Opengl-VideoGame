#version 430 core

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

out vec4 FragColor;

void main()

{
 FragColor = vec4(0.3,0.2,0.23, 1.0);
 //FragColor = texture(texture1, TexCoord);
}