#version 430 core

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

out vec4 FragColor;

void main()

{
	FragColor = texture(texture1, TexCoord);
	if (FragColor.a < 0.05 ){
		discard;
	}

}