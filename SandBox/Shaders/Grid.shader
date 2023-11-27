#type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_PVmatrix;	
uniform mat4 u_TransformMatrix;

out vec3 v_Position;


void main()
{
	v_Position = a_Pos;
	gl_Position = u_PVmatrix * u_TransformMatrix * vec4(a_Pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;
in vec3 v_Position;

uniform vec4 u_Color;

void main()
{
	o_Color = u_Color;
}