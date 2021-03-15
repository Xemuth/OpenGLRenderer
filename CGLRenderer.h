#ifndef _pluginUFE_OpenGLRenderer_CGLRenderer_h_
#define _pluginUFE_OpenGLRenderer_CGLRenderer_h_

#include <UFEngine/UFEngine.h>

namespace Upp{

class Renderer;
class CGLCamera;

/*
	In UFEngine Component always start by C letter
*/

class CGLRenderer : public CGLRoutineBeforeAfterRendering{
	public:
		String renderer="";
		CGLRenderer(){}
		
		CGLRenderer(const CGLRenderer& cglRenderer){
			*this = cglRenderer;
		}
		CGLRenderer& operator=(const CGLRenderer& cglRenderer){
			CGLRoutineBeforeAfterRendering::operator=(cglRenderer);
			renderer = cglRenderer.renderer;
			return *this;
		}
		
		CGLRenderer& SetRenderer(const String& rendererName){renderer = rendererName; return *this;}

		virtual String GetName()const{
			return "CGLRenderer";
		}

		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Renderer name: " << renderer << "\n";
			toString << "Purpose: Bind a renderer (a way of drawing meshData) to an object\n";
			String toString2;
			try{
				const SGLRenderer& service = GetObject().GetScene().GetContext().GetServiceManager().Get<SGLRenderer>();
				if(service.GetRendererManager().Get(renderer).GetVAO() != 0)
					toString2 << "This renderer is valide: \n\t" << Replace(service.GetRendererManager().Get(renderer).ToString(), Vector<String>{"\n"}, Vector<String>{"\n\t"}) << "\n";
				else
					throw Exc("Renderer named : " + renderer + " is invalide, is VAO is equal to 0");
			}catch(Exc& exception){
				toString2 ="";
				toString2 << "This renderer is invalide, Exception raised: " << exception;
			}
			toString << toString2;
			toString << Replace(CGLRoutineBeforeAfterRendering::ToString(), Vector<String>{"Object name: " << GetObject().GetName() <<"\n"}, Vector<String>{""});
			return toString;
		}
	protected:
		virtual CGLRenderer* Clone(Object& object) const{
			CGLRenderer* ptr = new CGLRenderer(*this);
			ptr->SetObject(object);
			return ptr;
		}
};

}
#endif
