#ifndef _pluginUFE_OpenGLRenderer_CGLColor_h_
#define _pluginUFE_OpenGLRenderer_CGLColor_h_

#include <UFEngine/UFEngine.h>
#include "OpenGLRenderer.h"


namespace Upp{

class CGLColor : public CGLRoutineBeforeAfterRendering{
	public:
		CGLColor(){
			SetBeforeRendering([&](Renderer& renderer, ShaderProgram& shader, CGLCamera& camera, Object& object){
				if(uniformName.GetCount() > 0){
					shader.UniformVec4(uniformName, color.x, color.y, color.z, color.w);
				}else{
					LOG("CGLColor exception : beforeRendering routine can't perform action on Object named "+ object.GetName() +", uniformName is empty");
				}
			});
		}
		
		CGLColor(const CGLColor& cglColor){
			*this = cglColor;
		}
		CGLColor& operator=(const CGLColor& cglColor){
			CGLRoutineBeforeAfterRendering::operator=(cglColor);
			color = cglColor.color;
			uniformName = cglColor.uniformName;
			return *this;
		}
		
		glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		String uniformName;
		
		CGLColor& SetColor(float r, float g, float b, float a){color = glm::vec4(FitColotFloat(r), FitColotFloat(g), FitColotFloat(b), FitColotFloat(a)); return *this;}
		CGLColor& SetColor(int r, int g, int b, int a){color = glm::vec4(ConvertIntToFloat(r), ConvertIntToFloat(g), ConvertIntToFloat(b), ConvertIntToFloat(a)); return *this;}
		
		CGLColor& SetUniformName(const String& _name){uniformName = _name; return *this;}
		
		virtual String ToString()const{
			String toString;
			toString << "Object name: " << GetObject().GetName() << "\n";
			toString << "Uniform name: " << uniformName << "\n";
			toString << "Purpose: bind a color to an Object\n";
			toString << "Actual color : RGBA(" << convertFloatToInt(color.x)  << "," << convertFloatToInt(color.y) << "," << convertFloatToInt(color.z) << "," << convertFloatToInt(color.w) << ")" << "\n";
			toString << Replace(CGLRoutineBeforeAfterRendering::ToString(), Vector<String>{"Object name: " << GetObject().GetName() << "\n"}, Vector<String>{""});
			return toString;
		}
		
		virtual String GetName()const{
			return "CGLColor";
		}
	protected:
		virtual CGLColor* Clone(Object& object) const{
			CGLColor* ptr = new CGLColor(*this);
			ptr->SetObject(object);
			return ptr;
		}
		
	private:
		int convertFloatToInt(float f)const{
			if(f > 1.0f) f = 1.0f;
			else if(f < 0.0f) f = 0.0f;
			return (int)(f * 255.0f);
		}
		float ConvertIntToFloat(int i)const{
			if(i > 255) i = 255;
			else if(i < 0) i = 0;
			return i/255.0f;
		}
		float FitColotFloat(float f)const{
			if(f > 1.0f) f = 1.0f;
			else if(f < 0.0f) f = 0.0f;
			return f;
		}
};


}




#endif
