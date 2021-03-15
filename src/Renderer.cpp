#include "../Renderer.h"

namespace Upp{

VertexArrayAttribFormat::VertexArrayAttribFormat(GLuint _attribIndex, GLint _size, GLenum _type, GLboolean _normalized, GLuint _relativeOffset){
	attribIndex = _attribIndex;
	size = _size;
	type = _type;
	normalized = _normalized;
	relativeOffset = _relativeOffset;
}

VertexArrayAttribFormat& VertexArrayAttribFormat::SetAttribIndex(GLuint _attribIndex){attribIndex = _attribIndex; return *this;}
VertexArrayAttribFormat& VertexArrayAttribFormat::SetSize(GLint _size){size = _size; return *this;}
VertexArrayAttribFormat& VertexArrayAttribFormat::SetType(GLenum _type){type = _type; return *this;}
VertexArrayAttribFormat& VertexArrayAttribFormat::SetNormalized(GLboolean _normalized){normalized = _normalized; return *this;}
VertexArrayAttribFormat& VertexArrayAttribFormat::SetRelativeOffset(GLuint _relativeOffset){relativeOffset = _relativeOffset; return *this;}


Renderer::Renderer(){
	beforeRendering = [](Renderer& renderer, ShaderProgram& shader, CGLCamera&  camera) -> void {
		glBindVertexArray(renderer.GetVAO());
#ifndef flagOLDOPENGL
		for(VertexArrayAttribFormat& vaaf : renderer.GetVertexArrayAttribFormat()){
			glVertexArrayAttribBinding(renderer.GetVAO(), vaaf.attribIndex, 0);
		}
#endif
	};
	afterRendering = [](Renderer& renderer, ShaderProgram& shader, CGLCamera&  camera) -> void {
		glBindVertexArray(0);
	};
}

Renderer::~Renderer(){
	Clear();
}

void Renderer::Clear(){
	if(vao != 0){
		glDeleteBuffers(1, &vao);
	}
	arrayAttribFormat.Clear();
}
	
void Renderer::Render(Model& model, ShaderProgram& shader, CGLCamera& camera){
	if(whenRendered){
		whenRendered(*this, shader, model, camera);
	}else{
		
		GLuint sizeOfVertexBuffer = 0;
		for(VertexArrayAttribFormat& vaaf : arrayAttribFormat){
			sizeOfVertexBuffer += vaaf.size * sizeof(float);
		}
		
#ifdef flagOLDOPENGL
		glBindBuffer(GL_ARRAY_BUFFER, model.GetVBO());
		for(VertexArrayAttribFormat& vaaf : GetVertexArrayAttribFormat()){
			glVertexAttribPointer(vaaf.attribIndex, vaaf.size,  vaaf.type, vaaf.normalized,sizeOfVertexBuffer,(void*)vaaf.relativeOffset);
			glEnableVertexAttribArray(vaaf.attribIndex);
		}
#else
		glVertexArrayVertexBuffer(vao, 0, model.GetVBO(), 0, sizeOfVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, model.GetVBO());
#endif
		
		
		for(Mesh& mesh : model.GetMeshes()){
			glDrawArrays(GL_TRIANGLES, mesh.GetIndice(), mesh.GetVertexCount());
		}
	}
}

void Renderer::LoadVAO(){
	if(vao != 0){
		glDeleteBuffers(1, &vao);
	}
	if(customVAOLoader){
		customVAOLoader(*this);
	}else{
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glGenVertexArrays(1,&vao);
		glBindVertexArray(vao);
		
#ifndef flagOLDOPENGL
		for(VertexArrayAttribFormat& vaaf : arrayAttribFormat){
			glEnableVertexArrayAttrib(vao, vaaf.attribIndex);
			glVertexArrayAttribFormat(vao, vaaf.attribIndex, vaaf.size, vaaf.type, vaaf.normalized, vaaf.relativeOffset);
		}
#endif
	}
}

String Renderer::ToString()const{
	return "Vao ID: "<< AsString(vao);
}

Vector<VertexArrayAttribFormat>& Renderer::GetVertexArrayAttribFormat(){
	return arrayAttribFormat;
}
const Vector<VertexArrayAttribFormat>& Renderer::GetVertexArrayAttribFormat()const{
	return arrayAttribFormat;
}

GLuint Renderer::GetVAO(){
	return vao;
}
const GLuint Renderer::GetVAO()const{
	return vao;
}

Renderer& Renderer::SetVAO(GLuint _vao){
	vao = _vao;
	return *this;
}

}