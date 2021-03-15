#include "../Model.h"

namespace Upp{

//Mesh
Mesh::Mesh(int _indice, int _vertexCount){
	indice = _indice;
	vertexCount = _vertexCount;
}

int Mesh::GetIndice()const{
	return indice;
}

int Mesh::GetVertexCount()const{
	return vertexCount;
}

//Model
Model::~Model(){
	Clear();
}
Model& Model::AddVertices(const Vector<GLfloat>& _datas, int verticeCount){
	meshes.Create(datas.GetCount(),verticeCount);
	datas.Append(_datas);
	return *this;
}

Model& Model::Load(GLbitfield flags){
	if(!loaded){
		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
#ifdef flagOLDOPENGL
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * datas.GetCount(), &datas[0], GL_STATIC_DRAW);
#else
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * datas.GetCount(), datas, flags);
#endif
		
		loaded = true;
	}
	return *this;
}

void Model::Clear(){
	if(vbo != 0){
		glDeleteBuffers(1,&vbo);
	}
	datas.Clear();
	meshes.Clear();
	loaded=false;
}

bool Model::IsLoaded()const{
	return loaded;
}

GLuint Model::GetVBO(){
	return vbo;
}

Vector<Mesh>& Model::GetMeshes(){
	return meshes;
}

const Vector<Mesh>& Model::GetMeshes()const{
	return meshes;
}

String Model::ToString()const{
	return "vbo ID: " << AsString(vbo) << "\nTotal mesh count: " << meshes.GetCount();
}

Model::Model(){}

}