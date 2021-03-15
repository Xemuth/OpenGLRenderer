#ifndef _pluginUFE_OpenGLRenderer_CGLShader_h_
#define _pluginUFE_OpenGLRenderer_CGLShader_h_

#include <UFEngine/UFEngine.h>
#include "CGLRenderer.h"

namespace Upp{

/*
	In UFEngine Component always start by C letter
*/

class CGLShader : public CGLRoutineBeforeAfterRendering{
	public:
		CGLShader(){
			beforeRendering = [&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object) -> void{
				try{
					if(!uniformModelMatrixName.GetCount())throw Exc("no modelMatrix name have been set");
					shader.UniformMat4(uniformModelMatrixName,object.GetTransform().GetModelMatrix());
				}catch(Exc& exception){
					LOG("CGLShader component beforeRendering failled in object named '"+ GetObject().GetName() +"' : " + exception );
				}
			};
		}
		CGLShader(const CGLShader& cglShader){
			*this = cglShader;
		}
		CGLShader& operator=(const CGLShader& cglShader){
			CGLRoutineBeforeAfterRendering::operator=(cglShader);
			shader = cglShader.shader;
			uniformModelMatrixName = cglShader.uniformModelMatrixName;
			return *this;
		}
				
		virtual void Update(double timeEllapsed, double deltaTime = 0.0){
			try{
				if(!shader.GetCount())throw Exc("no renderer name have been set");
				
				SGLRenderer& sglService = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
				ShaderProgram& shad = sglService.GetShaderManager().Get(shader);
				Model& model = sglService.GetModelManager().Get(GetObject().GetComponentManager().Get<CGLModel>().model);
				Renderer& renderer = sglService.GetRendererManager().Get(GetObject().GetComponentManager().Get<CGLRenderer>().renderer);
				
				sglService.AddToRenderQueue(renderer,shad, model, GetObject());
			}catch(Exc& exception){
				LOG("CGLRenderer update of " + GetObject().GetName() + " failled : " + exception);
			}
		}
		
		String shader;
		String uniformModelMatrixName;
		
		CGLShader& SetShader(const String& shaderName){shader = shaderName; return *this;}
		CGLShader& SetUniformModelMatrixName(const String& modelMatrixName){uniformModelMatrixName = modelMatrixName; return *this;}
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Shader name: " << shader << "\n";
			toString << "Purpose: Specify a shader to use by renderer (meshs data) to an object\n";
			String toString2;
			try{
				const SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
				toString2 << "This shader is valide: \n\t" << Replace(service.GetShaderManager().Get(shader).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}); ;
			}catch(Exc& exception){
				toString2 ="";
				toString2 << "This shader is invalide, Exception raised: " << exception;
			}
			toString << toString2;
			toString << Replace(CGLRoutineBeforeAfterRendering::ToString(), Vector<String>{"Object name: " << GetObject().GetName() << "\n"}, Vector<String>{""});
			return toString;
		}
		virtual String GetName()const{
			return "CGLShader";
		}
	protected:
		virtual CGLShader* Clone(Object& object) const{
			CGLShader* ptr = new CGLShader(*this);
			ptr->SetObject(object);
			return ptr;
		}
};

}

#endif
