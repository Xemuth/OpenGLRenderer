#ifndef _pluginUFE_OpenGLRenderer_Renderer_h_
#define _pluginUFE_OpenGLRenderer_Renderer_h_

#include "ShaderProgram.h"
#include "MeshData.h"

namespace Upp{
	
class CGLCamera;

class Renderer : public Moveable<Renderer> {
	public:
		Renderer(const String& shaderName);
		Renderer(Renderer&& renderer);
		Renderer(const Renderer& renderer);
		~Renderer();
		
		Renderer& operator=(Renderer&& renderer);
		Renderer& operator=(const Renderer& renderer);
		
		void Render(MeshData& meshData);
		void GenerateDefaultVAO();
		
		ShaderProgram& GetShaderProgram(){return shader;}
		
		GLuint GetVAO(){return vao;}
		Renderer& SetVAO(GLuint _vao){vao = _vao; return *this;}
		
		Renderer& SetUniformProjectionMatrixName(const String& name){uniformProjectionMatrixName = name; return *this;};
		Renderer& SetUniformViewMatrixName(const String& name){uniformViewMatrixName = name; return *this;}
		Renderer& SetBeforeRendering(Event<Renderer&, CGLCamera&> event){beforeRendering = event; return *this;}
		
		String uniformProjectionMatrixName = "";
		String uniformViewMatrixName = "";
		Event<Renderer&, MeshData&> whenRendered;
		Event<Renderer&, CGLCamera&> beforeRendering;
		
		String ToString()const;
	private:
		FreeShader shader;
		GLuint vao = 0;
};

}

#endif
