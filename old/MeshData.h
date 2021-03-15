#ifndef _pluginUFE_OpenGLRenderer_MeshData_h_
#define _pluginUFE_OpenGLRenderer_MeshData_h_
#include <Core/Core.h>
#include <plugin/glew/glew.h>

namespace Upp{

struct Mesh : public Moveable<Mesh> {
	public:
		Mesh(int indice, int vertexCount);
		Mesh(Mesh&& mesh);
		Mesh(const Mesh& mesh);
		~Mesh();
		
		Mesh& operator=(Mesh&& mesh);
		Mesh& operator=(const Mesh& mesh);
		
		int GetIndice()const;
		int GetVertexCount()const;
	private:
		int indice;
		int vertexCount;
};


class MeshData : public Moveable<MeshData>{
	public:
		MeshData();
		MeshData(MeshData&& meshdata);
		MeshData(const MeshData& meshdata);
		~MeshData();
		
		MeshData& operator=(MeshData&& meshdata);
		MeshData& operator=(const MeshData& meshdata);
		
		MeshData& AddMesh(const Vector<GLfloat>& _datas, int verticeCount);
				 
		MeshData& Load();
		
		bool IsLoaded(){return loaded;}
		GLuint GetVBO(){return vbo;}
		
		Vector<Mesh>& GetMeshes(){return meshes;}
		
		String ToString()const;
		
	private:
		bool loaded = false;
		
		Vector<GLfloat> datas;
		Vector<Mesh> meshes;

		GLuint vbo;
};

}



#endif
