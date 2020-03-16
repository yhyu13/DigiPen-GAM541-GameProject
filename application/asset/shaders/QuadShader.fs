#version 400 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
	if(texColor.a < 0.01) discard;
	color = texColor;
}