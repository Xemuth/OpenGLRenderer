SHADER(330 core,

	layout (location  = 0) in vec3 positions;
	layout (location  = 1) in vec3 normals;
	layout (location  = 2) in vec2 textCoords;
	
	out vec3 pos;
	out vec2 textureCoords;
	out vec3 norm;
	
	uniform mat4 view;
	uniform mat4 proj;
	uniform mat4 model;
	
	void main(void){
		gl_Position = proj * view * model * vec4(positions,1.0);
		pos = positions;
		textureCoords = textCoords;
		norm = normals;
	}
)