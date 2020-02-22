#version 400 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform float u_Offset;

void main()
{
    color = vec4(vec3(1.0 - texture(u_Texture, v_TexCoord)), 1.0);
}