#include "MeshData.h"

namespace Upp{
//Mesh
Mesh::Mesh(int _indice, int _vertexCount){
	indice = _indice;
	vertexCount = _vertexCount;
}
Mesh::Mesh(Mesh&& mesh){
	*this = mesh;
}
Mesh::Mesh(const Mesh& mesh){
	*this = mesh;
}
Mesh::~Mesh(){
	
}
Mesh& Mesh::operator=(Mesh&& mesh){
	indice = mesh.indice;
	vertexCount = mesh.vertexCount;
	return *this;
}
Mesh& Mesh::operator=(const Mesh& mesh){
	indice = mesh.indice;
	vertexCount = mesh.vertexCount;
	return *this;
}
int Mesh::GetIndice()const{
	return indice;
}
int Mesh::GetVertexCount()const{
	return vertexCount;
}
//MeshData
MeshData::MeshData(){
	
}
MeshData::MeshData(MeshData&& meshdata){
	*this = meshdata;
}
MeshData::MeshData(const MeshData& meshdata){
	*this = meshdata;
}
MeshData::~MeshData(){
	if(loaded) glDeleteBuffers(1,&vbo);
}
MeshData& MeshData::operator=(MeshData&& meshdata){
	loaded = meshdata.loaded;
	
	datas = clone(meshdata.datas);
	meshes = pick(meshdata.meshes);
	
	vbo = meshdata.vbo;
	meshdata.loaded = false;
	return *this;
}
MeshData& MeshData::operator=(const MeshData& meshdata){
	loaded = meshdata.loaded;
	
	datas = clone(meshdata.datas);
	meshes = clone(meshdata.meshes);
	
	vbo = meshdata.vbo;
	return *this;
}

MeshData& MeshData::AddMesh(const Vector<GLfloat>& _datas, int verticeCount){
	meshes.Create(datas.GetCount(),verticeCount);
	datas.Append(_datas);
	return *this;
}

/*
MeshData& MeshData::AddMesh(const Vector<GLfloat>& _vertices, const Vector<GLfloat>& _normals, const Vector<GLfloat>& _colors, const Vector<GLfloat>& _texCoords){
	int numberOfIteration = _vertices.GetCount()/3;
	meshes.Create(datas.GetCount(),numberOfIteration);
	
	int iterator = 0;
	int iterator2 = 0;
	for(int e = 0; e < numberOfIteration; e++){
		iterator = 3 * e;
		iterator2 = 2 * e;
		datas << _vertices[iterator + 0]  << _vertices[iterator + 1] << _vertices[iterator + 2];
		
		if(_normals.GetCount() == 0){
			datas << 0.0f << 0.0f << 0.0f;
		}else{
			datas << _normals[iterator + 0]   << _normals[iterator + 1]  << _normals[iterator +2];
		}
		
		if(_colors.GetCount() == 0){
			datas << 0.0f << 0.0f << 0.0f;
		}else{
			datas << _colors[iterator + 0]   << _colors[iterator + 1]  << _colors[iterator + 2];
		}
		
		if(_texCoords.GetCount() == 0){
			datas << 0.0f << 0.0f;
		}else{
			datas << _texCoords[iterator2 + 0] << _texCoords[iterator2 + 1];
		}
	}
	
	return *this;
}*/


MeshData& MeshData::Load(){
	if(!loaded){
		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * datas.GetCount(), datas, GL_DYNAMIC_STORAGE_BIT);
		loaded = true;
	}
	return *this;
}

String MeshData::ToString()const{
	return "vbo ID: " << AsString(vbo) << "\nTotal mesh count: " << meshes.GetCount();
}
}
