#ifndef _pluginUFE_OpenGLRenderer_Shader_h_
#define _pluginUFE_OpenGLRenderer_Shader_h_

#include <Core/Core.h>
#include <plugin/glew/glew.h>
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>

namespace Upp{

class CGLCamera;
class Renderer;
class Model;
	
enum class ShaderType {
	VERTEX = GL_VERTEX_SHADER,
	TCS = GL_TESS_CONTROL_SHADER,
	TES = GL_TESS_EVALUATION_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};
	
class Shader : public Moveable<Shader> {
	private:
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
		bool LoadShader(const char* shader, GLenum type);
		bool CheckForCompilation(const String& shaderType, const GLuint& shader);
		bool CheckForLinking();
	public:
		Shader();
		virtual ~Shader();

		GLuint GetID()const{return ID;}
		bool ContainVertex()const{return ((linked)? vertex : false);}
		bool ContainTCS()const{return ((linked)? TCS : false);}
		bool ContainTES()const{return ((linked)? TES : false);}
		bool ContainGeometry()const{return ((linked)? geometry : false);}
		bool ContainFragment()const{return ((linked)? fragment : false);}
		String GetLogs()const{return log;}
		bool IsLinked()const{return linked;}
		
		String ToString()const;
		
		String uniformViewMatrixName;
		String uniformProjectionMatrixName;
		Shader& SetUniformViewMatrixName(const String& name){uniformViewMatrixName = name; return *this;}
		Shader& SetUniformProjectionMatrixName(const String& name){uniformProjectionMatrixName = name; return *this;}
		
		
		Event<Shader&, CGLCamera&> beforeRendering;
		Event<Shader&, CGLCamera&> afterRendering;
		
		//Can throw exception :
		Shader& LoadFromFile(const String& filepath, ShaderType type); //Load a shader from file, return true if compilation have succed, false if not
		Shader& LoadFromString(const String& str, ShaderType type); //Load a shader from string, return true if compilation have succed, false if not
		Shader& Link(); //Link program, return true if linking have succed, false if not
		
		void Bind();
		void Unbind();
		
		Shader& BindAttribute(const String& attributName, unsigned int layout);
		Shader& LoadAttribute();
		
		bool RegisterUniform(const String& uniformName);
		bool IsRegisteredUniform(const String& uniformName);
		
		Shader& UniformBool(String uniformName, bool value);
		Shader& UniformBool(GLuint uniformLocation, bool value);
		Shader& UniformInt(String uniformName, int value);
		Shader& UniformInt(GLuint uniformLocation, int value);
		Shader& UniformFloat(String uniformName, float value);
		Shader& UniformFloat(GLuint uniformLocation, float value);
		Shader& UniformVec2(String uniformName, const glm::vec2 &value);
		Shader& UniformVec2(GLuint uniformLocation, const glm::vec2 &value);
		Shader& UniformVec2(String uniformName, float x, float y);
		Shader& UniformVec2(GLuint uniformLocation, float x, float y);
		Shader& UniformVec3(String uniformName, const glm::vec3 &value);
		Shader& UniformVec3(GLuint uniformLocation, const glm::vec3 &value);
		Shader& UniformVec3(String uniformName, float x, float y, float z);
		Shader& UniformVec3(GLuint uniformLocation, float x, float y, float z);
		Shader& UniformVec4(String uniformName, const glm::vec4 &value);
		Shader& UniformVec4(GLuint uniformLocation, const glm::vec4 &value);
		Shader& UniformVec4(String uniformName, float x, float y, float z, float w);
		Shader& UniformVec4(GLuint uniformLocation, float x, float y, float z, float w);
		Shader& UniformMat2(String uniformName, const glm::mat2 &mat);
		Shader& UniformMat2(GLuint uniformLocation, const glm::mat2 &mat);
		Shader& UniformMat3(String uniformName, const glm::mat3 &mat);
		Shader& UniformMat3(GLuint uniformLocation, const glm::mat3 &mat);
		Shader& UniformMat4(String uniformName, const glm::mat4 &mat);
		Shader& UniformMat4(GLuint uniformLocation, const glm::mat4 &mat);
};


}






#endif
