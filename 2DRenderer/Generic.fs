#version 440

const int Color_Mode = 0;
const int Texture_Mode = 1;
const int Animated_Mode = 2;

uniform int mode;

uniform vec3 color;
uniform sampler2D sampler;
uniform mat3 textureMatrix;

in vec2 uv;

layout(location = 0) out vec4 color0;
layout(location = 1) out vec4 color1;

void main() {
	if(mode == Color_Mode)
	{
		color0 = vec4(color, 1);
	}
	else if(mode == Texture_Mode)
	{
		color0 = texture(sampler, uv);
	}
	else
	{
		color0 = texture(sampler, (textureMatrix * vec3(uv, 1)).xy);
	}
	color1 = vec4(1, 0, 0, 1);
}