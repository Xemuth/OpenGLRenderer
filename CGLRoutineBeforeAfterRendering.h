#ifndef _pluginUFE_OpenGLRenderer_CGLRoutineBeforeAfterRendering_h_
#define _pluginUFE_OpenGLRenderer_CGLRoutineBeforeAfterRendering_h_
#include <UFEngine/UFEngine.h>

#include "Shader.h"

namespace Upp{

class Renderer;
class CGLCamera;

/*
	In UFEngine Component always start by C letter
*/

#define CGLROUTINE(arg1,arg2,arg3,arg4) [&](Upp::Renderer& arg1, Upp::ShaderProgram& arg2, Upp::CGLCamera& arg3, Upp::Object& arg4) ->void

class CGLRoutineBeforeAfterRendering : public Component{
	public:
		
		CGLRoutineBeforeAfterRendering(){}
		CGLRoutineBeforeAfterRendering(const CGLRoutineBeforeAfterRendering& routine) : Component(routine){
			*this = routine;
		}
		CGLRoutineBeforeAfterRendering& operator=(const CGLRoutineBeforeAfterRendering& routine){
			beforeRendering = routine.beforeRendering;
			afterRendering = routine.afterRendering;
			return *this;
		}
		
		Event<Renderer&, ShaderProgram&, CGLCamera&, Object&> beforeRendering;
		Event<Renderer&, ShaderProgram&, CGLCamera&, Object&> afterRendering;
		
		CGLRoutineBeforeAfterRendering& SetBeforeRendering(const Event<Renderer&, ShaderProgram&, CGLCamera&, Object&>& event){beforeRendering = event; return *this;}
		CGLRoutineBeforeAfterRendering& SetAfterRendering(const Event<Renderer&, ShaderProgram&, CGLCamera&, Object&>& event){afterRendering = event; return *this;}
		
		
		virtual String GetName()const{
			return "CGLRoutineBeforeAfterRendering";
		}
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			bool b1 = beforeRendering;
			bool b2 = afterRendering;
			toString << "Before rendering event set : " <<  AsString(b1) << "\n";
			toString << "After rendering event set : " <<  AsString(b2) << "\n";
			return toString;
		}
	protected:
		virtual CGLRoutineBeforeAfterRendering* Clone(Object& object) const{
			CGLRoutineBeforeAfterRendering* ptr = new CGLRoutineBeforeAfterRendering(*this);
			ptr->SetObject(object);
			return ptr;
		}
};

}
#endif
