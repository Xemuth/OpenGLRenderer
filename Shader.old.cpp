#include "Shader.h"
#include "CGLCamera.h"
namespace Upp{

Shader::Shader(){
	beforeRendering = [&](Upp::Shader& shader, Upp::CGLCamera& camera){
		shader.Bind();
		shader.UniformMat4(shader.uniformViewMatrixName,camera.GetViewMatrix());
		shader.UniformMat4(shader.uniformProjectionMatrixName,camera.GetProjectionMatrix());
	};
}

String Shader::GetLastLog(){
	/*int position = log.ReverseFind("\n", (log.GetCount() > 4)? log.GetCount() - 4 : 0);
	if(position != -1)*/
	if(log.GetCount() > 0)
		return log;
	else
		return "No log available";
}

bool Shader::CheckForCompilation(const String& shaderType, const GLuint& shader){
	int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		log << "Error " << shaderType << " shader compilation failled !\n" << String(infoLog) << "\n";
		LOG("Error " + shaderType +" shader compilation failled !\n" + String(infoLog));
		return false;
    }
    return true;
}

bool Shader::CheckForLinking(){
	int success;
    char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		log << "Error Shader \""+ AsString(ID) +"\" can't be linked !\n" + String(infoLog) << "\n";
		LOG("Error Shader \""+ AsString(ID) +"\" can't be linked !\n" + String(infoLog));
		return false;
	}
	return true;
}
	
bool Shader::LoadShader(const char* shader, GLenum type){
	if(!linked){
		GLuint buffer = 0;
		String shadType;
		switch(type){
			case GL_VERTEX_SHADER: buffer = vertex; shadType="VERTEX"; break;
			case GL_TESS_CONTROL_SHADER: buffer = TCS; shadType="TESS_CONT"; break;
			case GL_TESS_EVALUATION_SHADER: buffer = TES; shadType="TESS_EVAL"; break;
			case GL_GEOMETRY_SHADER: buffer = geometry; shadType="GEOMETRY"; break;
			case GL_FRAGMENT_SHADER: buffer = fragment; shadType="FRAGMENT"; break;
			default:
				log << "shader of Shader \"" + AsString(ID) +"\" can't be loaded, Unknow type !" << "\n";
				LOG("shader of Shader \"" + AsString(ID) +"\" can't be loaded, Unknow type !");
				return false;
			break;
		};
		if(buffer){
			glDeleteShader(buffer);
			buffer = 0;
			log << shadType << " shader of Shader \"" + AsString(ID) +"\" have been deleted to be realocated" << "\n";
			LOG(shadType +  " shader of Shader \"" + AsString(ID) +"\" have been deleted to be realocated");
		}
		buffer = glCreateShader(type);
	    glShaderSource(buffer, 1, &shader, NULL);
	    glCompileShader(buffer);
	    if(!CheckForCompilation(shadType,buffer))
	        buffer = 0;
		switch(type){
			case GL_VERTEX_SHADER: vertex = buffer; break;
			case GL_TESS_CONTROL_SHADER: TCS = buffer; break;
			case GL_TESS_EVALUATION_SHADER: TES = buffer; break;
			case GL_GEOMETRY_SHADER: geometry = buffer; break;
			case GL_FRAGMENT_SHADER: fragment = buffer; break;
		};
		return buffer;
	}else{
		log << "Shader \"" + AsString(ID) + "\" can't load any shader because program is already linked" << "\n";
		LOG("Shader \"" + AsString(ID) + "\" can't load any shader because program is already linked");
		return false;
	}
}

Shader::~Shader(){
	Unbind();
	if(linked){
		if(vertex != 0){
			glDetachShader(ID,vertex);
			glDeleteShader(vertex);
		}
		if(TCS != 0){
			glDetachShader(ID,TCS);
			glDeleteShader(TCS);
		}
		if(TES != 0){
			glDetachShader(ID,TES);
			glDeleteShader(TES);
		}
		if(geometry != 0){
			glDetachShader(ID,geometry);
			glDeleteShader(geometry);
		}
		if(fragment != 0){
			glDetachShader(ID,fragment);
			glDeleteShader(fragment);
		}
		glDeleteProgram(ID);
	}
}

Shader& Shader::LoadFromFile(const String& filepath, ShaderType type){ //Load a shader from file, return true if compilation have succed, false if not
	String str = LoadFile(filepath);
	if(str.GetCount()){
		if(!LoadShader(str,(unsigned int)type))
			throw Exc(GetLastLog());
	}else{
		log << "Shader can't be load, file don't exist or is empty ! " << filepath << "\n";
		LOG("Shader can't be load, file don't exist or is empty ! " + filepath);
		throw Exc(GetLastLog());
	}
	return *this;
}
Shader& Shader::LoadFromString(const String& str, ShaderType type){ //Load a shader from string, return true if compilation have succed, false if not
	if(!LoadShader(str,(unsigned int)type))
			throw Exc(GetLastLog());
	return *this;
}
Shader& Shader::Link(){ //Link program, return true if linking have succed, false if not
	if(!linked){
		ID = glCreateProgram();
		if(vertex != 0) glAttachShader(ID,vertex);
		if(TCS != 0) glAttachShader(ID,TCS);
		if(TES != 0) glAttachShader(ID,TES);
		if(geometry != 0) glAttachShader(ID,geometry);
		if(fragment != 0) glAttachShader(ID,fragment);
		glLinkProgram(ID);
		if(!CheckForLinking())
			throw Exc(GetLastLog());
		linked = true;
	}else{
		log << "ShaderProgram \"" + AsString(ID) + "\" is already link and can't be linked again !" << "\n";
		LOG("ShaderProgram \"" + AsString(ID) + "\" is already link and can't be linked again !");
		throw Exc(GetLastLog());
	}
	return *this;
}
void Shader::Bind(){
	if(linked) glUseProgram(ID);
}
void Shader::Unbind(){
	glUseProgram(0);
}
bool Shader::RegisterUniform(const String& uniformName){
	if(linked){
		bool uniformReplaced = false;
		if(uniforms.Find(uniformName) != -1) uniformReplaced = true;
		
		Bind();
		GLuint location = glGetUniformLocation(ID, uniformName.ToStd().c_str());
		if(location != -1){
			if(uniformReplaced){
				uniforms.Get(uniformName) = location;
				LOG("Uniform named \"" + uniformName + "\" of ShaderProgram \"" + AsString(ID)  + "\" have been replaced");
				log << "Uniform named \"" + uniformName + "\" of ShaderProgram \"" + AsString(ID)  + "\" have been replaced"<< "\n";
			}
			else uniforms.Add(uniformName) = location;
			return true;
		}else{
			LOG("Uniform named \"" + uniformName + "\" of ShaderProgram \"" + AsString(ID)  + "\" can't be found");
			log << "Uniform named \"" + uniformName + "\" of ShaderProgram \"" + AsString(ID)  + "\" can't be found"<< "\n";
			return false;
		}
	}
	LOG("Uniform named \"" + uniformName + "\" of ShaderProgram \"" + AsString(ID)  + "\" can't be found, Program is not linked !");
	log << "Uniform named \"" + uniformName + "\" of ShaderProgram \"" + AsString(ID)  + "\" can't be found, Program is not linked !"<< "\n";
	return false;
}

bool Shader::IsRegisteredUniform(const String& uniformName){
	if(linked){
		return uniforms.Find(uniformName);
	}return false;
}

Shader& Shader::BindAttribute(const String& attributName, unsigned int layout){
	if(linked){
		Bind();
		bool attributeReplace = false;
		if(attributes.Find(attributName) != -1) attributeReplace = true;
		
		glBindAttribLocation(ID,layout,attributName);
		if(attributeReplace){
			attributes.Get(attributName) = layout;
			LOG("Attribute layout "+ AsString(layout) +" named \"" + attributName + "\" of ShaderProgram \"" + AsString(ID)  + "\" have been replaced");
			log << "Attribute layout "+ AsString(layout) +" named \"" + attributName + "\" of ShaderProgram \"" + AsString(ID)  + "\" have been replaced"<< "\n";
		}else attributes.Add(attributName) = layout;
	}else{
		LOG("Attribute named \"" + attributName + "\" of Shader \"" + AsString(ID)  + "\" can't be defined, Program is not linked !");
		log << "Attribute named \"" + attributName + "\" of Shader \"" + AsString(ID)  + "\" can't be defined, Program is not linked !"<< "\n";
	}
	return *this;
}

Shader& Shader::LoadAttribute(){
	for(const String& str : attributes.GetKeys()){
		glBindAttribLocation(ID,attributes.Get(str),str);
	}
	return *this;
}

Shader& Shader::UniformBool(Upp::String name, bool value){
	if(uniforms.Find(name) != -1)
		glUniform1i(uniforms.Get(name),(int)value);
	return *this;
}

Shader& Shader::UniformInt(Upp::String name, int value){
	if(uniforms.Find(name) != -1)
		glUniform1i(uniforms.Get(name),(int)value);
	return *this;
}

Shader& Shader::UniformFloat(Upp::String name, float value){
	if(uniforms.Find(name) != -1)
		glUniform1f(uniforms.Get(name),value);
	return *this;
}
Shader& Shader::UniformVec2(Upp::String name, const glm::vec2 &value){
	if(uniforms.Find(name) != -1)
		glUniform2fv(uniforms.Get(name), 1, &value[0]);
	return *this;
}
Shader& Shader::UniformVec2(Upp::String name, float x, float y){
	if(uniforms.Find(name) != -1)
		glUniform2f(uniforms.Get(name), x, y);
	return *this;
}
Shader& Shader::UniformVec3(Upp::String name, const glm::vec3 &value){
	if(uniforms.Find(name) != -1)
		glUniform3fv(uniforms.Get(name), 1, &value[0]);
	return *this;
}
Shader& Shader::UniformVec3(Upp::String name, float x, float y, float z){
	if(uniforms.Find(name) != -1)
		glUniform3f(uniforms.Get(name), x, y, z);
	return *this;
}
Shader& Shader::UniformVec4(Upp::String name, const glm::vec4 &value){
	if(uniforms.Find(name) != -1)
		glUniform4fv(uniforms.Get(name), 1, &value[0]);
	return *this;
}
Shader& Shader::UniformVec4(Upp::String name, float x, float y, float z, float w){
	if(uniforms.Find(name) != -1)
		glUniform4f(uniforms.Get(name), x, y, z, w);
	return *this;
}
Shader& Shader::UniformMat2(Upp::String name, const glm::mat2 &mat){
	if(uniforms.Find(name) != -1)
		glUniformMatrix2fv(uniforms.Get(name), 1, GL_FALSE, &mat[0][0]);
	return *this;
}
Shader& Shader::UniformMat3(Upp::String name, const glm::mat3 &mat){
	if(uniforms.Find(name) != -1)
		glUniformMatrix3fv(uniforms.Get(name), 1, GL_FALSE, &mat[0][0]);
	return *this;
}
Shader& Shader::UniformMat4(Upp::String name, const glm::mat4 &mat){
	if(uniforms.Find(name) != -1)
		glUniformMatrix4fv(uniforms.Get(name), 1, GL_FALSE, &mat[0][0]);
	return *this;
}
Shader& Shader::UniformBool(GLuint UniformLocation, bool value){
	glUniform1i(UniformLocation,(int)value);
	return *this;
}
Shader& Shader::UniformInt(GLuint UniformLocation, int value){
	glUniform1i(UniformLocation,(int)value);
	return *this;
}
Shader& Shader::UniformFloat(GLuint UniformLocation, float value){
	glUniform1f(UniformLocation,(int)value);
	return *this;
}
Shader& Shader::UniformVec2(GLuint UniformLocation, const glm::vec2 &value){
	glUniform2fv(UniformLocation, 1, &value[0]);
	return *this;
}
Shader& Shader::UniformVec2(GLuint UniformLocation, float x, float y){
	glUniform2f(UniformLocation, x, y);
	return *this;
}
Shader& Shader::UniformVec3(GLuint UniformLocation, const glm::vec3 &value){
	glUniform3fv(UniformLocation, 1, &value[0]);
	return *this;
}
Shader& Shader::UniformVec3(GLuint UniformLocation, float x, float y, float z){
	glUniform3f(UniformLocation, x, y, z);
	return *this;
}
Shader& Shader::UniformVec4(GLuint UniformLocation, const glm::vec4 &value){
	glUniform4fv(UniformLocation, 1, &value[0]);
	return *this;
}
Shader& Shader::UniformVec4(GLuint UniformLocation, float x, float y, float z, float w){
	glUniform4f(UniformLocation, x, y, z, w);
	return *this;
}
Shader& Shader::UniformMat2(GLuint UniformLocation, const glm::mat2 &mat){
	glUniformMatrix2fv(UniformLocation, 1, GL_FALSE, &mat[0][0]);
	return *this;
}
Shader& Shader::UniformMat3(GLuint UniformLocation, const glm::mat3 &mat){
	glUniformMatrix3fv(UniformLocation, 1, GL_FALSE, &mat[0][0]);
	return *this;
}
Shader& Shader::UniformMat4(GLuint UniformLocation, const glm::mat4 &mat){
	glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, &mat[0][0]);
	return *this;
}

String Shader::ToString()const{
	return  "Shader ID: " << AsString(ID) <<"\nShader log: " << GetLogs();
}

}
