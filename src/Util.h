#ifndef _pluginUFE_OpenGLRenderer_ShadersData_h_
#define _pluginUFE_OpenGLRenderer_ShadersData_h_
#include <UFEngine/UFEngine.h>
#include "../SGLRenderer.h"
#include "VerticesData.h"

#define STRINGIFY(...) #__VA_ARGS__
#define SHADER(version, shader) "#version " #version "\n" STRINGIFY(shader)

namespace Upp{
	
	
static String VertexShader =
	#include "../shaders/VertexShader.glsl"
;

static String FragmentNoTextreShader =
	#include "../shaders/FragmentNoTextureShader.glsl"
;

static String WireGeometryShader =
	#include "../shaders/WireGeometryShader.glsl"
;

static String WireFragmentShader =
	#include "../shaders/WireFragmentShader.glsl"
;


static String prefixName = "C3D";
static String rendererName = prefixName + "Renderer";
static String shaderName = prefixName + "Shader";

static String ConvertDrawType(DrawType drawType){
	switch(drawType){
		case DrawType::solid:
			return "solid";
			break;
		case DrawType::wire:
			return "wire";
			break;
	}
	throw Exc("OpenGLRenderer : Invalide draw type");
}

static void AddToRendererQueue(SGLRenderer& openGLRenderer, String ComponentName, DrawType drawType, Object& object){
	openGLRenderer.AddToRenderQueue(
		openGLRenderer.GetRendererManager().Get(rendererName),
		openGLRenderer.GetShaderManager().Get(shaderName + ConvertDrawType(drawType)),
	    openGLRenderer.GetModelManager().Get(prefixName + ComponentName),
	    object
	);
}
	
static void CreateRenderer(SGLRenderer& openGLRenderer){
	if(!openGLRenderer.GetRendererManager().Has(rendererName)){
		Renderer& renderer = openGLRenderer.GetRendererManager().Create(rendererName);
		renderer.GetVertexArrayAttribFormat().Append(
			Upp::Vector<Upp::VAAF>{
				Upp::VAAF(0, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float)), //Position
				Upp::VAAF(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)), //Normals
				Upp::VAAF(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float))  //TextCoords
			}
		);
		renderer.LoadVAO();
	}
}

static void CreateShader(SGLRenderer& openGLRenderer){
	try{
		if(! openGLRenderer.GetShaderManager().Has( shaderName + ConvertDrawType(DrawType::solid))){
			Upp::ShaderProgram& shader = openGLRenderer.GetShaderManager().Create(shaderName + ConvertDrawType(DrawType::solid));
			
			shader.LoadShaderFromString(VertexShader, Upp::ShaderType::VERTEX);
			shader.LoadShaderFromString(FragmentNoTextreShader, Upp::ShaderType::FRAGMENT);
			shader.Link();
			
			shader.SetUniformProjectionMatrixName("proj");
			shader.SetUniformViewMatrixName("view");
		}
		if(! openGLRenderer.GetShaderManager().Has( shaderName + ConvertDrawType(DrawType::wire))){
			Upp::ShaderProgram& shaderFilDeFer = openGLRenderer.GetShaderManager().Create(shaderName + ConvertDrawType(DrawType::wire));
	
			shaderFilDeFer.LoadShaderFromString(VertexShader, Upp::ShaderType::VERTEX);
			shaderFilDeFer.LoadShaderFromString(WireGeometryShader, Upp::ShaderType::GEOMETRY);
			shaderFilDeFer.LoadShaderFromString(WireFragmentShader, Upp::ShaderType::FRAGMENT);
			shaderFilDeFer.Link();
		
			shaderFilDeFer.SetUniformProjectionMatrixName("proj");
			shaderFilDeFer.SetUniformViewMatrixName("view");
		}
	}catch(Exc& exception){
		Cout() << exception << EOL;
		Exit(-1);
	}
}

}

#endif
