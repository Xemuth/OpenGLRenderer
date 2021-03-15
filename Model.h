#ifndef _pluginUFE_OpenGLRenderer_Model_h_
#define _pluginUFE_OpenGLRenderer_Model_h_
#include <Core/Core.h>
#include <plugin/glew/glew.h>

namespace Upp{

struct Mesh : public Moveable<Mesh> {
	public:
		Mesh(int indice, int vertexCount);
		Mesh();
		
		int GetIndice()const;
		int GetVertexCount()const;
	private:
		int indice;
		int vertexCount;
};

class Model : public Moveable<Model>{
	public:
		Model();
		~Model();
		
		Model& AddVertices(const Vector<GLfloat>& _datas, int verticeCount);
		Model& Load(GLbitfield flags = GL_DYNAMIC_STORAGE_BIT);
		void Clear();
		
		bool IsLoaded()const;
		GLuint GetVBO();
		
		Vector<Mesh>& GetMeshes();
		const Vector<Mesh>& GetMeshes()const;
		String ToString()const;
		
	private:
		bool loaded = false;
		Vector<GLfloat> datas;
		Vector<Mesh> meshes;
		GLuint vbo;
};

}

#endif
