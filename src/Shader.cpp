#include "../Shader.h"
#include "../CGLCamera.h"
namespace Upp{

ShaderProgram::~ShaderProgram(){
	Clear();
};

bool ShaderProgram::RetrieveAllUniformsAttributesOnLink = true;
	
ShaderProgram::ShaderProgram(){
	beforeRendering = [&](Upp::ShaderProgram& shader, Upp::CGLCamera& camera){
		shader.Bind();
		shader.UniformMat4(shader.uniformViewMatrixName,camera.GetViewMatrix());
		shader.UniformMat4(shader.uniformProjectionMatrixName,camera.GetProjectionMatrix());
	};
}

ShaderProgram& ShaderProgram::Clear(){
	if(IsLinked()){
		Unbind();
		glDeleteProgram(programID);
	}
	if(vertexID != 0) glDeleteShader(vertexID);
	if(tcID != 0) glDeleteShader(tcID);
	if(teID != 0) glDeleteShader(teID);
	if(geometryID != 0) glDeleteShader(geometryID);
	if(fragmentID != 0) glDeleteShader(fragmentID);
	linked = false;
	pipelining = false;
	return *this;
}

ShaderProgram& ShaderProgram::Bind(){
	ASSERT_(linked, "ShaderProgram " + AsString(programID)  +" trying to be bind when programID it's not linked");
	glUseProgram(programID);
	return *this;
}
ShaderProgram& ShaderProgram::Unbind(){
	glUseProgram(0);
	return *this;
}

bool ShaderProgram::IsLinked()const{
	return linked;
}

ShaderProgram& ShaderProgram::LoadShaderFromFile(const String& path, ShaderType type){
	String str = LoadFile(path);
	if(str.GetCount()){
		try{
			LoadShader(str, type);
			return *this;
		}catch(Exc& exception){
			throw exception;
		}
	}else{
		throw Exc("Shader can't be load, file don't exist or is empty " + path);
	}
}

ShaderProgram& ShaderProgram::LoadShaderFromString(const String& str, ShaderType type){
	if(str.GetCount()){
		try{
			LoadShader(str, type);
			return *this;
		}catch(Exc& exception){
			throw exception;
		}
	}else{
		throw Exc("Shader can't be load, String is empty");
	}
}

GLuint ShaderProgram::GetProgramID()const{
	return programID;
}

bool ShaderProgram::Link(bool enableShaderPipelining){
	if(!linked){
		programID = glCreateProgram();
		if(vertexID != 0) glAttachShader(programID,vertexID);
		if(tcID != 0) glAttachShader(programID,tcID);
		if(teID != 0) glAttachShader(programID,teID);
		if(geometryID != 0) glAttachShader(programID,geometryID);
		if(fragmentID != 0) glAttachShader(programID,fragmentID);
		if(enableShaderPipelining) glProgramParameteri(programID, GL_PROGRAM_SEPARABLE, GL_TRUE);
		glLinkProgram(programID);
		
		try{
			CheckForLinking();
		}catch(Exc& exception){
			throw exception;
		}
		linked = true;
		if(RetrieveAllUniformsAttributesOnLink) RetrieveAllUniformsAttributes();
		return linked;
	}else{
		throw Exc("ShaderProgram \"" + AsString(programID) + "\" is already linked and can't be linked again !");
	}
}

String ShaderProgram::GetLog()const{
	char infoLog[4096];
	glGetProgramInfoLog(programID, 4096, NULL, infoLog);
	return String(infoLog);
}

bool ShaderProgram::HaveVertex(){return vertexID;}
bool ShaderProgram::HaveTesselationControl(){return tcID;}
bool ShaderProgram::HaveTesselationEvaluation(){return teID;}
bool ShaderProgram::HaveGeometry(){return geometryID;}
bool ShaderProgram::HaveFragment(){return fragmentID;}

String ShaderProgram::ToString()const{
	String toString;
	toString << "ShaderProgram ID :" << AsString(programID) << "\n";
	toString << "Status :" << ((linked)?"Linked":"Not linked") << "\n";
	if(linked){
		toString << "Shader on this program :" << ((vertexID)?"Vertex":"") <<  ((tcID)?", Tesselation_control":"") <<  ((teID)?", Tesselation_evaluation":"") << ((geometryID)? ", Geometry":"")  << ((fragmentID)?", Fragment":"") <<  "\n";
		toString << "Uniforms :\n";
		for(const String& str : uniforms.GetKeys()){
			const SingleValueShaderInformation& uniform = uniforms.Get(str);
			toString << "\t" << "Name :" << uniform.name << " Location:" << uniform.location << " Type:" << uniform.valueType <<"(enum " << AsString(uniform.enumType) << ")" <<"\n";
		}
		toString << "-------------------------\n";
		toString << "Attributes :\n";
		for(const String& str : attributes.GetKeys()){
			const SingleValueShaderInformation& attrib = attributes.Get(str);
			toString << "\t" << "Name :" << attrib.name << " Location:" << attrib.location << " Type:" << attrib.valueType <<"(enum " << AsString(attrib.enumType) << ")" <<"\n";
		}
		toString << "-------------------------\n";
	}
	String log = GetLog();
	if(log.GetCount()){
		toString << "Logs :" << log << "\n";
	}
	return toString;
}

/**
	Uniform and Buffer block management
**/

bool ShaderProgram::RetrieveAllUniformsAttributes(){
	if(linked){
		uniforms.Clear();
		attributes.Clear();
		
		GLint i;
		GLint count;
		GLint size;
		GLenum type;
		const GLsizei bufSize = 24;
		GLchar name[bufSize];
		GLsizei length;
		
		glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
		for (i = 0; i < count; i++){
		    glGetActiveAttrib(programID, (GLuint)i, bufSize, &length, &size, &type, name);
		    SingleValueShaderInformation& singleAttrib =  attributes.Add(name);
		    singleAttrib.name = name;
			singleAttrib.informationType = InformationShaderType::ATTRIBUTE;
			singleAttrib.valueType = GetTypeName(type);
			singleAttrib.enumType = type;
			singleAttrib.location = glGetAttribLocation(programID,name);
		}
		
		
		glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
		for (int i = 0; i < count; i++){
		    glGetActiveUniform(programID, (GLuint)i, bufSize, &length, &size, &type, name);
		    SingleValueShaderInformation& singleUniform=  uniforms.Add(name);
		    singleUniform.name = name;
			singleUniform.informationType = InformationShaderType::UNIFORM;
			singleUniform.valueType = GetTypeName(type);
			singleUniform.enumType = type;
			singleUniform.location = glGetUniformLocation(programID,name);
		}
		
		return true;
	}
	return false;
}

bool ShaderProgram::LoadShader(const char* shader, ShaderType type){
	if(!linked){
		GLuint shaderID = 0;
		switch(type){
			case ShaderType::VERTEX: shaderID = vertexID; break;
			case ShaderType::TESSELATION_CONTROL: shaderID = tcID; break;
			case ShaderType::TESSELATION_EVALUATION: shaderID = teID; break;
			case ShaderType::GEOMETRY: shaderID = geometryID; break;
			case ShaderType::FRAGMENT: shaderID = fragmentID; break;
			default:
				throw Exc("Unknow shader type");
		};
		
		if(shaderID){
			glDeleteShader(shaderID);
			shaderID = 0;
		}
		
		shaderID = glCreateShader((unsigned int) type);
		glShaderSource(shaderID, 1, &shader, NULL);
		glCompileShader(shaderID);
		
		try{
			CheckForCompilation(shaderID,type);
		}catch(Exc& exception){
			throw exception;
		}
		
		switch(type){
			case ShaderType::VERTEX: vertexID = shaderID; break;
			case ShaderType::TESSELATION_CONTROL: tcID = shaderID; break;
			case ShaderType::TESSELATION_EVALUATION: teID = shaderID; break;
			case ShaderType::GEOMETRY: geometryID = shaderID; break;
			case ShaderType::FRAGMENT: fragmentID = shaderID; break;
		};
		
		return true;
	}
	throw Exc("Can't load shader when ShaderProgram is already linked");
}


bool ShaderProgram::CheckForCompilation(GLuint shaderID ,ShaderType type){
	int success;
	char infoLog[4096];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success){
		glGetShaderInfoLog(shaderID, 4096, NULL, infoLog);
		throw Exc("Error " + GetShaderTypeName(type) +" shader compilation failled !\n" + String(infoLog));
    }
    return true;
}

String ShaderProgram::GetShaderTypeName(ShaderType type){
	String name = "unknow";
	switch(type){
		case ShaderType::VERTEX: name = "vertex"; break;
		case ShaderType::TESSELATION_CONTROL: name = "tesselation_control"; break;
		case ShaderType::TESSELATION_EVALUATION: name = "tesselation_evaluation"; break;
		case ShaderType::GEOMETRY: name = "geometry"; break;
		case ShaderType::FRAGMENT: name = "fragment"; break;
	};
	return name;
}

bool ShaderProgram::CheckForLinking(){
	int success;
    char infoLog[4096];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 4096, NULL, infoLog);
		throw Exc("Error Shader \""+ AsString(programID) +"\" can't be linked !\n" + String(infoLog));
	}
	return true;
}

String ShaderProgram::GetTypeName(GLenum typeName){
	String returnValue = "unknow";
	switch(typeName){
		case GL_BOOL: returnValue = "GLboolean"; break;
		case GL_BYTE: returnValue = "GLbyte"; break;
		case GL_UNSIGNED_BYTE: returnValue = "GLubyte"; break;
		case GL_SHORT: returnValue = "GLshort"; break;
		case GL_UNSIGNED_SHORT: returnValue = "GLushort"; break;
		case GL_INT: returnValue = "GLint"; break;
		case GL_UNSIGNED_INT: returnValue = "GLuint"; break;
		case GL_FIXED: returnValue = "GLfixed"; break;
		case GL_HALF_FLOAT: returnValue = "GLhalf"; break;
		case GL_FLOAT: returnValue = "GLfloat"; break;
		case GL_DOUBLE: returnValue = "GLdouble"; break;
		case GL_FLOAT_VEC4: returnValue = "GLvec4f"; break;
		case GL_FLOAT_VEC3: returnValue = "GLvec3f"; break;
		case GL_FLOAT_VEC2: returnValue = "GLvec2f"; break;
		case GL_FLOAT_MAT4: returnValue = "GLmat4f"; break;
		case GL_FLOAT_MAT3: returnValue = "GLmat3f"; break;
		case GL_FLOAT_MAT2: returnValue = "GLmat2f"; break;
	}
	return returnValue;
}

ShaderProgram& ShaderProgram::SetUniformViewMatrixName(const String& name){
	uniformViewMatrixName = name;
	return *this;
}

ShaderProgram& ShaderProgram::SetUniformProjectionMatrixName(const String& name){
	uniformProjectionMatrixName = name;
	return *this;
}

bool ShaderProgram::UniformExist(const String& uniformName){
	return (uniforms.Find(uniformName) == -1)? false:true;
}

bool ShaderProgram::AttributeExist(const String& attributeName){
	return (attributes.Find(attributeName) == -1)? false:true;
}

const Upp::ArrayMap<String, SingleValueShaderInformation>& ShaderProgram::GetUniforms(){
	return uniforms;
}

const Upp::ArrayMap<String, SingleValueShaderInformation>& ShaderProgram::GetAttributes(){
	return attributes;
}

ShaderProgram& ShaderProgram::UniformBool(const String& uniformName, bool value){
	if(uniforms.Find(uniformName) != -1)
		glUniform1i(uniforms.Get(uniformName).location,(int)value);
	return *this;
}

ShaderProgram& ShaderProgram::UniformInt(const String& uniformName, int value){
	if(uniforms.Find(uniformName) != -1)
		glUniform1i(uniforms.Get(uniformName).location,value);
	return *this;
}

ShaderProgram& ShaderProgram::UniformFloat(const String& uniformName, float value){
	if(uniforms.Find(uniformName) != -1)
		glUniform1f(uniforms.Get(uniformName).location,value);
	return *this;
}

ShaderProgram& ShaderProgram::UniformVec2(const String& uniformName, const glm::vec2 &value){
	if(uniforms.Find(uniformName) != -1)
		glUniform2fv(uniforms.Get(uniformName).location, 1, &value[0]);
	return *this;
}

ShaderProgram& ShaderProgram::UniformVec2(const String& uniformName, float x, float y){
	if(uniforms.Find(uniformName) != -1)
		glUniform2f(uniforms.Get(uniformName).location, x, y);
	return *this;
}

ShaderProgram& ShaderProgram::UniformVec3(const String& uniformName, const glm::vec3 &value){
	if(uniforms.Find(uniformName) != -1)
		glUniform3fv(uniforms.Get(uniformName).location, 1, &value[0]);
	return *this;
}

ShaderProgram& ShaderProgram::UniformVec3(const String& uniformName, float x, float y, float z){
	if(uniforms.Find(uniformName) != -1)
		glUniform3f(uniforms.Get(uniformName).location, x, y, z);
	return *this;
}

ShaderProgram& ShaderProgram::UniformVec4(const String& uniformName, const glm::vec4 &value){
	if(uniforms.Find(uniformName) != -1)
		glUniform4fv(uniforms.Get(uniformName).location, 1, &value[0]);
	return *this;
}

ShaderProgram& ShaderProgram::UniformVec4(const String& uniformName, float x, float y, float z, float w){
	if(uniforms.Find(uniformName) != -1)
		glUniform4f(uniforms.Get(uniformName).location, x, y, z, w);
	return *this;
}

ShaderProgram& ShaderProgram::UniformMat2(const String& uniformName, const glm::mat2 &mat){
	if(uniforms.Find(uniformName) != -1)
		glUniformMatrix2fv(uniforms.Get(uniformName).location, 1, GL_FALSE, &mat[0][0]);
	return *this;
}

ShaderProgram& ShaderProgram::UniformMat3(const String& uniformName, const glm::mat3 &mat){
	if(uniforms.Find(uniformName) != -1)
		glUniformMatrix3fv(uniforms.Get(uniformName).location, 1, GL_FALSE, &mat[0][0]);
	return *this;
}

ShaderProgram& ShaderProgram::UniformMat4(const String& uniformName, const glm::mat4 &mat){
	if(uniforms.Find(uniformName) != -1)
		glUniformMatrix4fv(uniforms.Get(uniformName).location, 1, GL_FALSE, &mat[0][0]);
	return *this;
}

}