#ifndef _TestShaderClass_Shader_h_
#define _TestShaderClass_Shader_h_
#include <Core/Core.h>
#include <plugin/glew/glew.h>
#include <plugin/glm/glm.hpp>
#include <plugin/glm/gtc/matrix_transform.hpp>

namespace Upp{
	
class CGLCamera;
class Renderer;
class Model;

enum class ShaderType{
	VERTEX = GL_VERTEX_SHADER,
	TESSELATION_CONTROL = GL_TESS_CONTROL_SHADER,
	TESSELATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};

enum class InformationShaderType{
	ATTRIBUTE,
	UNIFORM
};

struct SingleValueShaderInformation{
	String name;
	InformationShaderType informationType;
	String valueType;
	GLenum enumType;
	GLuint location;
};

class ShaderProgram : public Moveable<ShaderProgram> {
	public:
		ShaderProgram();
		~ShaderProgram();
		
		ShaderProgram& Bind();
		ShaderProgram& Unbind();
		
		
		ShaderProgram& LoadShaderFromFile(const String& path, ShaderType type);
		ShaderProgram& LoadShaderFromString(const String& str, ShaderType type);
		
		bool Link(bool enableShaderPipelining = false);
		
		ShaderProgram& Clear();
		
		Event<ShaderProgram&, CGLCamera&> beforeRendering;
		Event<ShaderProgram&, CGLCamera&> afterRendering;
		
		String GetLog()const;
		String ToString()const;

		bool IsLinked()const;

		bool HaveVertex();
		bool HaveTesselationControl();
		bool HaveTesselationEvaluation();
		bool HaveGeometry();
		bool HaveFragment();
		
		GLuint GetProgramID()const;

	private:
		friend class ShaderPipeline;
		bool linked = false;
		bool pipelining = false;
		
		ArrayMap<String, SingleValueShaderInformation> uniforms;
		ArrayMap<String, SingleValueShaderInformation> attributes;
		/*Upp::VectorMap<String, GLuint> bufferBlocks; //TODO
		Upp::VectorMap<String, GLuint> shaderSubRoutine;*/

		GLuint programID = 0;

		GLuint vertexID = 0;
		GLuint tcID = 0;
		GLuint teID = 0;
		GLuint geometryID = 0;
		GLuint fragmentID = 0;
		
		String GetShaderTypeName(ShaderType type);
		String GetTypeName(GLenum typeName);
		
		
		bool LoadShader(const char* shader, ShaderType type);
		bool CheckForCompilation(GLuint shaderID ,ShaderType type);
		bool CheckForLinking();
		
	public:
		/**
			Uniform and Buffer block management
		**/
		String uniformViewMatrixName;
		String uniformProjectionMatrixName;
		
		static bool RetrieveAllUniformsAttributesOnLink;
		bool RetrieveAllUniformsAttributes();
		
		ShaderProgram& SetUniformViewMatrixName(const String& name);
		ShaderProgram& SetUniformProjectionMatrixName(const String& name);
		
		bool UniformExist(const String& uniformName);
		bool AttributeExist(const String& attributeName);
		
		const ArrayMap<String, SingleValueShaderInformation>& GetUniforms();
		const ArrayMap<String, SingleValueShaderInformation>& GetAttributes();
		
		ShaderProgram& UniformBool(const String& uniformName, bool value);
		ShaderProgram& UniformInt(const String& uniformName, int value);
		ShaderProgram& UniformFloat(const String& uniformName, float value);
		ShaderProgram& UniformVec2(const String& uniformName, const glm::vec2 &value);
		ShaderProgram& UniformVec2(const String& uniformName, float x, float y);
		ShaderProgram& UniformVec3(const String& uniformName, const glm::vec3 &value);
		ShaderProgram& UniformVec3(const String& uniformName, float x, float y, float z);
		ShaderProgram& UniformVec4(const String& uniformName, const glm::vec4 &value);
		ShaderProgram& UniformVec4(const String& uniformName, float x, float y, float z, float w);
		ShaderProgram& UniformMat2(const String& uniformName, const glm::mat2 &mat);
		ShaderProgram& UniformMat3(const String& uniformName, const glm::mat3 &mat);
		ShaderProgram& UniformMat4(const String& uniformName, const glm::mat4 &mat);
};



class ShaderPipeline{
	//TODO

};



}

#endif
