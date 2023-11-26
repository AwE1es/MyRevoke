#type vertex
#version 330 core
			
	layout(location = 0) in vec3 a_Pos;
	layout(location = 1) in vec2 a_TexCoord;

	uniform mat4 u_PVmatrix;	
	uniform mat4 u_TransformMatrix;

	out vec2 v_TexCoord;


	void main()
	{
		v_TexCoord = a_TexCoord;
		gl_Position = u_PVmatrix * u_TransformMatrix * vec4(a_Pos, 1.0);
	}


#type fragment
#version 330 core

	layout(location = 0) out vec4 o_Color;
	
	in vec2 v_TexCoord;

	uniform vec4 u_Color1;

	uniform sampler2D u_Texture;

	void main()
	{
		o_Color = texture(u_Texture, v_TexCoord);
	}
