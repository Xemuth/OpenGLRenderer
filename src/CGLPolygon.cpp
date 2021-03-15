#include "../CGLPolygon.h"
#include "Util.h"
/*
#include "../Model.h"
#include "../Renderer.h"
#include "../Shader.h"
*/
#include "../CGLModel.h"
#include "../CGLRenderer.h"
#include "../CGLShader.h"

namespace Upp{

CGLPolygon::CGLPolygon(SGLRenderer& _openGLRenderer) : openGLRenderer(_openGLRenderer){
	beforeRendering = [&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
		try{
			shader.UniformMat4("model",object.GetTransform().GetModelMatrix());
		}catch(Exc& exception){
			LOG("Cylinder component beforeRendering failled in object named '"+ GetObject().GetName() +"' : " + exception );
		}
	};
}

String CGLPolygon::ToString()const{
	String toString;
	toString << "Object name: " << GetObject().GetName() << "\n";
	toString << "Purpose: draw a complete prebuilt OpenGL model / Shader / Renderer \n";
	toString << "Model name: " << prefixName + GetName() << "\n";
	const SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
	String toString2;
	try{
		if(service.GetModelManager().Get(prefixName + GetName()).IsLoaded())
			toString2 << "This model is valide: \n\t" << Replace(service.GetModelManager().Get(prefixName + GetName()).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
		else
			throw Exc("Model named : " + prefixName + GetName() + " is invalide, it's not loaded");
	}catch(Exc& exception){
		toString2 ="";
		toString2 << "This model is invalide, Exception raised: " << exception;
	}
	toString << toString2 << "\n";
	String shader = shaderName + ConvertDrawType(drawType);
	if(customShader.GetCount())shader = customShader;
	toString << "Shader name: " << shader << "\n";
	try{
		toString2 ="";
		toString2 << "Custom Shader :" << bool(customShader.GetCount()) << "\n";
		if(service.GetShaderManager().Get(shader).IsLinked())
			toString2 << "This shader is valide: \n\t" << Replace(service.GetShaderManager().Get(shader).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
		else
			throw Exc("Shader named : " + shader + " is invalide, it's not linked");
	}catch(Exc& exception){
		toString2 ="";
		toString2 << "This shader is invalide, Exception raised: " << exception;
	}
	toString << toString2 << "\n";
	String renderer = rendererName;
	if(customRenderer.GetCount()) renderer = customRenderer;
	toString << "Renderer name: " << renderer << "\n";
	try{
		toString2="";
		toString2 << "Custom Renderer: " << bool(customRenderer.GetCount()) << "\n";
		if(service.GetRendererManager().Get(renderer).GetVAO() != 0)
			toString2 << "This renderer is valide: \n\t" << Replace(service.GetRendererManager().Get(renderer).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
		else
			throw Exc("Renderer named : " + renderer + " is invalide, it's not linked");
	}catch(Exc& exception){
		toString2 ="";
		toString2 << "This renderer is invalide, Exception raised: " << exception;
	}
	toString << toString2 << "\n";
	return toString;
}
	
void CGLPolygon::Update(double timeEllapsed, double deltaTime){
	String shader = shaderName + ConvertDrawType(drawType);
	if(customShader.GetCount()){
		shader = customShader;
	}else if(GetObject().GetComponentManager().Has<Upp::CGLShader>()){
		shader = GetObject().GetComponentManager().Get<Upp::CGLShader>().GetName();
	}
	
	String renderer = rendererName;
	if(customRenderer.GetCount()) renderer = customRenderer;
	
	openGLRenderer.AddToRenderQueue(
		openGLRenderer.GetRendererManager().Get(renderer),
		openGLRenderer.GetShaderManager().Get(shader),
	    openGLRenderer.GetModelManager().Get(prefixName + GetName()),
	    GetObject()
	);
}

CGLPolygon::CGLPolygon(const CGLPolygon& polygon) : openGLRenderer(polygon.openGLRenderer), CGLRoutineBeforeAfterRendering(polygon) {
	CGLRoutineBeforeAfterRendering::operator=(polygon);
	*this = polygon;
}

CGLPolygon& CGLPolygon::operator=(const CGLPolygon& polygon){
	CGLRoutineBeforeAfterRendering::operator=(polygon);
	customRenderer = polygon.customRenderer;
	customShader = polygon.customShader;
	drawType = polygon.drawType;
	return *this;
}

}