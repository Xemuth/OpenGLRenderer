#ifndef _pluginUFE_RendererOpenGL_ShaderProgram_h_
#define _pluginUFE_RendererOpenGL_ShaderProgram_h_
#include <Core/Core.h>
#include <plugin/glew/glew.h>
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>


namespace Upp{
	
enum class ShaderType {
	VERTEX = GL_VERTEX_SHADER,
	TCS = GL_TESS_CONTROL_SHADER,
	TES = GL_TESS_EVALUATION_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};
	
class ShaderProgram {
	private:
		String name="";
		GLuint ID = 0;
		
		GLuint vertex = 0;
		GLuint TCS = 0; //Tesselation control shader
		GLuint TES = 0; //Tesselation evaluation shader
		GLuint geometry = 0;
		GLuint fragment = 0;
		
		String log; //All log of shader
		
		VectorMap<String, GLuint> attributes;
		VectorMap<String, GLuint> uniforms;
		
		bool linked = false;
		

		String GetLastLog();
	protected:
		ShaderProgram& SetName(String& shaderName){name = shaderName; return *this;}
		
		bool LoadShader(const char* shader, GLenum type);
		bool CheckForCompilation(const String& shaderType, const GLuint& shader);
		bool CheckForLinking();
	public:
		ShaderProgram(String shaderName) : name(shaderName){}
		virtual ~ShaderProgram();
		
		ShaderProgram& operator=(ShaderProgram&& program);
		ShaderProgram& operator=(const ShaderProgram& program);
		
		String GetName()const{return name;}
		GLuint GetID()const{return ID;}
		bool ContainVertex()const{return ((linked)? vertex : false);}
		bool ContainTCS()const{return ((linked)? TCS : false);}
		bool ContainTES()const{return ((linked)? TES : false);}
		bool ContainGeometry()const{return ((linked)? geometry : false);}
		bool ContainFragment()const{return ((linked)? fragment : false);}
		String GetLogs()const{return log;}
		bool IsLinked()const{return linked;}
		
		//Can throw exception :
		ShaderProgram& LoadFromFile(String filepath, ShaderType type); //Load a shader from file, return true if compilation have succed, false if not
		ShaderProgram& LoadFromString(String str, ShaderType type); //Load a shader from string, return true if compilation have succed, false if not
		ShaderProgram& Link(); //Link program, return true if linking have succed, false if not
		
		void Bind();
		void Unbind();
		
		ShaderProgram& BindAttribute(String attributName, unsigned int layout);
		ShaderProgram& LoadAttribute();
		virtual ShaderProgram& BindAttributes() = 0;
		
		bool RegisterUniform(String uniformName);
		virtual ShaderProgram& BindUniforms() = 0;
		
		ShaderProgram& UniformBool(String uniformName, bool value);
		ShaderProgram& UniformBool(GLuint uniformLocation, bool value);
		ShaderProgram& UniformInt(String uniformName, int value);
		ShaderProgram& UniformInt(GLuint uniformLocation, int value);
		ShaderProgram& UniformFloat(String uniformName, float value);
		ShaderProgram& UniformFloat(GLuint uniformLocation, float value);
		ShaderProgram& UniformVec2(String uniformName, const glm::vec2 &value);
		ShaderProgram& UniformVec2(GLuint uniformLocation, const glm::vec2 &value);
		ShaderProgram& UniformVec2(String uniformName, float x, float y);
		ShaderProgram& UniformVec2(GLuint uniformLocation, float x, float y);
		ShaderProgram& UniformVec3(String uniformName, const glm::vec3 &value);
		ShaderProgram& UniformVec3(GLuint uniformLocation, const glm::vec3 &value);
		ShaderProgram& UniformVec3(String uniformName, float x, float y, float z);
		ShaderProgram& UniformVec3(GLuint uniformLocation, float x, float y, float z);
		ShaderProgram& UniformVec4(String uniformName, const glm::vec4 &value);
		ShaderProgram& UniformVec4(GLuint uniformLocation, const glm::vec4 &value);
		ShaderProgram& UniformVec4(String uniformName, float x, float y, float z, float w);
		ShaderProgram& UniformVec4(GLuint uniformLocation, float x, float y, float z, float w);
		ShaderProgram& UniformMat2(String uniformName, const glm::mat2 &mat);
		ShaderProgram& UniformMat2(GLuint uniformLocation, const glm::mat2 &mat);
		ShaderProgram& UniformMat3(String uniformName, const glm::mat3 &mat);
		ShaderProgram& UniformMat3(GLuint uniformLocation, const glm::mat3 &mat);
		ShaderProgram& UniformMat4(String uniformName, const glm::mat4 &mat);
		ShaderProgram& UniformMat4(GLuint uniformLocation, const glm::mat4 &mat);
};


class FreeShader : public ShaderProgram{
	public:
		FreeShader(const String& shaderName) : ShaderProgram(shaderName){}
		FreeShader(FreeShader&& shader) : ShaderProgram(shader.GetName()){*this = pick(shader);}
		FreeShader(const FreeShader& shader) : ShaderProgram(shader.GetName()){*this = clone(shader);}
		FreeShader& operator=(FreeShader&& shader){ ShaderProgram::operator=(pick(shader)); return *this;}
		FreeShader& operator=(const FreeShader& shader){ ShaderProgram::operator=(clone(shader)); return *this;}
		
		
		
		FreeShader& BindAttributes(){return *this;};
		FreeShader& BindUniforms(){return *this;};
};

}
#endif
