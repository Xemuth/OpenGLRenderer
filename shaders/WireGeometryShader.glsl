SHADER(330 core,

	layout (triangles) in;
	layout (line_strip, max_vertices = 4) out;
	
	in vec3 pos[3];
	in vec2 textureCoords[3];
	in vec3 norm[3];
	
	void main()
	{
		int i;
		for(i = 0; i < gl_in.length(); i++){
			gl_Position =  gl_in[i].gl_Position;
			EmitVertex();
		}
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
)