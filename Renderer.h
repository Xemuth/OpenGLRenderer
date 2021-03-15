#ifndef _pluginUFE_OpenGLRenderer_Renderer_h_
#define _pluginUFE_OpenGLRenderer_Renderer_h_

#include "Shader.h"
#include "Model.h"

namespace Upp{
	
class CGLCamera;

struct VertexArrayAttribFormat : public Moveable<VertexArrayAttribFormat>{
	VertexArrayAttribFormat(GLuint _attribIndex, GLint _size, GLenum _type, GLboolean _normalized, GLuint _relativeOffset);
	
	VertexArrayAttribFormat& SetAttribIndex(GLuint _attribIndex);
	VertexArrayAttribFormat& SetSize(GLint _size);
	VertexArrayAttribFormat& SetType(GLenum _type);
	VertexArrayAttribFormat& SetNormalized(GLboolean _normalized);
	VertexArrayAttribFormat& SetRelativeOffset(GLuint _relativeOffset);
	
	GLuint attribIndex;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLuint relativeOffset;
};

typedef VertexArrayAttribFormat VAAF;

class Renderer : public Moveable<Renderer>{
	public:
		Renderer();
		~Renderer();
		
		void Render(Model& model, ShaderProgram& shader, CGLCamera& camera);
		void LoadVAO();
		void Clear();
		Vector<VertexArrayAttribFormat>& GetVertexArrayAttribFormat();
		const Vector<VertexArrayAttribFormat>& GetVertexArrayAttribFormat()const;
		
		GLuint GetVAO();
		const GLuint GetVAO()const;
		Renderer& SetVAO(GLuint _vao);

		String ToString()const;
		
		Event<Renderer&> customVAOLoader;
		Event<Renderer&, ShaderProgram&, Model&, CGLCamera&> whenRendered;
		Event<Renderer&, ShaderProgram&, CGLCamera&> beforeRendering;
		Event<Renderer&, ShaderProgram&, CGLCamera&> afterRendering;
	private:
		Vector<VertexArrayAttribFormat> arrayAttribFormat;
		GLuint vao = 0;
};

}

#endif
