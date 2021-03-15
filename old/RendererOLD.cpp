#include "Renderer.h"
#include "CGLCamera.h"
namespace Upp{

Renderer::Renderer(const String& shaderName) : shader(shaderName){
	beforeRendering = [&](Renderer& renderer, CGLCamera& camera) -> void{
		if( renderer.uniformProjectionMatrixName.GetCount() && renderer.uniformViewMatrixName.GetCount()){
			renderer.GetShaderProgram().Bind();
			renderer.GetShaderProgram().UniformMat4(renderer.uniformViewMatrixName,camera.GetViewMatrix());
			renderer.GetShaderProgram().UniformMat4(renderer.uniformProjectionMatrixName,camera.GetProjectionMatrix());
		}
	};
}

Renderer::Renderer(Renderer&& renderer) : shader(pick(renderer.shader)){
	*this = pick(renderer);
}

Renderer::Renderer(const Renderer& renderer) : shader(clone(renderer.shader)){
	*this = clone(renderer);
}

Renderer::~Renderer(){
	if(vao != 0){
		glDeleteVertexArrays(1,&vao);
	}
}

Renderer& Renderer::operator=(Renderer&& renderer){
	vao = renderer.vao;
	shader = pick(renderer.shader);
	beforeRendering = renderer.beforeRendering;
	uniformProjectionMatrixName = renderer.uniformProjectionMatrixName;
	uniformViewMatrixName = renderer.uniformViewMatrixName;
	renderer.vao = 0;
	renderer.beforeRendering = Event<Renderer&, CGLCamera&>();
	return *this;
}

Renderer& Renderer::operator=(const Renderer& renderer){
	vao = renderer.vao;
	shader = clone(renderer.shader);
	beforeRendering = renderer.beforeRendering;
	uniformProjectionMatrixName = renderer.uniformProjectionMatrixName;
	uniformViewMatrixName = renderer.uniformViewMatrixName;
	return *this;
}

void Renderer::Render(MeshData& meshData){
	if(meshData.IsLoaded()){
		if(whenRendered){
			whenRendered(*this, meshData);
		}
	}
}

String Renderer::ToString()const{
	return "Vao ID: "<< AsString(vao) << "\n" << "Shader name: " << shader.GetName() <<"\nShader ID: " << shader.GetID() <<"\nShader log: " << shader.GetLogs();
}

void Renderer::GenerateDefaultVAO(){
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	
	//Setting up the VAO Attribute format
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0* sizeof(float)); //Will be colors (R G B in float)
	
	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)); //Will be texture coordinates

	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float)); //Normals
	
	glEnableVertexArrayAttrib(vao, 3);
	glVertexArrayAttribFormat(vao, 3, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float)); //Will be my position
	
	whenRendered = [&](Renderer& renderer, MeshData& meshData) -> void{
		glBindBuffer(GL_ARRAY_BUFFER, meshData.GetVBO());
		glBindVertexArray(vao);
		glVertexArrayVertexBuffer(vao, 0, meshData.GetVBO(), 0, 11 * sizeof(float));
		//Binding the VBO to be read by VAO
		glVertexArrayAttribBinding(vao, 0, 0);
		glVertexArrayAttribBinding(vao, 1, 0);
		glVertexArrayAttribBinding(vao, 2, 0);
		glVertexArrayAttribBinding(vao, 3, 0);
		//shader.Bind();
		for(Mesh& mesh : meshData.GetMeshes()){
			glDrawArrays(GL_TRIANGLES, mesh.GetIndice(), mesh.GetVertexCount());
		}
		//shader.UnBind();
		glBindVertexArray(0);
	};
	
}

}