#version 400 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in mat4 a_Transform;

uniform mat4 u_ViewProjection;
//uniform mat4 u_Transform;
//attibute mat4 u_Transform;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ViewProjection * a_Transform * vec4(a_Position, 1.0f);
}