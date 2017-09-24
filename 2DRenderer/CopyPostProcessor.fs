#version 440

uniform sampler2D inputTexture;

in vec2 uv;

out vec4 fragColor;

void main()
{
	fragColor = texture(inputTexture, uv);
}