#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_PVmatrix;	
uniform mat4 u_TransformMatrix;

out vec3 v_Position;


void main()
{
	gl_Position = u_PVmatrix * u_TransformMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
in vec3 v_Position;

uniform vec4 u_Color1;

void main()
{
	o_Color = u_Color1;
}